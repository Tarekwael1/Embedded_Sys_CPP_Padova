/**
 * @file Warehouse.cpp
 * @brief Warehouse implementation
 */

/*****************************Project Headers*****************************************/
#include "Warehouse.h"

/*************************************************************************************/

/*****************************Standard Libraries**************************************/
#include <iostream>

/*************************************************************************************/

/*****************************Warehouse Methods***************************************/
/**
 * @brief Constructor for Warehouse
 */
Warehouse::Warehouse() {
    // Initialize empty warehouse
}

//Without these comming two fnctions, Overbooking may occur in multithreaded environment
/**
 * @brief Check if required components are available in the warehouse
 * /Used to check if the warehouse can fulfill a BOM, without modifying inventory.
 * @param required Map of component_id to required quantity
 * @return true if all required components are available, false otherwise
 */
bool Warehouse::has_components(const std::map<std::string, int>& required) {
    std::lock_guard<std::mutex> lock(inventory_mutex);  // Lock inventory for check <----RC------------------
    
    for (const auto& req : required) {
        auto it = components.find(req.first);
        if (it == components.end() || it->second < req.second) { // Not enough quantity
            return false;
        }
    }
    return true;
}


/**
 * @brief Reserve required components atomically
 * @param required Map of component_id to required quantity
 * @return true if reservation is successful, false otherwise
 */
bool Warehouse::reserve_components(const std::map<std::string, int>& required) {
    std::lock_guard<std::mutex> lock(inventory_mutex);  // Lock inventory for atomic check and reserve //<-----------------
   
    // Check and deduction happen atomically
    // No other thread can modify inventory mid-operation
    // Check availability first
    //If any component is missing → NO reservation occurs
    for (const auto& req : required) {
        auto it = components.find(req.first);
        int available = (it != components.end()) ? it->second : 0;
        if (available < req.second) {
            log_component_availability(req.first, req.second, available);
            return false;
        }
    }
    
    // Reserve components
    for (const auto& req : required) {
        components[req.first] -= req.second; // Deduct reserved quantity
    }
    
    return true;
}


/**
 * @brief Add components to the warehouse inventory
 * @param component_id The ID of the component
 * @param quantity The quantity to add
 */
void Warehouse::add_component(const std::string& component_id, int quantity) {
    std::lock_guard<std::mutex> lock(inventory_mutex); //(Reservation) Lock inventory for update <----------------------------
    components[component_id] += quantity;
}


/**
 * @brief Get the quantity of a specific component in the warehouse
 * @param component_id The ID of the component
 * @return The quantity available
 */
int Warehouse::get_component_quantity(const std::string& component_id) const {
    std::lock_guard<std::mutex> lock(inventory_mutex); // Lock inventory for read <----------
    auto it = components.find(component_id);
    return (it != components.end()) ? it->second : 0;
}


/**
 * @brief Add a finished product to the warehouse inventory
 * @param product_id The ID of the finished product
 */
void Warehouse::add_finished_product(const std::string& product_id) {
    std::lock_guard<std::mutex> lock(inventory_mutex); // Lock inventory for update //<-----------------------------------------------------------------------------
    finished_products[product_id]++;
}



/**
 * @brief Get the count of a specific finished product in the warehouse
 * @param product_id The ID of the finished product
 * @return The count available
 */
int Warehouse::get_finished_product_count(const std::string& product_id) const {
    std::lock_guard<std::mutex> lock(inventory_mutex); // Lock inventory for read //<-----------------------------------------------------------------------------
    auto it = finished_products.find(product_id);
    return (it != finished_products.end()) ? it->second : 0;
}



/**
 * @brief Print the current inventory status of the warehouse
 */
void Warehouse::print_inventory() const {
    std::lock_guard<std::mutex> lock(inventory_mutex);   // Lock inventory for read //<-----------------------------------------------------------------------------

    std::cout << "\n=== Warehouse Inventory ===\n";
    std::cout << "Components:\n";
    for (const auto& comp : components) {
        std::cout << "  " << comp.first << ": " << comp.second << std::endl;
    }

    std::cout << "\nFinished Products:\n";
    for (const auto& prod : finished_products) {
        std::cout << "  " << prod.first << ": " << prod.second << std::endl;
    }
}

/**
 * @brief Log component availability for debugging
 * @param component_id The component to check
 * @param required_quantity The required quantity
 * @param available_quantity The available quantity
 */
void Warehouse::log_component_availability(const std::string& component_id, int required_quantity, int available_quantity) const {
    std::cout << "[Warehouse] Component " << component_id
              << " - Required: " << required_quantity
              << ", Available: " << available_quantity
              << (available_quantity >= required_quantity ? " (OK)" : " (INSUFFICIENT)") << std::endl;
}


/*************************************************************************************/