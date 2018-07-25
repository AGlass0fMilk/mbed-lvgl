/**
 * @file SSD1351.h
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
 *  Created on: Jul 25, 2018
 *      Author: gdbeckstein
 */
#ifndef MBED_LVGL_DRIVERS_SSD1351_SSD1351_H_
#define MBED_LVGL_DRIVERS_SSD1351_SSD1351_H_

#include "DisplayDriver.h"

#include "ssd1351_registers.h"

class SSD1351Display : public DisplayDriver
{
	public:

		/**
		 * Instantiate an SSD1351 display
		 */
		SSD1351Display(DisplayInterface& interface) : DisplayDriver(interface) { }

		virtual ~SSD1351Display() {}

		/*
		 * Initializes the display driver
		 */
		virtual void init(void);

		/*
		 * @brief Flush the content of the internal buffer to the specific area on the display
		 * You can use DMA or any hardware acceleration to do this operation in the background but
		 * this->flush_ready() has to be called when finished
		 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h
		 */
		virtual void flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
				const lv_color_t* color_p);

		/*
		 * @brief Write a pixel array (called 'map') to the a specific area on the display
		 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h
		 */
		virtual void map(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
				const lv_color_t* color_p);

		/*
		 * @brief Write a pixel array (called 'map') to the a specific area on the display
		 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h
		 */
		virtual void fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
				lv_color_t color);

#if USE_LV_GPU

		/*
		 * @brief If your MCU has hardware accelerator (GPU) then you can use it to blend to memories using opacity
		 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)
		 */
		virtual void gpu_blend(lv_color_t* dest, const lv_color_t* src, uint32_t length, lv_opt_t opa) { }

		/*
		 * @brief If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color
		 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)
		 */
		virtual void gpu_fill(lv_color_t* dest, uint32_t length, lv_color_t color) { }

#endif

	private:

		mbed::


};

#endif /* MBED_LVGL_DRIVERS_SSD1351_SSD1351_H_ */
