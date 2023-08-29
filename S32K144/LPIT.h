
#ifndef _LPIT_
#define _LPIT_

#include "S32K144.h"
#include "LPIT_Register.h"

void MainLPIT_Test(void);

void LPIT_Init_CH0(unsigned int TimerVALUE);
void LPIT_CH0_Delay_ms(const unsigned int MiliSecond);

#endif

