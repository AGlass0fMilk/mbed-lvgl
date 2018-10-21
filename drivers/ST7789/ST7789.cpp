/**
 * @file SSD1351.cpp
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

#include "ST7789.h"

#include "platform/mbed_wait_api.h"

ST7789Display::ST7789Display(DisplayInterface& interface,
		PinName reset, PinName backlight) : DisplayDriver(interface),
		_reset(reset, 1), _backlight(NULL), _inverted(false)
{
	if(backlight != NC)
	{
		_backlight = new mbed::PwmOut(backlight);
		_backlight->period_ms(100);
	}
}

void ST7789Display::init(void)
{
	// Hardware reset
	_reset = 0;
	wait_ms(10);
	_reset = 1;
	wait_ms(10);

	// Software reset
	_interface.write_command(ST77XX_SWRESET);
	wait_ms(100);
	// Out of sleep mode
	_interface.write_command(ST77XX_SLPOUT);
	wait_ms(100);

	uint8_t buf[8];

	// Set color mode
	buf[0] = 0x55;
	_interface.write_command_with_params(ST77XX_COLMOD, buf, 1);
	wait_ms(10);

	// Set memory access control
	buf[0] = 0x00;
	_interface.write_command_with_params(ST77XX_MADCTL, buf, 1);

	// Set column address
/*	buf[0] = 0x00; // Start address high byte
	buf[1] = 0x00; // Start address low byte
	buf[2] = 0x00; // End address high byte
	buf[3] = 0xF0; // End address low byte
	_interface.write_command_with_params(ST77XX_CASET, buf, 4);

	// Set row address
	_interface.write_command_with_params(ST77XX_RASET, buf, 4);*/

	// Normal display on
	_interface.write_command(ST77XX_NORON);
	wait_ms(10);

	// Display on
	_interface.write_command(ST77XX_DISPON);
	wait_ms(100);

}

void ST7789Display::set_column_address(uint16_t start, uint16_t end)
{
	uint8_t buf[4] = {
			(uint8_t)((start & 0xFF00) >> 8),
			(uint8_t)(start & 0x00FF),
			(uint8_t)((end & 0xFF00) >> 8),
			(uint8_t)(end & 0x00FF)
	};

	_interface.write_command_with_params(ST77XX_CASET, buf, 4);
}

void ST7789Display::set_row_address(uint16_t start, uint16_t end)
{
	uint8_t buf[4] = {
			(uint8_t)((start & 0xFF00) >> 8),
			(uint8_t)(start & 0x00FF),
			(uint8_t)((end & 0xFF00) >> 8),
			(uint8_t)(end & 0x00FF)
		};

	_interface.write_command_with_params(ST77XX_RASET, buf, 4);
}

void ST7789Display::start_ram_write(void) {
	_interface.write_command(ST77XX_RAMWR);
}

void ST7789Display::set_address_mode(uint8_t mode) {
	_interface.write_command_with_params(ST77XX_MADCTL, &mode, 1);
}

void ST7789Display::display_normal_mode(void) {
	_interface.write_command(ST77XX_NORON);
}

void ST7789Display::display_partial_mode(uint8_t* params) {
	_interface.write_command_with_params(ST77XX_PTLAR, params, 4);
	_interface.write_command(ST77XX_PTLON);
}

void ST7789Display::invert(void) {
	if(_inverted)
	{
		_inverted = false;
		_interface.write_command(ST77XX_INVOFF);
	}
	else
	{
		_inverted = true;
		_interface.write_command(ST77XX_INVON);
	}
}

void ST7789Display::enter_sleep_mode(void) {
	_interface.write_command(ST77XX_SLPIN);
}

void ST7789Display::exit_sleep_mode(void) {
	_interface.write_command(ST77XX_SLPOUT);
}

void ST7789Display::tearing_effect_off(void) {
	_interface.write_command(ST77XX_TEOFF);
}

void ST7789Display::tearing_effect_on(uint8_t mode) {
	uint8_t buf = (mode & 0x1);
	_interface.write_command_with_params(ST77XX_TEON, &buf, 1);
}

void ST7789Display::set_brightness(float percentage) {
	if(_backlight)
	{
		*_backlight = percentage;
	}
}

void ST7789Display::fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		lv_color_t color)
{
}

void ST7789Display::flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p)
{
	// This offset is needed in the current rotation
	// Of the display because the ST7789 actually supports
	// Displays up to 240X320. So we need to add 80 to get into
	// The appropriate address space for the display in this rotation
	//static int32_t y_offset = 80;
	static int32_t y_offset = 0;
	this->set_column_address(x1, x2);
	this->set_row_address(y1 + y_offset, y2 + y_offset);
	this->start_ram_write();

	// TODO - Speed this up using DMA!
	// TODO - Make sure initialization is working correctly
	// TODO - Make sure the D/C pin is toggling fast enough (change it to high speed)
	int32_t x, y;
	uint16_t* ptr = (uint16_t*) color_p;
	for(y = y1; y <= y2; y++)
	{
		for(x = x1; x <= x2; x++)
		{
			uint8_t pix_color = 0;
			pix_color = (color_p->red << 3) | ((color_p->green & 0x38) >> 3);
			_interface.write_data(&pix_color, 1);
			pix_color = ((color_p->green & 0x7) << 5) | (color_p->blue);
			_interface.write_data(&pix_color, 1);

			color_p = (const lv_color_t*) ++ptr;
		}
	}
}

void ST7789Display::map(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p)
{
}
