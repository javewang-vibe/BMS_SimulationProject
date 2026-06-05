# BMS Simulation Project

A Battery Management System (BMS) simulation implemented in C, simulating real-world battery behavior including SOC estimation, fault detection, and state management.

## Project Structure
BMS_SimulationProject/
├── src/                  # Source files (.c)
│   ├── battery.c         # Battery sensor simulation (RC equivalent circuit)
│   ├── soc.c             # SOC estimation (Coulomb counting + OCV correction)
│   ├── state_machine.c   # State machine (STANDBY/CHARGE/DISCHARGE/FAULT)
│   ├── fault.c           # Fault detection and latch mechanism
│   ├── control.c         # MOS control output
│   ├── logger.c          # CSV data logger
│   ├── rc_model.c        # First-order RC equivalent circuit model
│   └── ocv.c             # OCV-SOC lookup table with interpolation
├── include/              # Header files (.h)
│   ├── bms.h             # Core data structures and enumerations
│   ├── battery.h
│   ├── soc.h
│   ├── state_machine.h
│   ├── fault.h
│   ├── control.h
│   ├── logger.h
│   ├── rc_model.h
│   └── ocv.h
├── main.c                  # Main loop
└── plot_bms_update.py      # Python visualization script
└── BMS_SimulationProject.m

## Features

- **RC Equivalent Circuit Model**: First-order RC model simulates real battery terminal voltage behavior, replacing random sensor data with physically meaningful   values
- **SOC Estimation**: Extended Kalman Filter (EKF) fusing Coulomb counting with OCV lookup table correction, with R0 and V_RC compensation
- **OCV-SOC Lookup Table**: 18650 standard OCV curve with linear interpolation (11 data points)
- **Fault Detection**: 7 fault types with real-time monitoring
  - Cell overvoltage / undervoltage
  - Overtemperature / undertemperature
  - Charge / discharge overcurrent
  - Cell imbalance
- **Fault Latch Mechanism**: Faults are latched and require condition-based auto reset with hysteresis and stability timer
- **State Machine**: 4 states (STANDBY / CHARGE / DISCHARGE / FAULT) with defined transition logic
- **CSV Data Logger**: Real-time logging with fflush for data integrity
- **Python Visualization**: 4-panel plot of SOC, voltage, current, and temperature with fault event markers
- **MATLAB Simulation**: EKF SOC estimation validation with true SOC comparisom, error analysis, and 6-panel visualization

## Simulation Results

![BMS Simulation](bms_simulation.png)
![BMS Simulation](bms_simulation_matlab.png)

## Requirements

### C Compilation
- GCC 16.1.0 (MSYS2)
- VSCode
### MATLAB Simulation
- MATLAB R2016a or later
- Run 'matlab/bms_simulation.m'

Compile:
```bash
gcc main.c src/*.c -Iinclude -lm -o bms_sim



ReadSensor         RC model generates physically correct terminal voltage
    ↓
UpdateSOC          Coulomb counting + OCV correction
    ↓
CheckState         Fault detection → State transition
    ↓
Control            MOS gate control based on current state
    ↓
Logger             Write data to CSV



