#include "soc.h"

void BMS_UpdateSOC(BmsData* bms)
{
    float capacity = 100.0f;
    float dt = 1.0f / 3600.0f;
    float delta_soc;
    delta_soc = (bms->current/capacity)*dt*100.0f;
    bms->soc -= delta_soc;
    if(bms->soc > 100.0f)
    {
        bms->soc = 100.0f;
    }
    if(bms->soc < 0.0f)
    {
        bms->soc = 0.0f;
    }
}
