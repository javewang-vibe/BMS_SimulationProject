#ifndef BMS_H
#define BMS_H

#define CELL_NUM 6

typedef enum
{
    BMS_STANDBY,
    BMS_CHARGE,
    BMS_DISCHARGE,
    BMS_FAULT
} BmsState;

typedef enum
{
    BMS_FAULT_NONE = 0,
    BMS_FAULT_CELL_OVERVOLTAGE,
    BMS_FAULT_CELL_UNDERVOLTAGE,
    BMS_FAULT_OVERTEMPERATURE,
    BMS_FAULT_UNDERTEMPERATURE,
    BMS_FAULT_CHARGE_OVERCURRENT,
    BMS_FAULT_DISCHARGE_OVERCURRENT,
    BMS_FAULT_CELL_IMBALANCE
} BmsFault;

typedef struct
{
    float cellvoltage[CELL_NUM];
    float temperature;
    float current;
    float soc;
    BmsState state;
    BmsFault fault;
} BmsData;

#endif
