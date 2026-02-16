/**
 * @file AssemblyStation.h
 * @brief Assembly station that processes orders
 */

#ifndef ASSEMBLY_STATION_H
#define ASSEMBLY_STATION_H
/******************************Project Headers*****************************************/
#include "Order.h"
#include "Product.h"
#include "Warehouse.h"

/*************************************************************************************/

class AGV;
class ControlCenter;
/****************************Forward Declarations*************************************/
#include <map>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <cstdint>
/*************************************************************************************/

/****************************AssemblyStation Class Definition*************************/
/**
 * @class AssemblyStation
 * @brief Represents an assembly station that processes orders
 */
class AssemblyStation {
private:
    struct ReadySlot {
        Order order;
        int estimated_time;
        uint64_t sequence;
    };
    struct ReadySlotCompare {
        bool operator()(const ReadySlot& a, const ReadySlot& b) const {
            if (a.estimated_time != b.estimated_time) {     
                return a.estimated_time > b.estimated_time;
            }
            return a.sequence > b.sequence;
        }
    };

    Warehouse* warehouse;
    std::vector<AGV*>* agv_fleet;
    ControlCenter* control_center;
    std::map<std::string, Product>* products;
    std::queue<Order> order_queue;
    std::priority_queue<ReadySlot, std::vector<ReadySlot>, ReadySlotCompare> ready_queue;
    mutable std::mutex queue_mutex;   // Mutex for order queue
    mutable std::mutex ready_mutex;   // Mutex for ready queue
    std::condition_variable order_cv; // Notify staging thread of new orders
    std::condition_variable ready_cv; // Notify station threads of new ready orders
    std::vector<std::thread> station_threads; 
    std::thread staging_thread;
    std::atomic<bool> running;  // Control flag for threads, which cannot changed during simulation
    std::atomic<int> current_sim_time_minutes; // Current simulation time in minutes 

    // Configuration
    int setup_time_minutes;
    int station_count;

    void process_orders(int line_id);
    void staging_loop();
    bool request_components(const Order& order);
    int calculate_operation_time(const std::string& product_id);
    int get_base_time(const std::string& product_id) const;

    // Delivery coordination
    std::mutex delivery_mutex;
    std::map<int, std::map<std::string, int>> pending_deliveries;
    std::mutex staging_mutex;
    std::map<int, Order> staging_orders;

    // Retry control
    std::map<int, int> retry_counts;
    const int max_request_retries = 100;

    // Multi-line timing helpers
    mutable std::mutex timing_mutex;
    std::vector<int> station_virtual_time_minutes;
    std::vector<std::string> last_product_processed;
    std::atomic<uint64_t> ready_sequence;

    // Statistics
    std::atomic<int> total_busy_time_minutes;
    std::atomic<int> orders_completed;

public:
    AssemblyStation(Warehouse* wh, std::vector<AGV*>* fleet);
    ~AssemblyStation();

    void start();
    void stop();
    void add_order(const Order& order);
    void set_station_count(int count);
    void set_simulation_time(int minutes);
    void set_products(std::map<std::string, Product>* prods) { products = prods; }
    void set_control_center(ControlCenter* cc) { control_center = cc; }

    void notify_component_delivered(int order_id, const std::string& component_id, int quantity);
    void notify_finished_product_delivered(const std::string& product_id);

    int get_total_busy_time() const { return total_busy_time_minutes.load(); }
    int get_orders_completed() const { return orders_completed.load(); }
    bool is_processing() const;
};
/*************************************************************************************/
#endif /* ASSEMBLY_STATION_H */

