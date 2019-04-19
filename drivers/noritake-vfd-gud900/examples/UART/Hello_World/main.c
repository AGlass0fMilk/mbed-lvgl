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
	GUD_UART_Init(38400);
	GUD_Init(256, 128); //Change to match your module's resolution.
	printUART("Hello World!");
	return 0;
}