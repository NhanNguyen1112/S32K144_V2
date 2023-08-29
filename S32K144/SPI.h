
#ifndef _SPI_
#define _SPI_

#include "S32K144.h"
#include "SPI_Register.h"

/*
  SPI0 - PORT B

  PTB2 - SCK (ALT 3)
  PTB3 - SIN (ALT 3)
  PTB4 - SOUT (ALT 3)
  PTB5 - PCS0 (ALT 4)
*/

void SPI_MAIN_TEST(void);

void SPI0_Init(void);
void SPI0_Transmit_16bit(unsigned int Data);

void SPI1_Init(void);
void SPI1_Transmit(unsigned short Data);

#endif /* _SPI_ */





