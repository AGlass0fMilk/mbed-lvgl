/*
 * NoritakeLVGL.h
 *
 *  Created on: May 15, 2019
 *      Author: gdbeckstein
 */

#ifndef MBED_LVGL_DRIVERS_NORITAKELVGL_H_

#define MBED_LVGL_DRIVERS_NORITAKELVGL_H_

#include "NoritakeVFD.h"
#include <LVGLDisplayDriver.h>

#if (LV_COLOR_DEPTH != 1)
#error LV_COLOR_DEPTH must be set to 1 for the NoritakeVFD monochrome display
#endif

class NoritakeLVGL : public LVGLDisplayDriver, public NoritakeVFD {
	public:
		/**
		 * Instantiates a DisplayDriver with a given DisplayInterface
		 * @parameter[in] interface Display interface to use to communicate with VFD module
		 * @parameter[in] reset (optional) Reset pin to display
		 * @parameter[in] height (optional) Height in pixels of the VFD display
		 * @paramter[in] width (optional) Width in pixels of the VFD display
		 */
		NoritakeLVGL(DisplayInterface& interface,
				PinName reset = NC, uint32_t height = 32, uint32_t width = 128);

		virtual ~NoritakeLVGL();


protected:

		/*
		 * @brief Flush the content of the internal buffer to the specific area on the display
		 * You can use DMA or any hardware acceleration to do this operation in the background but
		 * this function must block the caller until the transfer is complete!
		 */
		virtual void flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

		/**
		 * Subclass returns true if it has a custom pixel write function
		 */
		virtual bool has_pix_write_func(void) {
			return true;
		}

		/*Optional: Set a pixel in a buffer according to the requirements of the display*/
		virtual void set_pixel(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
				lv_color_t color, lv_opa_t opa);

};



#endif /* MBED_LVGL_DRIVERS_NORITAKELVGL_H_ */
