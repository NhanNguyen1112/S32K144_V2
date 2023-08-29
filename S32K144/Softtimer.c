
#include "Softtimer.h"

static volatile unsigned int TimeCountMs;

static Softtimer TIMER[Total_Timer];

void Function_LedRed(void)
{
  Gpio_TogglePin(PortD,LEDRED);
}

void Function_LedGreen(void)
{
  Gpio_TogglePin(PortD,LEDGREEN);
}

void Function_LedBlue(void)
{
  Gpio_TogglePin(PortD,LEDBLUE);
}

void Softtimer_Init(void)
{
  TimeCountMs = 0;

  SYSTICK->SYST_RVR = (uint32_t)(4800000u); /* Set reload value 100ms*/

  SYSTICK->SYST_CVR = (uint32_t)(0u); /* Clear current value */

  SYSTICK->SYST_CSR |= (7u<<0); /* Enable counter & Set processor clock & enable INT */
}

/*
* TimerID: 0->Total Timer
* TimeOutValue: 1=100ms
* Mode: ONESHOT / CONTINUE
*/
void Softtimer_StartTimer(unsigned int TimerID,unsigned int TimeoutValue,unsigned char Mode,CALLBACK Event)
{
  TIMER[TimerID].TimerID = TimerID;
  TIMER[TimerID].Mode = Mode;
  TIMER[TimerID].TimeoutValue = TimeoutValue;
  TIMER[TimerID].Callback = Event;
  TIMER[TimerID].CurrentTime = TimeCountMs;
}

void Softtimer_StopTimer(unsigned int TimerID)
{
  TIMER[TimerID].TimeoutValue = 0;
}

void Softtimer_MainFunction(void)
{
  int i;
  for(i=0;i<=Total_Timer;i++)
  {
    if(TIMER[i].TimeoutValue != 0)
    {
      if ( (TimeCountMs - TIMER[i].CurrentTime) >= TIMER[i].TimeoutValue )
      {
        if(TIMER[i].Mode==CONTINUE) 
        {
          TIMER[i].Callback();
          TIMER[i].CurrentTime = TimeCountMs;
        } 
        else TIMER[i].Callback();
      }
    }
  }
}

void SysTick_Handler(void)
{
  TimeCountMs++;
}


