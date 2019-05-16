/*
 * NoritakeLVGL.cpp
 *
 *  Created on: May 15, 2019
 *      Author: gdbeckstein
 */

#include "NoritakeLVGL.h"

NoritakeLVGL::NoritakeLVGL(DisplayInterface& interface, PinName reset,
		uint32_t height, uint32_t width) : NoritakeVFD(interface, reset, height, width) { }

void NoritakeLVGL::flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p) {

	draw_dot_unit_image(0, 0, 128, 32, _buf);

}

void NoritakeLVGL::map(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p) {
}

void NoritakeLVGL::fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		lv_color_t color) {
}

#if LV_VDB_SIZE

void NoritakeLVGL::vdb_write(uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
									 lv_color_t color, lv_opa_t opa) {
	/*Black/White. Store 8 pixel in one byte. Bytes are mapped vertically.  (Set LV_VDB_PX_BPP to 1)*/

	// Start of VDB + (row_width * (y/8)) + x  (selecting row and column in memory)
	buf += buf_w * (y >> 3) + x;
	if(lv_color_brightness(color) > 10) {
		(*buf) |= (1 << (y % 8));	// Set the corresponding bit in the byte buffer
	}
	else {
		(*buf) &= ~(1 << (y % 8)); // Clear the corresponding bit in the byte buffer
	}
}

#endif

