Flexible Assembly System � Concurrency Scenarios
=================================================

This note highlights every concurrency hotspot, the primitive we use (thread, mutex, monitor-style wait/notify via condition variables), the potential hazards (deadlock, starvation, race, interleaving), and why the chosen mechanism mitigates the issue.

## 1. Control Center Scheduler vs. Assembly Station (Order Flow)

- Primitive: Separate threads (`std::thread`) plus `std::mutex`/`std::condition_variable` guarding `AssemblyStation::order_queue` and the ready priority queue shared by multiple workers.
- Hazard: Race condition or interleaving if the scheduler pushed while the staging thread popped without synchronization; duplicated or lost orders.
- Mitigation: Queue protected by `queue_mutex`; producer (scheduler) notifies `order_cv` so the staging thread wakes exactly when needed. Downstream, the ready priority queue is also mutex-protected, and `ready_cv.notify_all()` ensures every worker competes fairly for finished kits.

## 2. Component Delivery Coordination (Assembly Station → AGVs)

- Primitive: `delivery_mutex` protects `pending_deliveries`; once counts reach zero, the staging thread pushes a `ReadySlot` and broadcasts `ready_cv` instead of blocking on a CV itself.
- Hazards:
  * Race condition: multiple AGVs decrement totals concurrently.
  * Stall: if no worker woke when kits finished, the ready queue could back up.
- Mitigation: Every `notify_component_delivered` acquires `delivery_mutex`, updates counts safely, clamps at zero, and when all items are satisfied pushes to the ready queue before broadcasting `ready_cv`. This guarantees at least one worker wakes immediately without needing a busy waiter.

## 3. AGV Task Assignment and State Machine

- Primitive: One thread per AGV; task ownership guarded by `state_mutex` and signaled with `task_cv`. `assign_task` now returns `bool` so callers know whether an AGV actually accepted the job.
- Hazards:
  * Race/interleaving: Two assignments could overwrite each other; AGV might read half-updated data and drive incorrectly.
  * Lost dispatch: If the AGV became busy between `is_idle` check and the write, the task used to vanish silently.
  * Deadlock: If callbacks held `state_mutex` while calling the station, circular waits could arise.
- Mitigation: `assign_task` writes the task under lock and reports success; callers retry with exponential backoff until an AGV truly accepts. The AGV thread copies the task, releases the mutex during travel segments, and reacquires it only when clearing state. Callbacks drop `state_mutex` before notifying the station, preserving lock ordering.

## 4. Warehouse Inventory Access

- Primitive: Single `inventory_mutex` guarding `components` and `finished_products` maps. All threads (staging, AGVs, workers) access through `Warehouse` methods.
- Hazards:
  * Race condition or lost update if simultaneous reservations and replenishments happened without locking.
- Mitigation: All inventory methods lock `inventory_mutex`, keep critical sections short, and never grab another mutex inside, eliminating deadlock risk.

## 5. Completion Tracking and Shutdown

- Primitive: `completed_orders` atomic plus `completion_mutex`/`completion_cv` used as a monitor. Threads signal when orders finish.
- Hazards:
  * Premature exit: main thread might terminate while workers still log or update KPIs.
- Mitigation: Each completion/cancellation increments the atomic counter and signals the CV. The wait predicate checks `completed_orders == total_orders`, so shutdown only begins once every worker has reported.

## 6. Logging Subsystem

- Primitive: `log_mutex` serializes writes to `sim_log.txt`; all threads share the logger.
- Hazards:
  * Output interleaving would corrupt the log file and make diagnostics useless.
- Mitigation: Messages are formatted first, then written while holding `log_mutex` for the shortest possible time.

## 7. Retry/Backoff Strategies

- Primitive: Timed retries (sleep) manage AGV assignment and component reservation.
- Hazards:
  * Starvation or livelock if resources never free up.
- Mitigation: Retries are bounded (50 attempts for AGV dispatch, 100 for component requests). After exhausting retries we either requeue or cancel the order, ensuring logs record the reason and the queue never spins forever.

## Thread Topology Snapshot

- Scheduler thread (Control Center)
- Staging thread (Assembly Station)
- `station_count` assembly worker threads pulling from the ready queue
- 10 AGV threads handling motion/state machines
- Main thread orchestrating startup/shutdown

Why These Primitives?
---------------------

- Threads provide the required concurrency between functional blocks (control center, station, AGVs) as mandated by the assignment.
- Mutexes and condition variables (monitors) give fine-grained, low-overhead coordination for shared data structures without involving heavier constructs like semaphores or message brokers.
- Semaphores were unnecessary because resource counts are managed explicitly in maps; a reservation call either succeeds or fails. If requirements grew to multiple identical consumers waiting on a resource pool, semaphores could be revisited.

Summary of Risk Mitigation
--------------------------

- Race Conditions: Eliminated via mutexes around every shared data structure (orders, deliveries, inventory, logs).
- Deadlocks: Prevented by never holding more than one mutex at a time (AGV callbacks release their lock before notifying others) and limiting lock scope.
- Starvation: Bounded retries and condition-variable waits ensure threads either progress or bail out cleanly.
- Interleaving: Logging and state transitions occur inside short critical sections, giving deterministic order of events in artifacts like `sim_log.txt`.

This file complements `docs/Concurrency_Overview.txt` by focusing specifically on where concurrency exists, which primitives guard each hotspot, and which failure modes they prevent.
