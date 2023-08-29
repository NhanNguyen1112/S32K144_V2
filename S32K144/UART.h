
#ifndef __UART__
#define __UART__

#include "S32K144.h"
#include "UART_Register.h"

#define UART_BUFFER_SIZE (20u)

typedef enum UartState
{
    UNINIT,
    IDLE,
    BUSY,
    EMPTY,
    NOT_EMP
}UartState_enum;

typedef enum TransmitState
{
    ACCEPT,
    NOT_ACCEPT
}TransmitState_enum;

extern unsigned char Software_Buffer_Read[UART_BUFFER_SIZE];

void Uart_Init ( void );

unsigned char Uart_Transmit (unsigned char* u8_TxBuffer, unsigned int u4_Length );
unsigned char Uart_GetTransmitState ( void );
void Uart_TxMainFunction ( void );

void Uart_GetReceiveData ( unsigned char* u8_RxBuffer );
unsigned char Uart_GetReceiveState ( void );
void Uart_RxMainFunction ( void );

void LPUART1_RxTx_IRQHandler ( void ); 

void UART_MAIN_TEST(void);

#endif


