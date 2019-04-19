/*
 * NoritakeVFD.cpp
 *
 *  Created on: Oct 30, 2018
 *      Author: gdbeckstein
 */

#include "NoritakeVFD.h"

NoritakeVFD::NoritakeVFD(DisplayInterface& interface, uint32_t height,
		uint32_t width) :
		DisplayDriver(interface), _height(height), _width(width), _lines(
				height / 8) {
}

void NoritakeVFD::init(void) {

	// Issue a software reset to initialize the display
	_interface.write_command(0x1b);
	_interface.write_command(0x40);
}

void NoritakeVFD::back(void) {
	_interface.write_command(0x08);
}

void NoritakeVFD::forward(void) {
	_interface.write_command(0x09);
}

void NoritakeVFD::linefeed() {
	_interface.write_command(0x0a);
}

void NoritakeVFD::home() {
	_interface.write_command(0x0b);
}

void NoritakeVFD::carriage_return() {
	_interface.write_command(0x0d);
}

void NoritakeVFD::crlf() {
	_interface.write_command(0x0d);
	_interface.write_command(0x0a);
}

void NoritakeVFD::send_xy(unsigned x, unsigned y) {
	y /= 8;
	_interface.write_command(x);
	_interface.write_command(x >> 8);
	_interface.write_command(y);
	_interface.write_command(y >> 8);
}

void NoritakeVFD::send_xy1(unsigned x, unsigned y) {
	_interface.write_command(x);
	_interface.write_command(x >> 8);
	_interface.write_command(y);
	_interface.write_command(y >> 8);
}

void NoritakeVFD::us_command() {
	_interface.write_command(0x1f);
	_interface.write_command(0x28);
}

void NoritakeVFD::set_cursor(unsigned x, unsigned y) {
	_interface.write_command(0x1f);
	_interface.write_command('$');
	this->send_xy(x, y);
}

void NoritakeVFD::clear_screen() {
	_interface.write_command(0x0c);
}

void NoritakeVFD::cursor_on() {
	_interface.write_command(0x1f);
	_interface.write_command('C');
	_interface.write_command(0x01);
}

void NoritakeVFD::cursor_off() {
	_interface.write_command(0x1f);
	_interface.write_command('C');
	_interface.write_command(0x00);
}

void NoritakeVFD::dot_mode_8x16() {
	this->us_command();
	_interface.write_command(0x67);
	_interface.write_command(0x01);
	_interface.write_command(0x02);
}

void NoritakeVFD::use_multi_byte_chars(uint8_t enable) {
	this->us_command();
	_interface.write_command('g');
	_interface.write_command(0x02);
	_interface.write_command(enable);
}

void NoritakeVFD::set_multi_byte_char_set(uint8_t code) {
	this->us_command();
	_interface.write_command('g');
	_interface.write_command(0x0f);
	_interface.write_command(code);
}

void NoritakeVFD::use_custom_chars(uint8_t enable) {
	_interface.write_command(0x1b);
	_interface.write_command('%');
	_interface.write_command(enable);
}

uint8_t NoritakeVFD::get_column(uint8_t* src, int col) {
	uint8_t out = 0;

	for (int i = 0; i < 8; i++) {
		if (src[i] & (1 << (4 - col))) {
			out += 1 << (7 - 1);
		}
	}
	return out;
}

void NoritakeVFD::define_custom_char(uint8_t code, uint8_t format,
		uint8_t* data) {
	int j = 0;
	int count;

	_interface.write_command(0x1b);
	_interface.write_command('&');
	_interface.write_command(0x01);
	_interface.write_command(code);
	_interface.write_command(code);

	switch (format) {
	case 0: //GUD9005x7Format
		_interface.write_command(0x05);
		count = 5;
		while (count != 0) {
			_interface.write_command(data[j]);
			j++;
			count--;
		}
		break;
	case 1: //GUD9007x8Format
		_interface.write_command(0x07);
		count = 7;
		while (count != 0) {
			_interface.write_command(data[j]);
			j++;
			count--;
		}
		break;
	case 2: //CUUFormat
		_interface.write_command(0x05);
		for (int i = 0; i < 5; i++) {
			_interface.write_command(this->get_column(data, i));
		}
		break;
	}
}

void NoritakeVFD::delete_custom_char(uint8_t code) {
	_interface.write_command(0x1b);
	_interface.write_command('?');
	_interface.write_command(0x01);
	_interface.write_command(code);
}

void NoritakeVFD::set_ascii_variant(uint8_t code) {
	if (code < 0x0d) {
		_interface.write_command(0x1b);
		_interface.write_command('R');
		_interface.write_command(code);
	}
}

void NoritakeVFD::set_char_set(uint8_t code) {
	if (code < 0x05 || (0x10 <= code && code <= 0x13)) {
		_interface.write_command(0x1b);
		_interface.write_command('t');
		_interface.write_command(code);
	}
}

void NoritakeVFD::set_scroll_mode(uint8_t mode) {
	_interface.write_command(0x1f);
	_interface.write_command(mode);
}

void NoritakeVFD::set_horizontal_scroll_speed(uint8_t speed) {
	_interface.write_command(0x1f);
	_interface.write_command('s');
	_interface.write_command(speed);
}

void NoritakeVFD::invert_off() {
	_interface.write_command(0x1f);
	_interface.write_command('r');
	_interface.write_command(0x00);
}

void NoritakeVFD::invert_on() {
	_interface.write_command(0x1f);
	_interface.write_command('r');
	_interface.write_command(0x01);
}

void NoritakeVFD::set_composition_mode(uint8_t mode) {
	_interface.write_command(0x1f);
	_interface.write_command('w');
	_interface.write_command(mode);
}

void NoritakeVFD::set_screen_brightness(unsigned level) {
	if (level == 0) {
		this->display_off();
	} else if (level <= 100) {
		this->display_on();
		_interface.write_command(0x1f);
		_interface.write_command('X');
		_interface.write_command((level * 10 + 120) / 125);
	}
}

void NoritakeVFD::wait(uint8_t wait) {
	this->us_command();
	_interface.write_command('a');
	_interface.write_command(0x01);
	_interface.write_command(wait);
}

void NoritakeVFD::scroll_screen(unsigned x, unsigned y, unsigned times,
		uint8_t speed) {
	unsigned pos = (x * _lines) + (y / 8);
	this->us_command();
	_interface.write_command('a');
	_interface.write_command(0x10);
	_interface.write_command(pos);
	_interface.write_command(pos >> 8);
	_interface.write_command(times);
	_interface.write_command(times >> 8);
	_interface.write_command(speed);
}

void NoritakeVFD::blink_screen_off() {
	this->us_command();
	_interface.write_command('a');
	_interface.write_command(0x11);
	_interface.write_command(0x00);
	_interface.write_command(0x00);
	_interface.write_command(0x00);
	_interface.write_command(0x00);
}

void NoritakeVFD::blink_screen_on(uint8_t enable, uint8_t reverse,
		uint8_t onTime, uint8_t offTime, uint8_t cycles) {
	this->us_command();
	_interface.write_command('a');
	_interface.write_command(0x11);
	_interface.write_command(enable ? (reverse ? 2 : 1) : 0);
	_interface.write_command(onTime);
	_interface.write_command(offTime);
	_interface.write_command(cycles);
}

void NoritakeVFD::display_off() {
	this->us_command();
	_interface.write_command('a');
	_interface.write_command(0x40);
	_interface.write_command(0x00);
}

void NoritakeVFD::display_on() {
	this->us_command();
	_interface.write_command('a');
	_interface.write_command(0x40);
	_interface.write_command(0x01);
}

void NoritakeVFD::screen_saver(uint8_t mode) {
	this->us_command();
	_interface.write_command('a');
	_interface.write_command(0x40);
	_interface.write_command(mode);
}

void NoritakeVFD::set_font_style(uint8_t proportional, uint8_t evenSpacing) {
	this->us_command();
	_interface.write_command('g');
	_interface.write_command(0x03);
	_interface.write_command(proportional * 2 + evenSpacing);
}

void NoritakeVFD::set_font_size(uint8_t x, uint8_t y, uint8_t tall) {
	if (x <= 4 && y <= 2) {
		this->us_command();
		_interface.write_command('g');
		_interface.write_command(0x40);
		_interface.write_command(x);
		_interface.write_command(y);
		this->us_command();
		_interface.write_command('g');
		_interface.write_command(0x01);
		_interface.write_command(tall + 1);
	}
}

void NoritakeVFD::select_window(uint8_t window) {
	if (window <= 4) {
		_interface.write_command(0x10 + window);
	}
}

void NoritakeVFD::define_window(uint8_t window, unsigned x, unsigned y,
		unsigned width, unsigned height) {
	this->us_command();
	_interface.write_command('w');
	_interface.write_command(0x02);
	_interface.write_command(window);
	_interface.write_command(0x01);
	this->send_xy(x, y);
	this->send_xy(width, height);
}

void NoritakeVFD::delete_window(uint8_t window) {
	this->us_command();
	_interface.write_command('w');
	_interface.write_command(0x02);
	_interface.write_command(window);
	_interface.write_command(0x00);
	this->send_xy(0, 0);
	this->send_xy(0, 0);
}

void NoritakeVFD::join_screens() {
	this->us_command();
	_interface.write_command('w');
	_interface.write_command(0x10);
	_interface.write_command(0x01);
}

void NoritakeVFD::separate_screens() {
	this->us_command();
	_interface.write_command('w');
	_interface.write_command(0x10);
	_interface.write_command(0x00);
}

/**
 Calculate the minimum value between two given values.

 @param  x Value 1.
 @param  y Value 2.
 @return The smaller integer.
 */
int min(int x, int y) {
	int result = (x < y) ? x : y;
	return result;
}

void NoritakeVFD::fill_rect(unsigned x0, unsigned y0, unsigned x1, unsigned y1,
		uint8_t on) {
	x0 = min(_width, x0);
	x1 = min(_width, x1);
	y0 = min(_height, y0);
	y1 = min(_height, y1);
	if (y1 <= y0 || x1 <= x0) {
		return;
	}
	uint8_t bufw = 8;
	uint8_t bufh = (y1 - y0 + 7) / 8 * 8;
	// Allocate an empty buffer array
	uint8_t *buf = new uint8_t[(bufh / 8 * bufw)]();
	for (unsigned x = 0; x < x1 - x0; x += bufw) {
		uint8_t part = (x + bufw < x1 - x0) ? bufw : (x1 - x0) - x;
		if (on) {
			for (uint8_t col = 0; col < part; col++) {
				for (uint8_t py = y0 % 8; py < y0 % 8 + min(y1 - y0, 8); py++) {
					buf[col * bufh / 8] |= 1 << (7 - py);
				}
				for (uint8_t row = (y0 + 7) / 8; row < y1 / 8; row++) {
					buf[row - y0 / 8 + col * bufh / 8] = 0xff;
				}
				if (y0 / 8 != y1 / 8) {
					for (uint8_t py = 0; py < y1 % 8; py++) {
						buf[(y1 - y0) / 8 + col * bufh / 8] |= 1 << (7 - py);
					}
				}
			}
			this->set_cursor(x + x0, y0);
			this->draw_image(bufw, bufh, buf);
		}
	}
	// Make sure to delete the dynamically allocated buffer!
	delete[] buf;
}

void NoritakeVFD::draw_image(unsigned width, uint8_t height, uint8_t* data) {
	if (height > _height) {
		return;
	}
	this->us_command();
	_interface.write_command('f');
	_interface.write_command(0x11);
	this->send_xy(width, height);
	_interface.write_command(0x01);
	for (unsigned i = 0; i < (height / 8) * width; i++) {
		_interface.write_command(data[i]);
	}
}

void NoritakeVFD::draw_dot_unit_image(unsigned x, uint8_t y, unsigned width,
		uint8_t height, uint8_t* data) {
	this->us_command();
	_interface.write_command('d');
	_interface.write_command(0x21);
	this->send_xy1(x, y);
	this->send_xy1(width, height);
	_interface.write_command(0x01);
	for (unsigned i = 0; i < (height / 8) * width; i++) {
		_interface.write_command(data[i]);
	}
}

void NoritakeVFD::print_dot_unit_char(unsigned x, uint8_t y, uint8_t* buffer,
		uint8_t len) {
	this->us_command();
	_interface.write_command('d');
	_interface.write_command(0x30);
	this->send_xy(x, y * 8);
	_interface.write_command(0x00);
	_interface.write_command(len);
	while (len--) {
		_interface.write_command(*buffer++);
	}
}

void NoritakeVFD::FROM_image_definition(uint8_t aL, uint8_t aH, uint8_t aE,
		unsigned length, uint8_t lE, uint8_t* data) {
	_interface.write_command(0x1f);
	_interface.write_command(0x28);
	_interface.write_command(0x65);
	_interface.write_command(0x10);
	_interface.write_command(aL);
	_interface.write_command(aH);
	_interface.write_command(aE);
	_interface.write_command(length);
	_interface.write_command(length >> 8);
	_interface.write_command(lE);
	while (length--) {
		_interface.write_command(*data++);
	}
}

void NoritakeVFD::draw_FROM_image(unsigned x, unsigned y, uint8_t memory,
		uint8_t aL, uint8_t aH, uint8_t aE, uint8_t yL, uint8_t yH,
		unsigned xOffset, unsigned yOffset, unsigned xSize, unsigned ySize) {
	_interface.write_command(0x1f);
	_interface.write_command(0x28);
	_interface.write_command(0x64);
	_interface.write_command(0x20);
	this->send_xy1(x, y);
	_interface.write_command(memory);
	_interface.write_command(aL);
	_interface.write_command(aH);
	_interface.write_command(aE);
	_interface.write_command(yL);
	_interface.write_command(yH);
	this->send_xy1(xOffset, yOffset);
	this->send_xy1(xSize, ySize);
	_interface.write_command(0x01);
}

void NoritakeVFD::enter_user_setup_mode() {
	_interface.write_command(0x1f);
	_interface.write_command(0x28);
	_interface.write_command(0x65);
	_interface.write_command(0x01);
	_interface.write_command(0x49);
	_interface.write_command(0x4e);
}

void NoritakeVFD::end_user_setup_mode() {
	_interface.write_command(0x1f);
	_interface.write_command(0x28);
	_interface.write_command(0x65);
	_interface.write_command(0x02);
	_interface.write_command(0x4f);
	_interface.write_command(0x55);
	_interface.write_command(0x54);
}

void NoritakeVFD::touch_status_read_all() {
	_interface.write_command(0x1f);
	_interface.write_command(0x4b);
	_interface.write_command(0x10);
}

void NoritakeVFD::touch_status_read(uint8_t switchNum) {
	_interface.write_command(0x1f);
	_interface.write_command(0x4b);
	_interface.write_command(0x11);
	_interface.write_command(switchNum);
}

void NoritakeVFD::touch_set(uint8_t mode) {
	_interface.write_command(0x1f);
	_interface.write_command(0x4b);
	_interface.write_command(0x18);
	_interface.write_command(mode);
}

void NoritakeVFD::touch_level_read() {
	_interface.write_command(0x1f);
	_interface.write_command(0x4b);
	_interface.write_command(0x14);
}

void NoritakeVFD::touch_change_param(uint8_t mode, uint8_t value) {
	_interface.write_command(0x1f);
	_interface.write_command(0x4b);
	_interface.write_command(0x70);
	_interface.write_command(mode);
	_interface.write_command(value);
}

void NoritakeVFD::IO_port_setting(uint8_t portSetting) {
	_interface.write_command(0x1f);
	_interface.write_command(0x28);
	_interface.write_command(0x70);
	_interface.write_command(0x01);
	_interface.write_command(0x00);
	_interface.write_command(portSetting);
}

void NoritakeVFD::IO_port_output(uint8_t portValue) {
	_interface.write_command(0x1f);
	_interface.write_command(0x28);
	_interface.write_command(0x70);
	_interface.write_command(0x10);
	_interface.write_command(0x00);
	_interface.write_command(portValue);
}

void NoritakeVFD::IO_port_input() {

	_interface.write_command(0x1f);
	_interface.write_command(0x28);
	_interface.write_command(0x70);
	_interface.write_command(0x20);
	_interface.write_command(0x00);
}

inline uint16_t get_byte_index(int32_t x, int32_t y)
{
	return x + ((y >> 3) << 7);
}

inline uint8_t get_bit_index(int32_t y)
{
	return y % 8; // Bit = y mod 8
}

void NoritakeVFD::set_pixel(int32_t x, int32_t y, lv_color_t color)
{
	// Calculate the byte/bit location in the display RAM
	uint16_t byte_idx = get_byte_index(x, y);
	uint8_t bit_idx = get_bit_index(y);

	// Invalidate the page
	//page_valid[y >> 3] = false;

	// Set clear the bit accordingly
	if(color.full)
		_buf[byte_idx] |= (1 << bit_idx);
	else
		_buf[byte_idx] &= ~(1 << bit_idx);
}

void NoritakeVFD::flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p) {

	// Copy it over to the internal bitwise buffer
	int32_t x, y;
	for(y = y1; y <= y2; y++)
	{
		for(x = x1; x <= x2; x++)
		{
			set_pixel(x, y, *color_p);

			color_p++;
		}
	}

	draw_dot_unit_image(0, 0, 128, 32, _buf);

	// Flush the internal buffer
	this->flush_ready();

}

void NoritakeVFD::map(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p) {
}

void NoritakeVFD::fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		lv_color_t color) {
}
