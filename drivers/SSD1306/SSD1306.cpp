/**
 * @file SSD1306.cpp
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
 *  Created on: Jul 3, 2018
 *      Author: gdbeckstein
 */

#include "SSD1306.h"

SSD1306Display::SSD1306Display(mbed::I2C* i2c) : driver(i2c), _i2c(i2c)
{
	// Clear the display RAM
	memset(display_ram, 0, sizeof(display_ram));
	memset(page_valid, 0, sizeof(page_valid));
}

void SSD1306Display::init(void)
{
	driver.initialize();
	driver.set_memory_addressing_mode(0);
	driver.hv_set_column_address(0, 127);
	driver.hv_set_page_address(0, 127);
	flush_ram(0, 0, 127, 63);
	driver.on();
}

void SSD1306Display::flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p)
{
}

void SSD1306Display::map(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p)
{
	lv_color_t* ptr = (lv_color_t*) color_p;
	for(int i = x1; i <= x2; i++)
	{
		for(int j = y1; j <= y2; j++)
		{
			set_pixel(i, j, *ptr);
			ptr++;
		}
	}
	//flush_ram(x1, y1, x2, y2);
}

void SSD1306Display::fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		lv_color_t color)
{
	for(int i = x1; i <= x2; i++)
	{
		for(int j = y1; j <= y2; j++)
		{
			set_pixel(i, j, color);
		}
	}
	//flush_ram(x1, y1, x2, y2);
}

void SSD1306Display::set_pixel(int32_t x, int32_t y, lv_color_t color)
{
	// Calculate the byte/bit location in the display RAM
	uint16_t byte_idx = get_byte_index(x, y);
	uint8_t bit_idx = get_bit_index(y);

	// Invalidate the page
	page_valid[y >> 3] = false;

	// Set clear the bit accordingly
	if(color.full)
		display_ram[byte_idx] |= (1 << bit_idx);
	else
		display_ram[byte_idx] &= ~(1 << bit_idx);
}

void SSD1306Display::flush_ram(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	uint16_t byte_start = get_byte_index(x1, y1);
	uint16_t byte_end = get_byte_index(x2, y2);

	driver.hv_set_page_address((y1 >> 3), (y2 >> 3));
	driver.hv_set_column_address(byte_start, byte_end);

	_i2c->start();
	_i2c->write(0x3C << 1);
	_i2c->write(0x40);
	for (int i = byte_start; i < byte_end + 1; i++)
	{
		_i2c->write(display_ram[i]);
	}
	_i2c->stop();
}

void SSD1306Display::redraw_pages(void)
{
	// Check each page validation flag and redraw pages as necessary
	for(int i = 0; i < sizeof(page_valid); i++)
	{
		if(!page_valid[i])
		{
			// Page has been modified, redraw it
			int32_t y = (i << 3); // i * 8
			flush_ram(0, y, 127, y);
			page_valid[i] = true;
		}
	}
}
//
//void SSD1306Display::flush_ram(void)
//{
//	_i2c->start();
//	_i2c->write(0x3C << 1);
//	_i2c->write(0x40);
//	for(int i = 0; i < sizeof(display_ram); i++)
//	{
//		_i2c->write(display_ram[i]);
//	}
//	_i2c->stop();
//}
