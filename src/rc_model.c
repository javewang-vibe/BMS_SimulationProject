#include "rc_model.h"
#include <math.h>

void RcModel_Init(RcModel* rc)
{
    rc->v_rc = 0.0f;
}

float RcModel_Update(RcModel* rc,float current,float ocv)
{
    float dt = 1.0f;
    float tau = R1 * C1;
    rc->v_rc = rc->v_rc * expf(-dt / tau) + current * R1 * (1.0f - expf(-dt / tau));
    float v_terminal = ocv - current * R0 - rc->v_rc;
    if(v_terminal > 4.2f) v_terminal = 4.2f;
    if(v_terminal < 2.8f) v_terminal = 2.8f;
    return v_terminal;
}