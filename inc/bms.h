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

typedef struct
{
    float cellvoltage[CELL_NUM];
    float temperature;
    float current;
    float soc;
    BmsState state;
} BmsData;

#endif