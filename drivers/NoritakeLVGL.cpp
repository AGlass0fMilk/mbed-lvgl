/*
 * NoritakeLVGL.cpp
 *
 *  Created on: May 15, 2019
 *      Author: gdbeckstein
 */

#include "NoritakeLVGL.h"

NoritakeLVGL::NoritakeLVGL(DisplayInterface& interface, PinName reset,
		uint32_t height, uint32_t width) : NoritakeVFD(interface, reset, height, width) {

	// Allocate our own display buffer based on height and width
	// Each pixel is one bit, so allocate (width*height)/8 bytes
	unsigned int num_bytes = ((width*height) >> 3);
	lv_color_t* disp_buf = new lv_color_t[num_bytes];

	primary_display_buffer = mbed::Span<lv_color_t>(disp_buf, num_bytes);

	// Do not use the secondary display buffer
	secondary_display_buffer = mbed::Span<lv_color_t, 0>();

	// Initialize the display buffers
	initialize_display_buffers();

}

NoritakeLVGL::~NoritakeLVGL() {
	// Delete the dynamically-allocated display buffer
	delete[] primary_display_buffer.data();
}

void NoritakeLVGL::flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {

	draw_dot_unit_image(0, 0, 128, 32, (unsigned char*) color_p);

}

void NoritakeLVGL::set_pixel(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
        lv_color_t color, lv_opa_t opa)
{
	buf = (uint8_t*) primary_display_buffer.data();
	/*Black/White. Store 8 pixels in one byte. Bytes are mapped vertically.  (Set LV_COLOR_DEPTH to 1)*/

	// Start of display buffer + (x *4 + (y/8)) (selecting row and column in memory)
	buf += ((x << 2) + (y >> 3));
	if(lv_color_brightness(color) > 10) {
		(*buf) |= (0x80 >> (y % 8));	// Set the corresponding bit in the byte buffer
	}
	else {
		(*buf) &= ~(0x80 >> (y % 8)); // Clear the corresponding bit in the byte buffer
	}
}

