#include <stdio.h>
#include "control.h"
#include "battery.h"
#include "state_machine.h"
#include "fault.h"

void BMS_Control(BmsData* bms)
{
    switch(bms->safety.state)
    {
        case BMS_STANDBY:
            printf("[CTRL] MOS: IDLE\n");
            break;

        case BMS_CHARGE:
            printf("[CTRL] MOS: CHARGING ENABLED\n");
            break;

        case BMS_DISCHARGE:
            printf("[CTRL] MOS: DISCHARGING ENABLED\n");
            break;

        case BMS_FAULT:
            printf("[CTRL] MOS: SHUTDOWN!!!\n");
            break;
    }
}

void BMS_Report(BmsData* bms)
{
    float minV,maxV;

    BMS_GetCellMinMax(bms,&minV,&maxV);

    printf("[REPORT] State=%s | Fault=%s | PackV=%.2fV | MinV=%.2fV | MaxV=%.2fV | Delta=%.3fV | I=%.2fA | T=%.1fC | SOC=%.6f%%\n",
           StateToStr(bms->safety.state),
           FaultToStr(bms->safety.fault),
           BMS_GetPackVoltage(bms),
           minV,
           maxV,
           maxV - minV,
           bms->raw.current,
           bms->raw.temperature,
           bms->est.soc_ekf);
}
