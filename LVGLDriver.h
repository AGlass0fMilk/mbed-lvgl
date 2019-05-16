/*
 * LVGLDriver.h
 *
 *  Created on: Apr 19, 2019
 *      Author: gdbeckstein
 */

#ifndef MBED_LVGL_DRIVERS_LVGLDRIVER_H_
#define MBED_LVGL_DRIVERS_LVGLDRIVER_H_

extern "C" {
#include "lv_vdb.h"
}

class LVGLDriver
{
public:

		virtual ~LVGLDriver() { }

		/*
		 * @brief Flush the content of the internal buffer to the specific area on the display
		 * You can use DMA or any hardware acceleration to do this operation in the background but
		 * this->flush_ready() has to be called when finished
		 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h
		 */
		virtual void flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t* color_p) = 0;

		/*
		 * @brief Write a pixel array (called 'map') to the a specific area on the display
		 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h
		 */
		virtual void map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t* color_p) = 0;

		/*
		 * @brief Write a pixel array (called 'map') to the a specific area on the display
		 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h
		 */
		virtual void fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color) = 0;

#if USE_LV_GPU

		/*
		 * @brief If your MCU has hardware accelerator (GPU) then you can use it to blend to memories using opacity
		 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)
		 */
		virtual void gpu_blend(lv_color_t* dest, const lv_color_t* src, uint32_t length, lv_opt_t opa) = 0;

		/*
		 * @brief If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color
		 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)
		 */
		virtual void gpu_fill(lv_color_t* dest, uint32_t length, lv_color_t color) = 0;

#endif

#if LV_VDB_SIZE

		/**
		 * Subclass returns true if it has a custom VDB write function
		 */
		virtual bool has_vdb_func(void) = 0;

	    /*Optional: Set a pixel in a buffer according to the requirements of the display*/
		virtual void vdb_write(uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
				lv_color_t color, lv_opa_t opa) = 0;

#endif

};



#endif /* MBED_LVGL_DRIVERS_LVGLDRIVER_H_ */
