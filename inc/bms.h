#ifndef BMS_H
#define BMS_H

#include "rc_model.h"

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
} BatteryRaw;

typedef struct 
{
    RcModel rc[CELL_NUM];
} RcModelState;

typedef struct 
{
    float soc;
    float soc_cell[CELL_NUM];
} BatteryEstimation;


typedef struct 
{
    BmsState state;
    BmsFault fault;
    BmsFault fault_latch;
    int fault_reset_request;
    int fault_timer;    
} BatterySafety;

typedef struct
{
    BatteryRaw raw;
    RcModel rc[CELL_NUM];
    BatteryEstimation est;
    BatterySafety safety;
} BmsData;

#endif
