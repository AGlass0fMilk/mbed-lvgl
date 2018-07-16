/**
 * @file DisplayDriver.h
 * @brief Abstract base class for LittleVGL display drivers
 *
 *  Created on: Jun 14, 2018
 *      Author: gdbeckstein
 */
#ifndef LVGL_DRIVERS_DISPLAYDRIVER_H_
#define LVGL_DRIVERS_DISPLAYDRIVER_H_

#include "lvgl.h"
#include "stdint.h"
#include "../lv_conf_lib.h"

class DisplayDriver
{
	public:

		virtual ~DisplayDriver() { }

		/*
		 * Initializes the display driver
		 */
		virtual void init(void) = 0;

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

	protected:

		/*
		 * @brief This method MUST be called at the end of the subclass's flush() implementation!
		 */
		void flush_ready(void) { lv_flush_ready(); }

};



#endif /* LVGL_DRIVERS_DISPLAYDRIVER_H_ */
