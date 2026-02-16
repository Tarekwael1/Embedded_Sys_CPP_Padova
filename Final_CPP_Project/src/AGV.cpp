/**
 * @file AGV.cpp
 * @brief AGV implementation with state machine
 */

/******************************Project Headers*****************************************/
#include "AGV.h"
#include "AssemblyStation.h"
#include <iostream>
#include <chrono>
#include <thread>
/*************************************************************************************/

/****************************AGV Methods**********************************************/
/**
 * @brief Constructor for AGV
 * @param id Unique identifier for the AGV
 */
AGV::AGV(int id) 
    : agv_id(id), 
      state(AGVState::IDLE), 
      running(false),
      travel_time_warehouse_minutes(2),
      travel_time_station_minutes(3),
      picking_time_minutes(1),
      dropping_time_minutes(1),
      return_time_minutes(2),
      total_operations(0),
      busy_time_minutes(0) {
}

/**
 * @brief Destructor for AGV
 */
AGV::~AGV() {
    stop();
    if (agv_thread.joinable()) { //stop without interrupt
        agv_thread.join();
    }
}


/**
 * @brief Start the AGV thread
 */
void AGV::start() {
    running = true;
    agv_thread = std::thread(&AGV::run, this);  //Start AGV thread 
}

/**
 * @brief Stop the AGV thread
 */
void AGV::stop() {
    running = false;
    task_cv.notify_all(); //Signal to stop AGV //<-------DL--
}


/**
 * @brief Main AGV state machine loop
 */
void AGV::run() {
    while (running) {
        std::unique_lock<std::mutex> lock(state_mutex);   //mustex wait for task <------------
        
        // Wait for task assignment or stop signal
        task_cv.wait(lock, [this] { //<--------(polling) (new task)---------
            return !running || !current_task.component_id.empty(); 
        });
        
        if (!running) break;
        
        if (current_task.component_id.empty()) {
            continue;
        }

        AGVTask task = current_task; // copy so we can release mutex during travel
        lock.unlock(); //<-----------------------------------------------------------------------------

        int busy_increment = 0;

        if (!task.is_finished_product) {
            run_segment(AGVState::TO_WAREHOUSE, travel_time_warehouse_minutes);
            busy_increment += travel_time_warehouse_minutes;

            run_segment(AGVState::PICKING, picking_time_minutes);
            busy_increment += picking_time_minutes;

            run_segment(AGVState::TO_STATION, travel_time_station_minutes);
            busy_increment += travel_time_station_minutes;

            run_segment(AGVState::DROPPING, dropping_time_minutes);
            busy_increment += dropping_time_minutes;

            if (task.notify_station && task.destination == std::string("ASSEMBLY_STATION")) {
                task.notify_station->notify_component_delivered(task.order_id, task.component_id, task.quantity); //<-------
            }
        } else {
            run_segment(AGVState::TO_STATION, travel_time_station_minutes);
            busy_increment += travel_time_station_minutes;

            run_segment(AGVState::PICKING, picking_time_minutes);
            busy_increment += picking_time_minutes;

            run_segment(AGVState::TO_WAREHOUSE, travel_time_warehouse_minutes);
            busy_increment += travel_time_warehouse_minutes;

            run_segment(AGVState::DROPPING, dropping_time_minutes);
            busy_increment += dropping_time_minutes;

            if (task.notify_station) {
                task.notify_station->notify_finished_product_delivered(task.component_id);
            }
        }

        run_segment(AGVState::RETURNING, return_time_minutes);
        busy_increment += return_time_minutes;

        busy_time_minutes.fetch_add(busy_increment, std::memory_order_relaxed);
        total_operations.fetch_add(1, std::memory_order_relaxed);

        lock.lock(); //<---RC------state, and current task update (shared data)
        current_task = AGVTask();
        transition_to(AGVState::IDLE);
        lock.unlock();  //<------
    }
}

void AGV::run_segment(AGVState new_state, int minutes) {
    {
        std::lock_guard<std::mutex> lock(state_mutex); //<-------------state change
        transition_to(new_state);
    }
    if (minutes > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(minutes * 100));
    }
}

/**
 * @brief Transition AGV to a new state
 * @param new_state The new state to transition to
 */
void AGV::transition_to(AGVState new_state) {
    state = new_state;
}



/**
 * @brief Assign a new task to the AGV
 * @param component_id ID of the component to pick
 * @param quantity Quantity to pick (per task, should be 1)
 * @param destination Destination ("ASSEMBLY_STATION" or "WAREHOUSE")
 * @param notify_station Optional station to notify upon delivery
 */
bool AGV::assign_task(const std::string& component_id, int quantity,
                      const std::string& destination,
                      AssemblyStation* notify_station,
                      bool is_finished_product,
                      int order_id) {
    std::lock_guard<std::mutex> lock(state_mutex);  //wait assign task (current_task, and state are shared data)
    if (state == AGVState::IDLE && current_task.component_id.empty()) {
        current_task.component_id = component_id;
        current_task.quantity = quantity;
        current_task.destination = destination;
        current_task.is_complete = false;
        current_task.notify_station = notify_station;
        current_task.is_finished_product = is_finished_product;
        current_task.order_id = order_id;
        task_cv.notify_one();   // Signal new task  //<--Reduce the remaining time---------N---
        return true;
    }
    return false;
}


 /**
 * @brief Check if AGV is idle
 * @return true if AGV is idle, false otherwise
 */
bool AGV::is_idle() const {
    std::lock_guard<std::mutex> lock(state_mutex);  //mustex wait check idle  //<-----------------------------------------------------------------------------
    return state == AGVState::IDLE && current_task.component_id.empty();
}


/**
 * @brief Get the current state of the AGV
 * @return Current AGV state
 */
AGVState AGV::get_state() const {
    std::lock_guard<std::mutex> lock(state_mutex);  //mustex wait get state  //<-----------------------------------------------------------------------------
    return state;
}


/**
 * @brief Get the current task assigned to the AGV
 * @return Current AGV task
 */
AGVTask AGV::get_current_task() const {
    std::lock_guard<std::mutex> lock(state_mutex);  //mustex wait get task //<-----------------------------------------------------------------------------
    return current_task; 
}

/*************************************************************************************/
