#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "bms.h"

void BMS_CheckState(BmsData* bms);

const char* StateToStr(BmsState state);

#endif