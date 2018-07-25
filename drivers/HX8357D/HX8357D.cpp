/**
 * @file HX8357D.cpp
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
 *  Created on: Jun 14, 2018
 *      Author: gdbeckstein
 */

#include "HX8357D.h"
#include "hx8357d_registers.h"

#include "platform/mbed_wait_api.h"

void HX8357D::init(void) {

	uint8_t buffer[8];

	// Issue a soft reset and wait a moment
	this->reset();
	wait_ms(10);

	this->set_extc();
	wait_ms(10);

	// Set RGB
	buffer[0] = 0;
	buffer[1] = 0;
	buffer[2] = 0x6;
	buffer[3] = 0x6;
	this->set_rgb(buffer);

	// SetCOM (0x25)
	this->set_com(0x25);

	//SetOSC(0x68)
	this->set_osc(0x68);

	//SetPanel(0x05)
	this->set_panel(0x05);

	//SetPWR1( 0x00, 0x15, 0x1C, 0x1C, 0x83, 0xAA )
	buffer[0] = 0;
	buffer[1] = 0x15;
	buffer[2] = 0x1C;
	buffer[3] = 0x1C;
	buffer[4] = 0x83;
	buffer[5] = 0xAA;
	buffer[6] = 0x29;
	this->set_power(buffer);

	//SetSTBA( 0x50, 0x50, 0x01, 0x3C, 0x1E, 0x08 )
	buffer[0] = 0x50;
	buffer[1] = 0x50;
	buffer[2] = 0x01;
	buffer[3] = 0x3C;
	buffer[4] = 0x1E;
	buffer[5] = 0x08;
	this->set_stba(buffer);

	//SetCYC (0x02, 0x40, 0x00, 0x2A, 0x2A, 0x0D, 0x78)
	buffer[0] = 0x02;
	buffer[1] = 0x40;
	buffer[2] = 0x00;
	buffer[3] = 0x2A;
	buffer[4] = 0x2A;
	buffer[5] = 0x0D;
	buffer[6] = 0x78;
	this->set_cyc(buffer);

	// COLMOD (0x55)
	this->set_color_mode(0x55);

	// MADCTL (0xC0)
	this->set_address_mode(0x60); //0xC0);

	//this->set_tear_on();

	// TEON (0x0)
	// TEARLINE(0x00, 0x02)
	// SLPOUT
	this->exit_sleep_mode();
	wait_ms(150);

	this->display_on();
	wait_ms(50);
}

void HX8357D::reset(void) {
	_interface.write_command(HX8357_SWRESET);
}

uint32_t HX8357D::get_id(void) {
	uint32_t id = 0;
	_interface.write_command(HX8357_RDDID);
	id |= (_interface.read() << 16);
	id |= (_interface.read() << 8);
	id |= (_interface.read());
	return id;
}

uint8_t HX8357D::get_power_mode(void) {
	_interface.write_command(HX8357B_RDPOWMODE);
	return _interface.read();
}

uint8_t HX8357D::get_address_mode(void) {
	_interface.write_command(HX8357B_RDMADCTL);
	return _interface.read();
}

void HX8357D::invert_on(void) {

	_interface.write_command(HX8357_INVON);
}

void HX8357D::invert_off(void) {

	_interface.write_command(HX8357_INVOFF);
}

void HX8357D::set_gamma_curve(uint8_t curve) {
	_interface.write_command(HX8357_GAMSET);
	_interface.write_data(&curve, 1);
}

void HX8357D::display_on(void) {
	_interface.write_command(HX8357_DISPON);
}

void HX8357D::display_off(void) {
	_interface.write_command(HX8357_DISPOFF);
}

void HX8357D::set_column_address(uint16_t col_start, uint16_t col_end) {

	uint8_t cmd[] = {
			(uint8_t)((col_start & 0xFF00) >> 8),
			(uint8_t)((col_start & 0x00FF)),
			(uint8_t)((col_end & 0xFF00) >> 8),
			(uint8_t)((col_end & 0x00FF))
	};

	_interface.write_command(HX8357_CASET);

	_interface.write_data(cmd, 4);

}

void HX8357D::set_row_address(uint16_t row_start, uint16_t row_end) {
	uint8_t cmd[] = {
			(uint8_t)((row_start & 0xFF00) >> 8),
			(uint8_t)((row_start & 0x00FF)),
			(uint8_t)((row_end & 0xFF00) >> 8),
			(uint8_t)((row_end & 0x00FF))
	};

	_interface.write_command(HX8357_PASET);

	_interface.write_data(cmd, 4);
}

void HX8357D::write_memory_start() {
	_interface.write_command(HX8357_RAMWR);
}

void HX8357D::read_memory_start() {
	_interface.write_command(HX8357_RAMRD);
}

void HX8357D::set_tear_on() {
	_interface.write_command(HX8357_TEON);
}

void HX8357D::set_tear_off() {
	_interface.write_command(HX8357_TEOFF);
}

void HX8357D::set_address_mode(uint8_t mode) {
	_interface.write_command(HX8357_MADCTL);
	_interface.write_data(&mode, 1);

}

void HX8357D::enter_idle_mode() {
	_interface.write_command(HX8357_IDMON);

}

void HX8357D::exit_idle_mode() {
	_interface.write_command(HX8357_IDMOFF);
}

void HX8357D::enter_sleep_mode() {
	_interface.write_command(HX8357_SLPIN);
}

void HX8357D::exit_sleep_mode() {
	_interface.write_command(HX8357_SLPOUT);
}

void HX8357D::set_extc() {

	// This is kind of like a passkey you have to write
	uint8_t cmd[] = {
			0xFF, 0x83, 0x57
	};

	_interface.write_command(HX8357_PASET);

	_interface.write_data(cmd, 3);
}

void HX8357D::set_rgb(uint8_t* mode) {
	_interface.write_command(HX8357_SETRGB);
	_interface.write_data(mode, 4);
}

void HX8357D::set_osc(uint8_t mode) {
	_interface.write_command(HX8357_SETOSC);
	_interface.write_data(&mode, 1);
}

void HX8357D::set_panel(uint8_t mode) {
	_interface.write_command(HX8357_SETPANEL);
	_interface.write_data(&mode, 1);
}

void HX8357D::set_power(uint8_t* mode) {
	_interface.write_command(HX8357_SETPWR1);
	_interface.write_data(mode, 7);
}

void HX8357D::set_stba(uint8_t* mode) {
	_interface.write_command(HX8357D_SETSTBA);
	_interface.write_data(mode, 6);
}

void HX8357D::set_cyc(uint8_t* mode) {
	_interface.write_command(HX8357D_SETCYC);
	_interface.write_data(mode, 7);
}

void HX8357D::set_color_mode(uint8_t mode) {
	_interface.write_command(HX8357_COLMOD);
	_interface.write_data(&mode, 1);
}

void HX8357D::set_com(uint8_t mode) {
	_interface.write_command(HX8357D_SETCOM);
	_interface.write_data(&mode, 1 );
}

void HX8357D::flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p) {
	this->set_column_address(x1, x2);
	this->set_row_address(y1, y2);

	this->write_memory_start();

	//_interface.write_data((uint16_t*) color_p, (x2-x1)*(y2-y1));

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

void HX8357D::map(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p) {
}

void HX8357D::fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		lv_color_t color) {
}
