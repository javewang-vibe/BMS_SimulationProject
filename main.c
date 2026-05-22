#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "battery.h"
#include "state_machine.h"
#include "control.h"
#include "logger.h"

int main()
{
    BmsData bms;

    srand((unsigned)time(NULL));

    BMS_Init(&bms);
    CSV_Init();

    printf("==== BMS V1.0 START ====\n");

    while(1)
    {
        BMS_ReadSensor(&bms);

        BMS_CheckState(&bms);

        BMS_Control(&bms);

        BMS_Report(&bms);

        CSV_Write
        (
            bms.soc,
            BMS_GetPackVoltage(&bms),
            bms.temperature,
            StateToStr(bms.state)
        );

        printf("---------------------\n");

        Sleep(1000);
    }
    CSV_Close();
    return 0;
}