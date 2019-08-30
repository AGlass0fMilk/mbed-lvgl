/*
 * ST7789LVGL.h
 *
 *  Created on: Aug 27, 2019
 *      Author: gdbeckstein
 */

#ifndef MBED_LVGL_DRIVERS_ST7789LVGL_H_
#define MBED_LVGL_DRIVERS_ST7789LVGL_H_

#include "ST7789.h"
#include <LVGLDisplayDriver.h>

class ST7789LVGL : public LVGLDisplayDriver, public ST7789Display {

public:

	/**
	 * Instantiate an ST7789LVGL display
	 * @param[in] interface Display interface to use to talk to ST7789
	 * @param[in] reset Pin to use for resetting the display
	 * @param[in] backlight Backlight PWM control pin (optional)
	 * @param[in] primary_display_buffer (optional) The user may provide a display buffer to use (or one will be dynamically allocated)
	 * @param[in] secondary_display_buffer (optional) If using a double-buffered scheme, the user must provide both display buffers
	 *
	 */
	ST7789LVGL(DisplayInterface& interface, PinName reset, PinName backlight = NC,
			mbed::Span<lv_color_t> primary_display_buffer = mbed::Span<lv_color_t, 0>(),
			mbed::Span<lv_color_t> secondary_display_buffer = mbed::Span<lv_color_t, 0>()) :
				LVGLDisplayDriver(primary_display_buffer, secondary_display_buffer),
				ST7789Display(interface, reset, backlight) { }

protected:

		/*
		 * @brief Flush the content of the internal buffer to the specific area on the display
		 * You can use DMA or any hardware acceleration to do this operation in the background but
		 * this->flush_ready() has to be called when finished
		 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h
		 */
		virtual void flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {

			// Number of pixels * size of pixel data in bytes
			uint32_t size_bytes = (area->x2-area->x1+1)*(area->y2-area->y1+1)*sizeof(lv_color_t);

			// This offset is needed in the current rotation
			// Of the display because the ST7789 actually supports
			// Displays up to 240X320. So we need to add 80 to get into
			// The appropriate address space for the display in this rotation
			static int32_t y_offset = -80;

			this->set_column_address(area->x1, area->x2);
			this->set_row_address(area->y1, area->y2);
			this->start_ram_write();
			this->write_data((uint8_t*) color_p, size_bytes);
		}
};



#endif /* MBED_LVGL_DRIVERS_ST7789LVGL_H_ */
