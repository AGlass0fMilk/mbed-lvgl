#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stdio.h"
#include "stdint.h"
#include <stddef.h>
#include "GUD900.h"
#include "GUD_I2C.h"
#include "GUD_SPI.h"
#include "GUD_USART.h"

int main(){
	GUD_SPI_Init();
	startSPI();
	GUD_Init(128, 32); //Change the parameters based on your module's resolution.
	writeString("Hello World!");
	endSPI();
}