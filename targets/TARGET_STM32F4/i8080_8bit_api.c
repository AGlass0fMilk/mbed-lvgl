/**
 * @file i8080_8bit_api.c
 * @brief Brief Description
 * 
 * Detailed Description
 *
 * Link to [markdown like this](@ref PageTag)
 * Make sure you tag the markdown page like this:
 * # Page title {#PageTag}
 * 
 * <a href='MyPDF.pdf'> Link to PDF documents like this</a>
 * If you add document files, make sure to add them into a directory inside a "docs" folder
 * And then run hud-devices/tools/copy-dox-files.py 
 *
 * To use images, make sure they're in an "images" folder and follow the doxygen user manual to add images.
 * You must run copy-dox-files.py after adding images as well.
 *
 * @copyright Copyright &copy; 2018 Heads Up Display, Inc.
 *
 *  Created on: Jun 16, 2018
 *      Author: gdbeckstein
 */

#include "i8080_8bit_api.h"
#include "lvgl_objects.h"
#include "stdint.h"
#include "stm32f4xx_hal_gpio.h"

#include "platform/mbed_assert.h"

#define CMD 0
#define DATA (1 << 16)
#define SRAM_BANK_ADDR_8BIT(d) *((__IO uint8_t*)(0x60000000 + d))

//#if DEVICE_I8080_8BIT

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable FSMC clock */
  __HAL_RCC_FSMC_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

	/*-- GPIO Configuration ------------------------------------------------------*/
	/* SRAM Data lines,  NOE (/RD) and NWE (/WR) configuration */

	// 8-bit interface (PORT D)
	GPIO_InitStructure.Pin = GPIO_PIN_14 // D0
								  | GPIO_PIN_15 // D1
								  | GPIO_PIN_0	 // D2
								  | GPIO_PIN_1	 // D3
								  | GPIO_PIN_7  // ~CS
								  | GPIO_PIN_11 // ~RS
								  | GPIO_PIN_4	 // ~RD
								  | GPIO_PIN_5; // ~WR
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;

	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

	// 8-bit interface (PORT E)
	GPIO_InitStructure.Pin = GPIO_PIN_7   // D4
								  | GPIO_PIN_8   // D5
								  | GPIO_PIN_9   // D6
								  | GPIO_PIN_10; // D7
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* RESET */
	GPIO_InitStructure.Pin = GPIO_PIN_12;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull  = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
}

void i8080_8bit_init(i8080_8bit_t *obj)
{
	/** Configure the SRAM device */
	obj->sram_handle.Instance = FSMC_NORSRAM_DEVICE;
	obj->sram_handle.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

	obj->timing.AddressSetupTime = 3;
	obj->timing.AddressHoldTime = 2;
	obj->timing.DataSetupTime = 3;
	obj->timing.BusTurnAroundDuration = 0;
	obj->timing.CLKDivision = 0;
	obj->timing.DataLatency = 0;
	obj->timing.AccessMode = FSMC_ACCESS_MODE_B;

	obj->sram_handle.Init.NSBank = FSMC_NORSRAM_BANK1;
	obj->sram_handle.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	obj->sram_handle.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	obj->sram_handle.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_8;
	obj->sram_handle.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	obj->sram_handle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	obj->sram_handle.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	obj->sram_handle.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	obj->sram_handle.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	obj->sram_handle.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	obj->sram_handle.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;

	// Initialize the "SRAM" FSMC interface
	MBED_ASSERT(HAL_SRAM_Init(&obj->sram_handle, &obj->timing, &obj->timing)
			== HAL_OK);

	//i8080_init_pins();
}

void i8080_8bit_free(i8080_8bit_t *obj)
{

	//TODO - deinit pins
}

void i8080_8bit_write(i8080_8bit_t* obj, uint8_t value)
{
	SRAM_BANK_ADDR_8BIT(CMD) = value;
}

void i8080_8bit_write_bytes(i8080_8bit_t *obj,
		uint8_t* value, uint32_t len)
{
	SRAM_BANK_ADDR_8BIT(CMD) = *value;
	for(int i = 1; i < len; i++)
	{
		SRAM_BANK_ADDR_8BIT(DATA) = value[i];
	}

}

uint8_t i8080_8bit_read(i8080_8bit_t *obj)
{
	return (uint8_t) SRAM_BANK_ADDR_8BIT(CMD);
}

//#endif


