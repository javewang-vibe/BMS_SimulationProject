# BMS_SimulationProject
This project is a simplified Battery Management System (BMS) simulation developed in C language.  

## Current Features

- Simulated multi-cell voltage, pack current and temperature sampling
- SOC estimation based on coulomb counting
- BMS state machine for standby, charge, discharge and fault states
- Fault diagnosis for cell overvoltage, undervoltage, overtemperature, undertemperature, overcurrent and cell imbalance
- CSV logging for SOC, pack voltage, current, temperature, state and fault
- Python-based CSV visualization script

## Build

```powershell
gcc -Wall -Wextra -std=c11 -Iinc main.c src/battery.c src/control.c src/fault.c src/logger.c src/soc.c src/state_machine.c -o bms.exe
```
