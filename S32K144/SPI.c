
#include "SPI.h"

static void SPI0_GPIO_Config(void);
void LPSPI_init(void);

unsigned char Data = 123;

static void SPI0_GPIO_Config(void)
{
	EnableClockPortB(Enable);

	PORTB->PCR[2] &= ~(7u<<8);
	PORTB->PCR[2] |= (3u<<8); /* ALT 3 */

	PORTB->PCR[3] &= ~(7u<<8);
	PORTB->PCR[3] |= (3u<<8); /* ALT 3 */

	PORTB->PCR[4] &= ~(7u<<8);
	PORTB->PCR[4] |= (3u<<8); /* ALT 3 */

	PORTB->PCR[5] &= ~(7u<<8);
	PORTB->PCR[5] |= (3u<<8); /* ALT 4 */
}

static void SPI1_GPIO_Config(void)
{
	EnableClockPortB(Enable);

	PORTB->PCR[14] &= ~(7u<<8);
	PORTB->PCR[14] |= (3u<<8); /* ALT 3 */

	PORTB->PCR[15] &= ~(7u<<8);
	PORTB->PCR[15] |= (3u<<8); /* ALT 3 */

	PORTB->PCR[16] &= ~(7u<<8);
	PORTB->PCR[16] |= (3u<<8); /* ALT 3 */

	PORTB->PCR[17] &= ~(7u<<8);
	PORTB->PCR[17] |= (3u<<8); /* ALT 4 */
}

/*
  SPI0 - PORT B
  
  PTB2 - SCK (ALT 3)
  PTB3 - SIN (ALT 3)
  PTB4 - SOUT (ALT 3)
  PTB5 - PCS0 (ALT 4)
*/
void SPI0_Init(void)
{
	unsigned int CFGR1=0;
	unsigned int TCR=0;
	unsigned int CCR=0;
	unsigned int CR=0;

	SPI0_GPIO_Config();

	SPI0_CLK_ENABLE(Enable,SPLLDIV2_CLK); /* 30Mhz */

	SPI0->CR = 2; 	/* Reset Master */
	SPI0->CR = 0; 	/* Disable Module for config */
	
	SPI0->IER = 0; 	/* No use Interrupt */
	SPI0->DER = 0; 	/* No use DMA */

	SPI0->CFGR0 = 0; /* Defaults */

	CFGR1 |= (1u<<3); /* NO STALL */
	CFGR1 |= (1u<<0); /* Master Mode */
	SPI0->CFGR1 = CFGR1;
	
	SPI0->FCR = 0x3u; /* TXWATER = 3 */
	
	CCR |= (4u<<24); 	/* SCKPCS = 4+1 =5 */
	CCR |= (4u<<16); 	/* PCSSCK = 4+1 =5 */
	CCR |= (8u<<8); 	/* DBT = 8+2 =10 */
	CCR |= (13u<<0); 	/* SCKDIV = 13+2 = 15 cycles */
	SPI0->CCR = CCR;	
	
	TCR &= ~(1u<<31); 	/* CPOL: LOW */
	TCR |= (1u<<30); 		/* CPHA 1 */
	TCR &= ~(7u<<27); 	/* FRESCALE DIV 1 = 2*1 = 2 -> 30/2=15Mhz*/
	TCR &= ~(3u<<24); 	/* PCS[0] */
	// TCR |= (1u<<18);    /* TXMSK */
	TCR |= (15u<<0); 		/* FRAME SIZE = 15+1 = 16 */
	SPI0->TCR = TCR;
	
	CR |= (1u<<3); 	/* Debug Enable */
	CR |= (1u<<0); 	/* Module Enable */
	SPI0->CR = CR;	

}

void SPI0_Transmit_16bit(unsigned int DataSend)
{
	while( !(SPI0->SR & (1u<<0)) ) /* TDF: Wait transmit data is requested */

	SPI0->TDR = DataSend;

	SPI0->SR |= (1u<<0); /* Clear TDF flag */
}

void SPI1_Init(void)
{
	/* Pin number        | Function
	* ----------------- |------------------
	* PTB14             | LPSPI1_SCK
	* PTB15							| LPSPI1_SIN
	* PTB16			        | LPSPI1_SOUT
	* PTB17			        | LPSPI1_PCS3 */

	SCG->FIRCDIV |= (1<<8u); /* FIRCDIV /1 */
	
	/* 1. Setting SCK/PCS/SOUT/SIN pin */
	SPI1_GPIO_Config();

	/* 2. Select source LPUART  */
	SPI1_CLK_ENABLE(Enable,FIRCDIV2_CLK);
		
	/* 3. Setting baud rate */
	SPI1->CCR |= (4u);      /* SCK Divider: 6-2 = 4 */
	SPI1->CCR |= (4u<<24u);
	SPI1->CCR |= (4u<<16u);
	
	/* 3.4.5.6 */	
	/*3. Set Prescaler Value */  /*4. CPOL =0, CPHA = 0 */ 
	/*5. Set frame size is 8 bits (FRAMESZ + 1) */
	/*6. Transfer using PCS3  */
	SPI1->TCR = ( (3u<<27u) | (15u) | (3u<<24u) ); 					
	
	/* 7. Setting Transmit/Receive FIFO */
	SPI1->FCR  |= (3u);        				/* Transmit FIFO */
	SPI1->CFGR1 |= (1u<<3u);  				/* No Stall if Tx FIFO empty or Rx FIFO full*/

	/* 9. Enable LPSPI module */
	SPI1->CR |= (1u<<3u);
	SPI1->CR |= (1u);
}

void SPI1_Transmit(unsigned short DataSend)
{
	while((SPI1->SR & (1u)) == 0 ){}
                                   /* Wait for Tx FIFO available 	*/
  SPI1->TDR = DataSend;              /* Transmit data 				*/
}

void LPSPI_init(void)
{
	/* Pin number        | Function
	* ----------------- |------------------
	* PTB14             | LPSPI1_SCK
	* PTB15             | LPSPI1_SIN
	* PTB16			        | LPSPI1_SOUT
	* PTB17			        | LPSPI1_PCS3 */
	
	SCG->FIRCDIV |= (1<<8u); /* FIRCDIV /1 */
	
	/* 1. Setting SCK/PCS/SOUT/SIN pin */
	SPI1_GPIO_Config();

	/* 2. Select source LPUART  */
	SPI1_CLK_ENABLE(Enable,FIRCDIV2_CLK);
		
	/* 3. Setting baud rate */
	SPI1->CCR |= (4u);      /* SCK Divider: 6-2 = 4 */
	SPI1->CCR |= (4u<<24u);
	SPI1->CCR |= (4u<<16u);
	
	/* 3.4.5.6 */	
	/*3. Set Prescaler Value */  /*4. CPOL =0, CPHA = 0 */ 
	/*5. Set frame size is 8 bits (FRAMESZ + 1) */
	/*6. Transfer using PCS3  */
	SPI1->TCR = ( (3u<<27u) | (15u) | (3u<<24u) ); 					
	
	/* 7. Setting Transmit/Receive FIFO */
	SPI1->FCR  |= (3u);        				/* Transmit FIFO */
	SPI1->CFGR1 |= (1u<<3u);  				/* No Stall if Tx FIFO empty or Rx FIFO full*/
	
	/* 8. Configures LPSPI mode */ 
	SPI1->CFGR1 |= (1u) ;       				/* Master mode */
	SPI1->CR |= (1u<<3u);      				/* Debug mode */

	/* 9. Enable LPSPI module */
	SPI1->CR |= (1u);
	
}

/*========================================================================================
																MAIN TEST
=========================================================================================*/
void SPI_MAIN_TEST(void)
{
	// SOSC_Init_8Mhz();
	// SPLL_Init(Div_2,Div_2);
	// Run_Mode_Clock(SPLLDIV2_CLK, RunDiv_2, RunDiv_2, RunDiv_3);
  //CheckClock();

	// LPIT_Init_CH0(60000);

	// SPI0_Init();
	// SPI0_Transmit_16bit(Data);

	// SPI1_Init();

	// SPI1_Transmit(Data);

	LPSPI_init();

	/* intensity*/
	SPI1_Transmit(0x0A01);
	
	/* scan limit*/
	SPI1_Transmit(0x0B07);

	/* Normal Operation */
	SPI1_Transmit(0x0C01);

	/* Display Test */
	//LPSPI1_send_char(0x0F01);
 
	/* Decode mode */
	SPI1_Transmit(0x09FF);

	/* set 1 for led 0 */
	SPI1_Transmit(0x0101);
	SPI1_Transmit(0x020F);
	SPI1_Transmit(0x030C);
	SPI1_Transmit(0x040C);
	SPI1_Transmit(0x050C);
	SPI1_Transmit(0x060C);
	SPI1_Transmit(0x070C);
	SPI1_Transmit(0x080C);

	while(1)
	{
		//SPI0_Transmit_16bit(Data);
		
		// while( !(SPI0->SR & (1u<<0)) ) /* wait transmit data is requested */

		// SPI0->TDR = Data;

		// SPI0->SR |= (1u<<0); /* Clear TDF flag */
		
		// SysTickDelay_ms(1000);
	}
}
/*=========================================================================================*/


