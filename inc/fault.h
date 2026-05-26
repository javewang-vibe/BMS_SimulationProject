#ifndef FAULT_H
#define FAULT_H

#include "bms.h"

void BMS_DiagnoseFault(BmsData* bms);
void BMS_TryAutoReset(BmsData* bms);
const char* FaultToStr(BmsFault fault);

#endif
