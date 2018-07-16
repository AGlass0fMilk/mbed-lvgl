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
#include "stdint.h"
#include "lvgl_objects.h"
#include "stm32f4xx_hal_sram.h"

#include "platform/mbed_assert.h"

#define CMD 0
#define DATA (1 << 16)
#define SRAM_BANK_ADDR_8BIT(d) *((__IO uint8_t*)(0x60000000 + d))

void i8080_8bit_init(i8080_8bit_t *obj)
{
	/** Perform the SRAM1 memory initialization sequence */
	obj->sram_handle.Instance = FSMC_NORSRAM_DEVICE;
	obj->sram_handle.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	obj->sram_handle.Init.NSBank = FSMC_NORSRAM_BANK1;
	obj->sram_handle.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	obj->sram_handle.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	obj->sram_handle.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_8;
	obj->sram_handle.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	obj->sram_handle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	obj->sram_handle.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	obj->sram_handle.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	obj->sram_handle.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	obj->sram_handle.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	obj->sram_handle.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	obj->sram_handle.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	obj->sram_handle.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	obj->sram_handle.Init.PageSize = FSMC_PAGE_SIZE_NONE;

	/* Timing */
	obj->timing.AddressSetupTime = 3;
	obj->timing.AddressHoldTime = 2;
	obj->timing.DataSetupTime = 3;
	obj->timing.BusTurnAroundDuration = 0;
	obj->timing.CLKDivision = 0;
	obj->timing.DataLatency = 0;
	obj->timing.AccessMode = FSMC_ACCESS_MODE_B;

	// Initialize the "SRAM" FSMC interface
	MBED_ASSERT(HAL_SRAM_Init(&obj->sram_handle, &Timing, NULL) == HAL_OK);

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


