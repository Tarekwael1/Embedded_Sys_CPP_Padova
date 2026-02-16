Flexible Assembly System (FAS) Simulator
========================================

## 1. **High-Level Sequence**

---

- **Startup:** `main.cpp` instantiates `Warehouse`, `ControlCenter`, `AssemblyStation`, and a pool of 10 `AGV` objects.
  Input files are parsed (orders, BOM, warehouse inventory) before any thread starts, ensuring deterministic initial state.
- **Thread Wiring:** `ControlCenter::start_simulation` injects shared references into the assembly station, starts every AGV thread,
  and finally launches the assembly-station staging thread plus *N* assembly-line workers (configurable; defaults to one) alongside the scheduler. At this point we have:

  * 1 scheduler thread advancing simulated time and releasing orders.
  * 1 staging thread that requests components, populates the ready queue, and wakes all workers via `ready_cv.notify_all()` when kits finish.
  * `station_count` assembly workers, each pulling from a priority-ready queue so short jobs can run first while longer kits finish staging.
  * N AGV threads, each blocking on its own condition variable until work arrives.
- **Order Lifecycle**: the scheduler pushes an `Order` into the assembly-station queue via `AssemblyStation::add_order`.
  Staging reserves components, dispatches AGVs to fetch them, and once all deliveries arrive, pushes a ticket on the ready priority queue. The fastest assembly worker wakes up, runs the operation (tracking per-line virtual time for setup/processing), and dispatches a finished-product trip. Completion/cancellation events go back to the control center so KPIs are updated.
- Shutdown: when all orders finish or cancel, `control_center.stop_simulation()` joins the scheduler, stops the staging thread and every assembly worker, and signals AGVs to exit after they return to `IDLE`. KPIs and logs are flushed once every worker confirms idle state.

## 2. **Synchronization Choices**

### 2.1 **Order Queue Guarded by `std::mutex`**

- `AssemblyStation::order_queue` is accessed by the scheduler thread (**producer**) and the staging thread (**consumer**).
  A simple mutex + condition variable suffices because only one consumer exists, and push/pop must be atomic to avoid races. We do not spin up additional threads here, because the concurrency requirement is satisfied by dedicating one thread to assembly logic while keeping order sequencing deterministic.
- Potential Problem: Without the mutex, the queue could be corrupted or two threads could remove the same order, leading to lost work or duplicated assembly. The chosen mutex ensures each order is processed exactly once.

### 2.2 **Delivery Tracking with `delivery_mutex` + `delivery_cv`**

- Component deliveries from multiple AGVs converge on the station. We track remaining quantities in `pending_deliveries` (one entry per component). Updates arrive from many AGV threads, so we guard the map with `delivery_mutex` to prevent lost updates.
- `wait_for_components` is replaced with a push-based model: once counts reach zero we place the order into the ready queue and broadcast `ready_cv` so any idle worker can pick it up immediately. This avoids a single consumer thread and scales to multiple lines.
- Potential Problems and Mitigations:

  * Missing signals could deadlock the station. Every `notify_component_delivered` call re-checks the map and broadcasts when all counts hit zero.
  * Race conditions could decrement below zero if multiple deliveries fired simultaneously; guarding the map prevents this and we clamp to zero when subtracting.

### 2.3 **AGV State Machine and `state_mutex`**

- Each AGV owns its state and current task. Assignments arrive from the staging thread whenever a component or finished-product trip is needed.
  We could have used a message queue per AGV, but the simpler approach is to guard the `AGVTask` struct with `state_mutex` and signal `task_cv` when a task arrives.
  This avoids launching yet another coordination thread per AGV.
- Returning State: the new `run_segment` helper transitions through `{TO_WAREHOUSE, PICKING, TO_STATION, DROPPING, RETURNING}`    and sleeps for the specified travel/pick/drop durations. This models the requirement explicitly and keeps busy-time accounting accurate.
- Potential Problems: Without the mutex, simultaneous assignments might overwrite each other and AGVs could wake up with partially written tasks.
  Without the explicit `RETURNING` state, utilization metrics would under-report real duty cycles and finished products would never be logged.

### 2.4 **Warehouse Inventory Mutex**

- `Warehouse::reserve_components`, `add_component`, and `add_finished_product` all use the same `inventory_mutex`,
  ensuring that reservations and updates remain consistent even while multiple AGVs interact with the warehouse simultaneously.
- Potential Problem: If one AGV reserved components while another tried to add finished goods without locking,
  the component map could be corrupted or counts could go negative. The shared mutex serializes these operations, preventing race-induced inventory drift.

### 2.5 **Completion Coordination (`completion_mutex` + `completion_cv`)**

- The control center exposes `wait_until_all_orders_complete`. This method needs to unblock only when every order completes or cancels. Rather than starting extra polling threads, we use a condition variable so the main thread sleeps efficiently while the assembly station and scheduler run. Each completion increments an atomic counter and notifies the CV.
- Potential Problem: Without this coordination, `main` may exit before all threads complete, leading to truncated logs or partially written KPI files. Waiting on the condition variable guarantees tidy shutdown.

## 3. **Contention Points and Resolutions**

- AGV Assignment Contention: When components are scarce, multiple AGVs may compete for tasks. The assembly station retries up to 50 times, sleeping briefly between attempts to reduce contention. This *prevents starvation *while keeping the implementation simple.
- Component Shortage Handling: `request_components` checks availability up front and uses `retry_counts` to avoid infinite loops. If shortages persist beyond 100 retries, the order is canceled and logged. Without this guard, the queue could livelock.
- Logging Contention: `ControlCenter::log_event` wraps file output with `log_mutex` so overlapping messages do not interleave. We avoided per-thread log buffers to keep the design compact while ensuring readable diagnostics.

## 4. **Thread Topology Summary**

- Control Center Scheduler Thread: releases orders, maintains simulation time, and handles completion tracking.
- Assembly Station Staging Thread: consumes the order queue, reserves components, and pushes ready jobs into the priority queue.
- Assembly Station Worker Threads (`station_count`): each pops ready kits, runs assembly, and dispatches completed products.
- AGV Threads (10): each simulates independent motion, blocking on `task_cv` when idle.
- Main Thread: orchestrates initialization, waits on `wait_until_all_orders_complete`, then performs orderly shutdown.

## 5. **Why Mutexes Instead of More Threads?**

- The assignment emphasizes concurrency among functional blocks (control center, assembly station, AGVs).
  Mutexes provide the necessary safety when these blocks share queues or maps without forcing us to spawn additional micro-threads for every shared structure.
- Mutex + condition variable patterns keep CPU usage low (no busy-waiting) and guarantee deterministic ownership of shared data structures.
  Creating new threads just to guard a queue would increase complexity and scheduling overhead without adding practical concurrency.

## 6. **Failure Modes Addressed**

- **Race Conditions:** Guarded by mutexes in the order queue, delivery map, warehouse inventory, and log file.
- **Deadlocks:** Avoided by keeping lock scopes small and never acquiring multiple locks in different orders
  (e.g., AGV notifications release `state_mutex` before calling back into `AssemblyStation`).
- Starvation: Retries + backoff when assigning AGVs or requesting components ensure eventual progress or clear cancellation.
- **Premature Shutdown:** `wait_until_all_orders_complete` blocks until `completed_orders == total_orders`, ensuring KPIs and logs are complete.

This document serves as a reference for the project�s concurrency design, detailing why each synchronization primitive was chosen and how it prevents the specific hazards inherent in a multi-threaded flexible assembly simulation.
