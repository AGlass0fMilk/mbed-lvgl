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

#include "SSD1351.h"

SSD1351Display::SSD1351Display(PinName mosi, PinName sclk, PinName cs,
		PinName dc, PinName reset)
{

}

void SSD1351Display::init(void)
{
}

void SSD1351Display::flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p)
{
}

void SSD1351Display::map(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p)
{
}

void SSD1351Display::fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		lv_color_t color)
{
}
