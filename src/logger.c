#include <stdio.h>
#include "logger.h"

FILE *fp;

void CSV_Init()
{
    fp = fopen("bms_log.csv","w");
    if(fp == NULL)
    {
        printf("Failed to open CSV file!\n");
        return;
    }
    fprintf(fp,"soc,voltage,temp,state\n");
}

void CSV_Write(float soc, float voltage, float temp, const char* state)
{
    if(fp == NULL) return;
    fprintf(fp,"%.2f,%.2f,%.2f,%s\n",soc,voltage,temp,state);
    fflush(fp); 
}

void CSV_Close()
{
    if(fp != NULL)
    {
        fclose(fp);
        fp = NULL;
    }
}