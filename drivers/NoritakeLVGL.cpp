/* LittlevGL for Mbed-OS library
 * Copyright (c) 2018-2019 George "AGlass0fMilk" Beckstein
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "NoritakeLVGL.h"

NoritakeLVGL::NoritakeLVGL(DisplayInterface& interface, PinName reset,
		uint32_t height, uint32_t width) : NoritakeVFD(interface, reset, height, width) {

	set_resolution(width, height);
	// Allocate our own display buffer based on height and width
	// Each pixel is one bit, so allocate (width*height)/8 bytes
	unsigned int num_bytes = ((width*height) >> 3);
	lv_color_t* disp_buf = new lv_color_t[num_bytes]();

	memset((void*) disp_buf, 0, num_bytes);

	// Here we kind of lie and say the buffer is 8X bigger than it really is
	primary_display_buffer = mbed::Span<lv_color_t>(disp_buf, num_bytes*8);

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

	draw_dot_unit_image(area->x1, area->y1, (area->x2 - area->x1)+1, (area->y2 - area->y1)+1, (unsigned char*) color_p);
}

void NoritakeLVGL::set_pixel(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
        lv_color_t color, lv_opa_t opa)
{
	/*Black/White. Store 8 pixels in one byte. Bytes are mapped vertically.  (Set LV_COLOR_DEPTH to 1)*/

	// Start of display buffer + (x *4 + (y/8)) (selecting row and column in memory)
	buf += ((x << 2) + (y >> 3));
	if(lv_color_brightness(color) < 10) {
		(*buf) |= (0x80 >> (y % 8));	// Set the corresponding bit in the byte buffer
	}
	else {
		(*buf) &= ~(0x80 >> (y % 8)); // Clear the corresponding bit in the byte buffer
	}
}

