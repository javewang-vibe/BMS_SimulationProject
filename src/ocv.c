#include "ocv.h"


//18650 标准OCV-SOC对应表
static const float ocv_table[]=
{
    2.800f,  // SOC = 0%
    3.300f,  // SOC = 10%
    3.520f,  // SOC = 20%
    3.650f,  // SOC = 30%
    3.720f,  // SOC = 40%
    3.780f,  // SOC = 50%
    3.830f,  // SOC = 60%
    3.890f,  // SOC = 70%
    3.960f,  // SOC = 80%
    4.060f,  // SOC = 90%
    4.200f   // SOC = 100%
};

#define OCV_TABLE_SIZE 11

float OCV_LookupSOC(float ocv)
{
    if(ocv <= ocv_table[0])
        return 0.0f;
    if(ocv >= ocv_table[OCV_TABLE_SIZE - 1])
        return 100.0f;
    for(int i = 0; i < OCV_TABLE_SIZE - 1; i++)
    {
        if(ocv >= ocv_table[i] && ocv <= ocv_table[i+1])
        {
            float soc_low = i * 10.0f;
            float soc_high = (i+1) * 10.0f;
            float ratio = (ocv - ocv_table[i])/(ocv_table[i+1] - ocv_table[i]);
            return soc_low + ratio * 10.0f;
        }
    }
    return 0.0f;
}

float OCV_LookupSOC_Inverse(float soc)
{
    if(soc <= 0.0f) return ocv_table[0];
    if(soc >= 100.0f) return ocv_table[OCV_TABLE_SIZE - 1];
    int idx = (int)(soc / 10.0f);
    if(idx >= OCV_TABLE_SIZE - 1) idx = OCV_TABLE_SIZE - 2;
    float ratio = (soc - idx * 10.0f) / 10.0f;
    return ocv_table[idx] + ratio * (ocv_table[idx+1] - ocv_table[idx]);
}