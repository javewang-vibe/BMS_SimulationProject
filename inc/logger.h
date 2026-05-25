#ifndef LOGGER_H
#define LOGGER_H

void CSV_Init(void);
void CSV_Write(int time,float soc, float voltage, float temp, const char* state);
void CSV_Close(void);

#endif