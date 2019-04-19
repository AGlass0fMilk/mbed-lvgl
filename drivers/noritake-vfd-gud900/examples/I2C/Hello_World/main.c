#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stdio.h"
#include "stdint.h"
#include <stddef.h>
#include "GUD900.h"
#include "GUD_I2C.h"
#include "GUD_SPI.h"
#include "GUD_USART.h"

int main()
{
	GUD_I2C_Init(0x50);
	GUD_I2C_Start(I2C_Direction_Transmitter);
	GUD_Init(128, 32);
	GUD_I2C_Stop();
	GUD_I2C_MultiWrite("Hello World!");
}