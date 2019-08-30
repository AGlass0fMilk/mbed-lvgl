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

#ifndef MBED_LVGL_DRIVERS_NORITAKELVGL_H_

#define MBED_LVGL_DRIVERS_NORITAKELVGL_H_

#include "NoritakeVFD.h"
#include <LVGLDisplayDriver.h>

#if (LV_COLOR_DEPTH != 1)
#warning LV_COLOR_DEPTH must be set to 1 for the NoritakeVFD monochrome display to work properly
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
