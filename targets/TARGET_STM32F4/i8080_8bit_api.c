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

//#define CMD 0
//#define DATA (1 << 16)
//#define SRAM_BANK_ADDR_8BIT(d) *((__IO uint8_t*)(0x60000000 + d))

/**
 * The FSMC peripheral maps this address space to an external memory device
 * So simply writing to these addresses in code will initiate a transfer on the
 * FSMC parallel bus
 *
 * The NORSRAM _BANK1 we use here starts at address 0x60000000
 */
#define SRAM_START_ADDRESS 0x60000000

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
	MBED_ASSERT(HAL_SRAM_Init(&obj->sram_handle, &obj->timing, NULL) == HAL_OK);

	/*
	 * The HAL drivers will call HAL_SRAM_MspInit to finish configuring
	 * the SRAM pins for the specific target.
	 *
	 * See the target's i8080_8bit_hal.c file for details
	 */
}

void i8080_8bit_free(i8080_8bit_t *obj)
{
	/*
	 * The HAL drivers will call HAL_SRAM_MspDeInit to finish configuring
	 * the SRAM pins for the specific target.
	 *
	 * See the target's i8080_8bit_hal.c file for details
	 */
	MBED_ASSERT(HAL_SRAM_DeInit(&obj->sram_handle) == HAL_OK);
}

void i8080_8bit_write(i8080_8bit_t* obj, uint8_t value)
{
	HAL_SRAM_Write_8b(&obj->sram_handle,
			(uint32_t*) SRAM_START_ADDRESS,
			&value, 1);
}

void i8080_8bit_write_bytes(i8080_8bit_t *obj, uint8_t* value, uint32_t len)
{
	// TODO Look into using the Write_DMA function instead?
	HAL_SRAM_Write_8b(&obj->sram_handle,
			(uint32_t*) SRAM_START_ADDRESS,
			value, len);

}

void i8080_8bit_read(i8080_8bit_t *obj, uint8_t* buf, uint32_t len)
{
	HAL_SRAM_Read_8b(&obj->sram_handle,
			(uint32_t*) SRAM_START_ADDRESS,
			buf, len);
}


