#include <stdio.h>
#include "state_machine.h"
#include "battery.h"
#include "fault.h"

void BMS_CheckState(BmsData* bms)
{
    BMS_DiagnoseFault(bms);

    if(bms->safety.fault != BMS_FAULT_NONE)
    {
        if(bms->safety.fault_latch == BMS_FAULT_NONE)
        {
            bms->safety.fault_latch = bms->safety.fault;
            bms->safety.fault_timer = 0;
        } 
        bms->safety.state = BMS_FAULT;
        printf("[FAULT] %s\n", FaultToStr(bms->safety.fault));
        return;
    }
    if(bms->safety.state == BMS_FAULT)
    {
        BMS_TryAutoReset(bms);
        if(bms->safety.fault_reset_request ==1)
        {
            bms->safety.fault_latch = BMS_FAULT_NONE;
            bms->safety.fault_reset_request = 0;
            bms->safety.fault_timer = 0;
            bms->safety.state = BMS_STANDBY;
            printf("[RECOVERY] System reset, back to STANDBY\n");
        }
        return;
    }

    switch(bms->safety.state)
    {
        case BMS_STANDBY:
            if(bms->raw.current < -0.05f)
                bms->safety.state = BMS_CHARGE;
            else if(bms->raw.current > 0.05f)
                bms->safety.state = BMS_DISCHARGE;
            break;
        case BMS_CHARGE:
            if(bms->raw.current >= -0.05f)
                bms->safety.state = BMS_STANDBY;
            break;
        case BMS_DISCHARGE:
            if(bms->raw.current <= 0.05f)
                bms->safety.state = BMS_STANDBY;
            break;
        case BMS_FAULT:
            bms->safety.state = BMS_STANDBY;
            break;
        default:
            break;
    }
}

const char* StateToStr(BmsState state)
{
    switch(state)
    {
        case BMS_STANDBY:
            return "STANDBY";

        case BMS_CHARGE:
            return "CHARGE";

        case BMS_DISCHARGE:
            return "DISCHARGE";

        case BMS_FAULT:
            return "FAULT";
    }

    return "UNKNOWN";
}
