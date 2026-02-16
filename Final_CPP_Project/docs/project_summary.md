# Flexible Assembly System Simulation Summary

## High-Level Flow

1. `src/main.cpp` boots the simulator, ensures the `output` directory exists, and prints a banner.
2. `ControlCenter` loads orders, the BOM, and warehouse inventory from the `input/` files.
3. A `Warehouse` instance, a `ControlCenter`, and an `AssemblyStation` share a fleet of `AGV` objects that
   get allocated after loading succeeds.
4. `ControlCenter::start_simulation` wires the station/AGVs, starts their threads, and begins releasing orders
   according to the configured scheduling policy (`FIFO` by default).
5. `ControlCenter::wait_until_all_orders_complete` blocks `main` until every released order fully processes.
6. `ControlCenter::stop_simulation` signals the station and every AGV to shut down, after which `main` deletes the AGV objects and exits.

## Component & Function Purposes

- `ControlCenter`
  - `load_orders`, `load_bom`, `load_warehouse`: parse input files, validate data, and populate internal queues and the warehouse; failures abort early to avoid inconsistent runs.
  - `set_scheduling_policy`: switches queue discipline (currently `FIFO`; extendable for priority or round-robin later).
  - `start_simulation`: spawns the assembly station and AGV worker threads, then starts releasing orders.
  - `wait_until_all_orders_complete`: blocks until internal tracking shows no unfinished orders.
  - `stop_simulation`: broadcasts stop signals so the station and AGVs exit cleanly.
  - Logging and KPI helpers write chronological traces to `output/sim_log.txt` and summarized metrics to `output/kpi_report.txt`.
- `AssemblyStation`
  - Coordinates component staging, drives the ready-order priority queue (shortest base assembly time first), dispatches AGVs both for kitting and for finished-product returns, and now tracks per-line virtual clocks so multiple assembly lines can run concurrently when enabled.
  - Even when configured for a single physical line, the new dispatcher logic prevents dropped kits by confirming every AGV assignment and retrying when a vehicle accepts work late.
- `Warehouse`
  - Tracks on-hand inventory from `input/warehouse.txt`, exposes reservation/pick APIs, and prevents negative stock while AGVs request components.
- `AGV`
  - Represents each autonomous guided vehicle. The thread loop (`run`) waits for assignments, travels to warehouse slots, picks components, delivers them to the assembly station, and reports timing/status back to the control center.
  - `assign_task` now returns a success flag so callers can immediately retry if a race occurs between an `is_idle` check and the actual assignment.
- `FileHandler`, `Order`, `Product`
  - Provide parsing utilities plus lightweight data models so higher-level modules operate on structured orders and BOM entries instead of raw text.

## Output Files

- `output/sim_log.txt`: chronological log of every major event (order releases, AGV pickups/deliveries, warehouse shortages,
  assembly completions, errors). Use it for debugging sequence issues or timing anomalies.
- `output/kpi_report.txt`: aggregated performance data such as total completed orders, average cycle times, AGV utilization,
  and detected shortages. Review to evaluate whether configuration changes improved throughput.

## Configuration Notes

- `NUM_AGVS` is currently `10`, satisfying the requirement to have at least ten AGVs. Leave it at `10` for grading runs;
  temporarily lowering it (e.g., to `2`) can help with debugging but should be reverted afterward.
- `NUM_ASSEMBLY_LINES` defaults to `1` so the baseline run models a single physical station. Increase this constant if you want to experiment with parallel assembly threads�the station will automatically balance work across them.
- File location constants (`ORDERS_FILE`, `BOM_FILE`, `WAREHOUSE_FILE`, `LOG_FILE`, `KPI_REPORT_FILE`) already point to
  the expected `input/` and `output/` directories. No numeric adjustments are needed unless you relocate data files.

## Next Steps

1. Run the simulation once to generate fresh `sim_log.txt` and `kpi_report.txt`.
2. Inspect KPIs to decide whether you need a different scheduling policy or AGV count.
3. Extend the summary if new modules or policies are added later.
