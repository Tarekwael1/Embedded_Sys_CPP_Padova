/**
 * @file AssemblyStation.cpp
 * @brief Assembly Station implementation
 */

/******************************Project Headers*****************************************/
#include "AssemblyStation.h"
#include "ControlCenter.h"
#include "AGV.h"
/*************************************************************************************/
/*****************************Standard Libraries***************************************/
#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include <string>
#include <algorithm>
/*************************************************************************************/


/**************************AssemblyStation Methods***********************************/

/**
 * @brief Constructor for AssemblyStation
 * @param wh Pointer to the Warehouse instance
 * @param fleet Pointer to the vector of AGV pointers
 * @return void
 */
AssemblyStation::AssemblyStation(Warehouse* wh, std::vector<AGV*>* fleet)
    : warehouse(wh),
      agv_fleet(fleet),
      control_center(nullptr),
      products(nullptr),
      running(false),
      current_sim_time_minutes(0),
      setup_time_minutes(5),
      station_count(1),
      ready_sequence(0),
      total_busy_time_minutes(0),
      orders_completed(0) {
}


/**
 * @brief Destructor for AssemblyStation
 */
AssemblyStation::~AssemblyStation() {
    stop();
}


/**
 * @brief Start the assembly station processing thread
 */
void AssemblyStation::start() {
    running = true;
    total_busy_time_minutes = 0;
    orders_completed = 0;
    ready_sequence = 0;
    station_virtual_time_minutes.assign(station_count, 0);
    last_product_processed.assign(station_count, std::string());
    {
        std::lock_guard<std::mutex> ready_lock(ready_mutex);  //<---------RC--------read-----------------
        while (!ready_queue.empty()) {
            ready_queue.pop();
        } 
    }
    station_threads.clear();
    for (int i = 0; i < station_count; ++i) { //This makes 
        station_threads.emplace_back(&AssemblyStation::process_orders, this, i);
    }
    staging_thread = std::thread(&AssemblyStation::staging_loop, this);  //Start staging thread
}


/**
 * @brief Stop the assembly station processing thread
 */
void AssemblyStation::stop() {
    running = false;  //Common stop flag for all threads (Atomic)
    order_cv.notify_all();  //<--------DL-------------N------------------
    ready_cv.notify_all();  //<--------DL--------------N(AGV)-----------------
    if (staging_thread.joinable()) {
        staging_thread.join();
    }
    for (auto& thread : station_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    station_threads.clear();
}


/**
 * @brief Main processing loop for assembly station
 */
void AssemblyStation::process_orders(int line_id) {
    while (true) {
        ReadySlot slot;
        {
            std::unique_lock<std::mutex> ready_lock(ready_mutex);      //<-----------Busy waiting Efficiency sleep---------
            ready_cv.wait(ready_lock, [this] { return !running || !ready_queue.empty(); }); //Waits new order or stop signal
            if (!running && ready_queue.empty()) {
                break;
            }
            if (ready_queue.empty()) {
                continue;
            }
            slot = ready_queue.top(); //Get the first ready slot (FIFO)
            ready_queue.pop();
        }

        Order order = slot.order;
        int base_time = get_base_time(order.product_id);
        int setup_time = setup_time_minutes;
        int start_time = 0;
        {
            std::lock_guard<std::mutex> timing_lock(timing_mutex);      //Common between multiple threads (RC or incorrect timing)
            if (line_id < (int)last_product_processed.size() && last_product_processed[line_id] == order.product_id) {
                setup_time = 0;
            }
            if (line_id >= (int)last_product_processed.size()) {
                last_product_processed.resize(line_id + 1);
            }
            last_product_processed[line_id] = order.product_id;
            if (line_id >= (int)station_virtual_time_minutes.size()) {
                station_virtual_time_minutes.resize(line_id + 1, 0);
            }
            start_time = std::max(order.release_time_minutes, station_virtual_time_minutes[line_id]); //Once the order arrives at the station start time or after the previous order
        }
        int operation_time = base_time + setup_time;
        int completion_time = start_time + operation_time;
        {
            std::lock_guard<std::mutex> timing_lock(timing_mutex);  //<-----------------------------------------
            station_virtual_time_minutes[line_id] = completion_time;
        }
        total_busy_time_minutes.fetch_add(operation_time, std::memory_order_relaxed); //Update busy time statistic(Atomic)

        std::this_thread::sleep_for(std::chrono::milliseconds(operation_time * 10)); //Simulate assembly time
        orders_completed.fetch_add(1, std::memory_order_relaxed); //Update completed orders statistic(Atomic)
        if (control_center) { control_center->mark_order_completed(order.order_id, completion_time); } //Notify control center of completion

        bool dispatched = false;
        for (int retry = 0; retry < 50 && !dispatched; ++retry) { //Try to dispatch finished product return
            for (size_t i = 0; i < agv_fleet->size(); ++i) {
                AGV* agv = (*agv_fleet)[i];
                if (agv->assign_task(order.product_id, 1, "WAREHOUSE", this, true)) {
                    if (control_center) {
                        control_center->log_event("[Diag] assign finished product " + order.product_id + " to AGV" + std::to_string(agv->get_id()));
                    }
                    dispatched = true;
                    break;
                }
            }
            if (!dispatched) std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        if (!dispatched && control_center) {
            control_center->log_event("[Diag] could not dispatch finished product return for " + order.product_id + " immediately");
        }
    }
}

/**
 * @brief Staging loop to handle component requests,
 * Pulls orders from the order queue and requests components,
 * Assigns AGVs to deliver components to the assembly station.
 * @return void
 */
void AssemblyStation::staging_loop() {
    while (running) {
        Order order;
        {
            std::unique_lock<std::mutex> lock(queue_mutex); //<--------Polling, DL------------------
            order_cv.wait(lock, [this] { return !running || !order_queue.empty(); }); //Waits new order or stop signal
            if (!running && order_queue.empty()) break;
            if (order_queue.empty()) continue;
            order = order_queue.front();
            order_queue.pop(); // Remove order from queue(FIFO)
        }

        if (!request_components(order)) {
            int attempts = ++retry_counts[order.order_id];
            if (attempts > max_request_retries) {
                if (control_center) control_center->log_event("[Diag] request_components failed permanently for order ID " + std::to_string(order.order_id));
                if (control_center) control_center->mark_order_canceled(order.order_id);
                continue;
            }
            if (control_center) control_center->log_event("[Diag] request_components failed (attempt " + std::to_string(attempts) + ") requeue order " + order.product_id);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            {
                std::lock_guard<std::mutex> relock(queue_mutex); //<---Re-lock to push back the order into the queue
                order_queue.push(order);
            }
            order_cv.notify_one(); // Notify staging thread of new order //<------------
            continue;
        }

        retry_counts.erase(order.order_id);
    }
}

/**
 * @brief Request components for an order and assign AGVs for delivery
 * @param order The order for which components are requested
 * @return true if components requested successfully, false otherwise
*/
bool AssemblyStation::request_components(const Order& order) {
    if (!products || !agv_fleet || agv_fleet->empty()) {
        return false;
    }
    
    auto it = products->find(order.product_id);
    if (it == products->end()) {
        return false;
    }
    
    const Product& product = it->second;
    if (!warehouse->reserve_components(product.bom)) {
        return false;
    }
    
    {
        std::lock_guard<std::mutex> lk(delivery_mutex); //<---------------
        pending_deliveries[order.order_id].clear(); 
        for (const auto& kv : product.bom) { //Check each component in BOM and the missing quantity
            pending_deliveries[order.order_id][kv.first] = kv.second;
        }
    }
    {
        std::lock_guard<std::mutex> stage_lock(staging_mutex); //<-----------Stores order until all components arrive.
        staging_orders[order.order_id] = order;
    }
    
    int agv_index = 0;
    for (const auto& component : product.bom) {  //Each component unit becomes one AGV task.
        const std::string& comp_id = component.first;
        int qty = component.second;
        for (int q = 0; q < qty; ++q) {
            bool assigned = false;
            for (int retry = 0; retry < 50 && !assigned; ++retry) { //<--Try to assign AGV for delivery
                for (size_t i = 0; i < agv_fleet->size(); i++) {
                    AGV* agv = (*agv_fleet)[(agv_index + i) % agv_fleet->size()];
                    if (agv->assign_task(comp_id, 1, "ASSEMBLY_STATION", this, false, order.order_id)) {
                        if (control_center) {
                            control_center->log_event("[Diag] assign_task " + comp_id + " to AGV" + std::to_string(agv->get_id()) + " (order " + std::to_string(order.order_id) + ")");
                        }
                        assigned = true;
                        agv_index = (agv_index + i + 1) % agv_fleet->size();
                        break;
                    }
                }
                if (!assigned) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
            if (!assigned) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                --q;
            }
        }
    }
    return true;
}


/**
 * @brief Notify the assembly station that a component has been delivered
 * @param order_id The ID of the order
 * @param component_id The ID of the delivered component
 * @param quantity The quantity delivered
 */
void AssemblyStation::notify_component_delivered(int order_id, const std::string& component_id, int quantity) {
    if (control_center) control_center->log_event("[Diag] delivered " + component_id + " x" + std::to_string(quantity) + " for order " + std::to_string(order_id));
    Order ready_order;
    bool order_ready = false;
    {
        std::lock_guard<std::mutex> lk(delivery_mutex);    //<------------Updates remaining quantity.
        auto order_it = pending_deliveries.find(order_id);  //search for the order
        if (order_it == pending_deliveries.end()) {
            return;
        }
        auto comp_it = order_it->second.find(component_id);
        if (comp_it != order_it->second.end()) {
            comp_it->second -= quantity;
            if (comp_it->second <= 0) comp_it->second = 0;
        }
        bool all_done = true;
        for (const auto& kv : order_it->second) {
            if (kv.second > 0) {
                all_done = false;
                break;
            }
        }
        if (all_done) { //Checks if entire BOM is fulfilled.
            pending_deliveries.erase(order_it);
            {
                std::lock_guard<std::mutex> stage_lock(staging_mutex); //<---------Moves order from staging → ready.
                auto staged = staging_orders.find(order_id);
                if (staged != staging_orders.end()) {
                    ready_order = staged->second;
                    staging_orders.erase(staged);
                    order_ready = true;
                }
            }
        }
    }

    if (order_ready) {
        if (control_center) control_center->log_event("[Diag] all components delivered for order " + std::to_string(order_id));
        std::lock_guard<std::mutex> ready_lock(ready_mutex); //block to push ready order <<-------------------------------------
        ReadySlot slot{ready_order, get_base_time(ready_order.product_id), ready_sequence.fetch_add(1, std::memory_order_relaxed)};
        ready_queue.push(slot);
        ready_cv.notify_all();  // Wake all station threads so any idle one can pick up wor, if there are multiple lines 
    }
}

/**
 * @brief Check if the assembly station is currently processing orders
 * @return true if processing, false otherwise
 */
bool AssemblyStation::is_processing() const {
    bool has_waiting = false;
    {
        std::lock_guard<std::mutex> lock(queue_mutex); //Block to check order queue 
        has_waiting = !order_queue.empty();
    }
    if (has_waiting) {
        return true;
    }
    std::lock_guard<std::mutex> ready_lock(ready_mutex); //Block to check ready queue
    return !ready_queue.empty();
}


/**
 * @brief Calculate operation time for a product
 * @param product_id The ID of the product
 * @return Operation time in minutes
 */
int AssemblyStation::calculate_operation_time(const std::string& product_id) {
    return get_base_time(product_id) + setup_time_minutes;
}


/**
 * @brief Get the base assembly time for a product
 * @param product_id The ID of the product
 * @return Base assembly time in minutes
 */
int AssemblyStation::get_base_time(const std::string& product_id) const {
    if (!products) {
        return 30;
    }
    auto it = products->find(product_id);
    if (it == products->end()) {
        return 30;
    }
    return it->second.base_assembly_time_minutes;
}


/**
 * @brief Add an order to the assembly station queue
 * @param order The order to add
 */
void AssemblyStation::add_order(const Order& order) {
    std::lock_guard<std::mutex> lock(queue_mutex); //<-----------------------------------------------------------------------------
    order_queue.push(order);    //Safe push to order queue
    order_cv.notify_one(); //<----------------------------------------------------------------------------- 
}


/**
 * @brief Set the current simulation time in minutes
 * @param minutes The current simulation time
 */
void AssemblyStation::set_simulation_time(int minutes) {
    current_sim_time_minutes = minutes;
}


/**
 * @brief Notify the assembly station that a finished product has been delivered
 * @param product_id The ID of the delivered finished product
 */
void AssemblyStation::notify_finished_product_delivered(const std::string& product_id) {
    if (control_center) {
        control_center->log_event("[Diag] finished product delivered " + product_id);
    }
    warehouse->add_finished_product(product_id);  //Add finished product to warehouse inventory
}


/**
 * @brief Set the number of assembly lines in the station
 * @param count The number of assembly lines
 */
void AssemblyStation::set_station_count(int count) {
    if (running) {
        return;
    }   //Prevents resizing during execution.
    station_count = std::max(1, count);
}
/*************************************************************************************/

