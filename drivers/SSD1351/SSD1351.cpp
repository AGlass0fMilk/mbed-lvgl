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

#include "platform/mbed_wait_api.h"

SSD1351Display::SSD1351Display(DisplayInterface& interface,
		PinName reset, PinName shutdown) : DisplayDriver(interface),
		_reset(reset), _shutdown(NULL)
{
	if(shutdown != NC)
	{
		_shutdown = new mbed::DigitalOut(shutdown, 1);
	}
}

void SSD1351Display::init(void)
{
}

void SSD1351Display::enable_power(bool enable)
{
	if(enable)
	{
		// Powering on the display
		// Set reset low for at least 2us
		_reset = 0;
		wait_us(3);
		_reset = 1;

		// Power on VCC (high voltage)
		this->shutdown(true);

		// Application is responsible for waiting for VCC to stabilize
		// before sending the display ON command
	}
	else
	{
		// Powering down the display
		// Send display off (enter sleep mode)
		this->enter_sleep_mode();

		// Power down VCC (high voltage)
		this->shutdown(false);

		// Application is responsible for waiting typically 100ms before
		// powering down VDDIO (3.3V to display)
	}
}

void SSD1351Display::set_column_address(uint8_t start, uint8_t end)
{
	const uint8_t buf[2] = { start, end };
	_interface.write_command_with_params(SSD1351_SET_COL_ADDR, buf, 2);
}

void SSD1351Display::set_row_address(uint8_t start, uint8_t end)
{
	const uint8_t buf[2] = { start, end };
	_interface.write_command_with_params(SSD1351_SET_ROW_ADDR, buf, 2);
}

void SSD1351Display::start_ram_write(void)
{
	_interface.write_command(SSD1351_WRITE_RAM);
}

void SSD1351Display::set_address_mode(uint8_t mode)
{
	_interface.write_command_with_params(SSD1351_REMAP, &mode, 1);
}

void SSD1351Display::display_all_off(void)
{
	_interface.write_command(SSD1351_DISPLAY_MODE_OFF);
}

void SSD1351Display::display_all_on(void)
{
	_interface.write_command(SSD1351_DISPLAY_MODE_ALL_ON);
}

void SSD1351Display::display_normal_mode(void)
{
	_interface.write_command(SSD1351_DISPLAY_MODE_NORMAL);
}

void SSD1351Display::invert(void)
{
	_interface.write_command(SSD1351_DISPLAY_MODE_INVERT);
}

void SSD1351Display::enter_sleep_mode(void)
{
	_interface.write_command(SSD1351_SLEEP_MODE_ON);
}

void SSD1351Display::exit_sleep_mode(void)
{
	_interface.write_command(SSD1351_SLEEP_MODE_OFF);
}

void SSD1351Display::set_reset_precharge(uint8_t setting)
{
	_interface.write_command_with_params(SSD1351_SET_RESET_PRECHG, &setting, 1);
}

void SSD1351Display::set_oscillator_frequency(uint8_t setting)
{
	_interface.write_command_with_params(SSD1351_FREQUENCY_DIVIDER, &setting, 1);
}

void SSD1351Display::set_segment_low_voltage(bool internal)
{
	const uint8_t buf[3] = { 0xA0 | (((uint8_t)internal) << 1), 0xB5, 0x55 };
	_interface.write_command_with_params(SSD1351_SET_SGMT_LOW_VOLTG, buf, 3);
}

void SSD1351Display::set_second_precharge_period(uint8_t period)
{
	_interface.write_command_with_params(SSD1351_SET_SECOND_PRECHG, &period, 1);
}

void SSD1351Display::set_gray_scale_LUT(uint8_t* table)
{
	_interface.write_command_with_params(SSD1351_SET_GRAY_SCALE_PW, table, 63);
}

void SSD1351Display::default_gray_scale_LUT(void)
{
	_interface.write_command(SSD1351_RESET_GRAY_SCALE_PW);
}

void SSD1351Display::set_precharge_voltage(uint8_t setting)
{
	_interface.write_command_with_params(SSD1351_SET_PRE_CHG_VOLTAGE, &setting, 1);
}

void SSD1351Display::set_vcomh_voltage(uint8_t setting)
{
	_interface.write_command_with_params(SSD1351_SET_VCOMH_VOLTAGE, &setting, 1);
}

void SSD1351Display::set_white_balance(uint8_t a, uint8_t b, uint8_t c)
{
	const uint8_t buf[3] = { a, b, c };
	_interface.write_command_with_params(SSD1351_SET_CONTRAST_SINGLE, buf, 3);
}

void SSD1351Display::set_contrast(uint8_t setting)
{
	_interface.write_command_with_params(SSD1351_SET_CONTRAST_GLOBAL, &setting, 1);
}

void SSD1351Display::set_mux_ratio(uint8_t setting)
{
	_interface.write_command_with_params(SSD1351_SET_MUX_RATIO, &setting, 1);
}

void SSD1351Display::set_command_lock(uint8_t setting)
{
	_interface.write_command_with_params(SSD1351_SET_CMD_LOCK, &setting, 1);
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
