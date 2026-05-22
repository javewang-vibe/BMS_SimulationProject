#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "battery.h"
#include "state_machine.h"
#include "control.h"

int main()
{
    BmsData bms;

    srand((unsigned)time(NULL));

    BMS_Init(&bms);

    printf("==== BMS V1.0 START ====\n");

    while(1)
    {
        BMS_ReadSensor(&bms);

        BMS_CheckState(&bms);

        BMS_Control(&bms);

        BMS_Report(&bms);

        printf("---------------------\n");

        Sleep(1000);
    }

    return 0;
}