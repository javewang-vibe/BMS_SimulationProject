#include <stdio.h>
#include <stdlib.h>
#include "battery.h"

void BMS_Init(BmsData* bms)
{
    for(int i = 0; i < CELL_NUM; i++)
    {
        bms->cellvoltage[i] = 3.7;
    }

    bms->temperature = 25.0;
    bms->current = 0.0;
    bms->soc = 50.0;
    bms->state = BMS_STANDBY;
    bms->fault = BMS_FAULT_NONE;
    bms->fault_latch = BMS_FAULT_NONE;
    bms->fault_reset_request = 0;
    bms->fault_timer = 0;
}

void BMS_ReadSensor(BmsData* bms)
{
    for(int i = 0; i < CELL_NUM; i++)
    {
        bms->cellvoltage[i] += ((rand() % 101) - 50) * 0.001;

        if(bms->cellvoltage[i] > 4.2)
            bms->cellvoltage[i] = 4.2;

        if(bms->cellvoltage[i] < 2.8)
            bms->cellvoltage[i] = 2.8;
    }

    bms->temperature += ((rand() % 101) - 50) * 0.05;

    if(bms->temperature > 70)
        bms->temperature = 70;

    if(bms->temperature < -10)
        bms->temperature = -10;

    bms->current = (((rand() % 201)) - 100) * 0.01;
}

float BMS_GetPackVoltage(BmsData* bms)
{
    float sum = 0;

    for(int i = 0; i < CELL_NUM; i++)
    {
        sum += bms->cellvoltage[i];
    }

    return sum;
}

void BMS_GetCellMinMax(BmsData* bms,float* minV,float* maxV)
{
    *minV = bms->cellvoltage[0];
    *maxV = bms->cellvoltage[0];

    for(int i = 0; i < CELL_NUM; i++)
    {
        if(bms->cellvoltage[i] < *minV)
            *minV = bms->cellvoltage[i];

        if(bms->cellvoltage[i] > *maxV)
            *maxV = bms->cellvoltage[i];
    }
}

float BMS_GetDeltaVoltage(BmsData* bms)
{
    float minV,maxV;

    BMS_GetCellMinMax(bms,&minV,&maxV);

    return maxV - minV;
}
