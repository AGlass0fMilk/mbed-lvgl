/**
 * @file SSD1306.h
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
 *  Created on: Jun 14, 2018
 *      Author: gdbeckstein
 */
#ifndef LVGL_DRIVERS_HX8357D_H_
#define LVGL_DRIVERS_HX8357D_H_

#include "DisplayDriver.h"
#include "drivers/I2C.h"
#include "ssd1306_driver.h"

class SSD1306Display : public DisplayDriver
{
	public:

		SSD1306Display(mbed::I2C* i2c);

		virtual ~SSD1306Display() {}

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

	//protected:

		inline uint16_t get_byte_index(int32_t x, int32_t y)
		{
			return x + ((y >> 3) << 7);
		}

		inline uint8_t get_bit_index(int32_t y)
		{
			return y % 8; // Bit = y mod 8
		}

		void set_pixel(int32_t x, int32_t y, lv_color_t color);
		void flush_ram(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

		void redraw_pages(void); /*!< Called periodically to draw invalidated (modifed) pages */

	protected:

		uint8_t display_ram[1024]; /*!< 1kB of display data (monochrome 128X64 bits) */

		bool page_valid[8];	/*!< Keeps track of pages that are modified */

		SSD1306 driver;

		// TODO - get rid of this
		mbed::I2C* _i2c;

};

#endif /* LVGL_DRIVERS_HX8357D_H_ */
