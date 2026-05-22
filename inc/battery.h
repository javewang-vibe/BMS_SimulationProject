#ifndef BATTERY_H
#define BATTERY_H

#include "bms.h"

void BMS_Init(BmsData* bms);

void BMS_ReadSensor(BmsData* bms);

float BMS_GetPackVoltage(BmsData* bms);

void BMS_GetCellMinMax(BmsData* bms,float* minV,float* maxV);

float BMS_GetDeltaVoltage(BmsData* bms);

#endif