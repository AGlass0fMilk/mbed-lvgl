/**
 * @file hx8357d_driver.h
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
#ifndef LVGL_DRIVERS_HX8357D_HX8357D_DRIVER_H_
#define LVGL_DRIVERS_HX8357D_HX8357D_DRIVER_H_

#include "stdint.h"
#include "i8080_8bit_api.h"

#ifdef __cplusplus
extern "C" {
#endif

void hx8357d_init(i8080_8bit_t* obj);

void hx8357d_nop(i8080_8bit_t* obj);
void hx8357d_soft_reset(i8080_8bit_t* obj);
uint32_t hx8357d_get_id(i8080_8bit_t* obj);
uint8_t hx8357d_get_power_mode(i8080_8bit_t* obj);
uint8_t hx8357d_get_address_mode(i8080_8bit_t* obj);
void hx8357d_invert_on(i8080_8bit_t* obj);
void hx8357d_invert_off(i8080_8bit_t* obj);
void hx8357d_set_gamma_curve(i8080_8bit_t* obj, uint8_t curve);
void hx8357d_display_on(i8080_8bit_t* obj);
void hx8357d_display_off(i8080_8bit_t* obj);
void hx8357d_set_column_address(i8080_8bit_t* obj, uint16_t col_start, uint16_t col_end);
void hx8357d_set_row_address(i8080_8bit_t* obj, uint16_t row_start, uint16_t row_end);
void hx8357d_write_memory_start(i8080_8bit_t* obj);
void hx8357d_read_memory_start(i8080_8bit_t* obj);
void hx8357d_set_tear_on(i8080_8bit_t* obj);
void hx8357d_set_tear_off(i8080_8bit_t* obj);
void hx8357d_set_address_mode(i8080_8bit_t* obj, uint8_t mode);
void hx8357d_enter_idle_mode(i8080_8bit_t* obj);
void hx8357d_exit_idle_mode(i8080_8bit_t* obj);
void hx8357d_enter_sleep_mode(i8080_8bit_t* obj);
void hx8357d_exit_sleep_mode(i8080_8bit_t* obj);
void hx8357d_set_extc(i8080_8bit_t* obj);
void hx8357d_set_rgb(i8080_8bit_t* obj, uint8_t* mode);
void hx8357d_set_osc(i8080_8bit_t* obj, uint8_t mode);
void hx8357d_set_panel(i8080_8bit_t* obj, uint8_t mode);
void hx8357d_set_power(i8080_8bit_t* obj, uint8_t* mode);
void hx8357d_set_stba(i8080_8bit_t* obj, uint8_t* mode);
void hx8357d_set_cyc(i8080_8bit_t* obj, uint8_t* mode);
void hx8357d_set_color_mode(i8080_8bit_t* obj, uint8_t mode);
void hx8357d_set_com(i8080_8bit_t* obj, uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif /* LVGL_DRIVERS_HX8357D_HX8357D_DRIVER_H_ */
