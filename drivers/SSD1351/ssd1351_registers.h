/**
 * @file ssd1351_registers.h
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
 *  Created on: Jul 25, 2018
 *      Author: gdbeckstein
 */
#ifndef MBED_LVGL_DRIVERS_SSD1351_SSD1351_REGISTERS_H_
#define MBED_LVGL_DRIVERS_SSD1351_SSD1351_REGISTERS_H_

#define SSD1351_SET_COL_ADDR			0x15
#define SSD1351_SET_ROW_ADDR			0x75
#define SSD1351_WRITE_RAM				0x5C
#define SSD1351_READ_RAM				0x5D
#define SSD1351_REMAP					0xA0
#define SSD1351_SET_START_LINE_RAM	0xA1
#define SSD1351_SET_START_LINE_ROW	0xA2
#define SSD1351_DISPLAY_MODE_OFF		0xA4
#define SSD1351_DISPLAY_MODE_ALL_ON	0xA5
#define SSD1351_DISPLAY_MODE_NORMAL	0xA6
#define SSD1351_DISPLAY_MODE_INVERT	0xA7
#define SSD1351_FUNCTION_SELECT		0xAB
#define SSD1351_NOP_1					0xAD
#define SSD1351_SLEEP_MODE_ON			0xAE
#define SSD1351_SLEEP_MODE_OFF		0xAF
#define SSD1351_NOP_2					0xB0
#define SSD1351_SET_RESET_PRECHG		0xB1
#define SSD1351_FREQUENCY_DIVIDER	0xB3
#define SSD1351_SET_SGMT_LOW_VOLTG	0xB4
#define SSD1351_SET_GPIO				0xB5
#define SSD1351_SET_SECOND_PRECHG	0xB6
#define SSD1351_SET_GRAY_SCALE_PW	0xB8
#define SSD1351_RESET_GRAY_SCALE_PW	0xB9
#define SSD1351_SET_PRE_CHG_VOLTAGE	0xBB
#define SSD1351_SET_VCOMH_VOLTAGE	0xBE
#define SSD1351_SET_CONTRAST_SINGLE	0xC1
#define SSD1351_SET_CONTRAST_GLOBAL	0xC7
#define SSD1351_SET_MUX_RATIO			0xCA
#define SSD1351_NOP_3					0xD1
#define SSD1351_NOP_4					0XE3
#define SSD1351_SET_CMD_LOCK			0xFD
#define SSD1351_HORIZONTAL_SCROLL	0x96
#define SSD1351_STOP_MOVING			0x9E
#define SSD1351_START_MOVING			0x9F



#endif /* MBED_LVGL_DRIVERS_SSD1351_SSD1351_REGISTERS_H_ */
