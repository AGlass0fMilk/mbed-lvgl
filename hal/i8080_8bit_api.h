/**
 * @file i8080_8bit_api.h
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
#ifndef LVGL_HAL_I8080_8BIT_API_H_
#define LVGL_HAL_I8080_8BIT_API_H_

#include "device.h"
#include "stdint.h"
#include "lvgl_objects.h"

#ifdef __cplusplus
extern "C" {
#endif

/** i8080_8bit hal structure. i8080_8bit_s is declared in the target's hal
 */
typedef struct i8080_8bit_s i8080_8bit_t;

/**
 * \defgroup hal_i8080_8bit hal functions
 * @{
 */

/** Initialize the i8080_8bit peripheral
 *
 * Configures the pin used by i8080_8bit.
 * @param obj The i8080_8bit object to initialize
 */
void i8080_8bit_init(i8080_8bit_t *obj);

/** Release the i8080_8bit object
 *
 * Note: This is not currently used in the mbed-drivers
 * @param obj The i8080_8bit object
 */
void i8080_8bit_free(i8080_8bit_t *obj);

/** Write a single byte to the i8080_8bit bus
 * @param obj The i8080_8bit object
 * @param address Address to write to
 * @param value Byte to write to bus
 */
void i8080_8bit_write(i8080_8bit_t* obj, uint32_t address, uint8_t value);

/** Write bytes to the i8080_8bit bus
 *
 * @param obj The i8080_8bit object
 * @param address Starting address to write to
 * @param value Pointer to array of values to write out
 * @param len Number of bytes to write
 * @param increment_addr increment address after each byte?
 */
void i8080_8bit_write_bytes(i8080_8bit_t *obj, uint32_t address,
		uint8_t* value, uint32_t len, uint8_t increment_addr);

/** Read the i8080_8bit bus
 *
 * @param obj The i8080_8bit object
 * @return the 8-bit value on the bus
 */
uint8_t i8080_8bit_read(i8080_8bit_t *obj);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* LVGL_HAL_I8080_8BIT_API_H_ */
