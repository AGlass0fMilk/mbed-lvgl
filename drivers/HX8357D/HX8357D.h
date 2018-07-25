/**
 * @file HX8357D.h
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

#include "hx8357d_registers.h"

class HX8357D : public DisplayDriver
{
	public:

		HX8357D(DisplayInterface& interface) : DisplayDriver(interface) { }

		virtual ~HX8357D() {}

		/*
		 * Initializes the display driver
		 */
		virtual void init(void);

		/**
		 * Resets the display driver
		 */
		void reset(void);

		/**
		 * Gets the display driver's ID
		 * @retval id 32-bit id of the display driver
		 */
		uint32_t get_id(void);

		/**
		 * Gets the display's power mode
		 * @retval mode
		 */
		uint8_t get_power_mode(void);

		/**
		 * Gets the display's address mode
		 */
		uint8_t get_address_mode(void);

		/**
		 * Inverts the display
		 */
		void invert_on(void);

		/**
		 * Deinverts the display
		 */
		void invert_off(void);

		/**
		 * Sets the gamma curve
		 */
		void set_gamma_curve(uint8_t curve);

		/**
		 * Turns the display on
		 */
		void display_on(void);

		/**
		 * Turns the display off
		 */
		void display_off(void);

		void set_column_address(uint16_t col_start, uint16_t col_end);

		void set_row_address(uint16_t row_start, uint16_t row_end);

		void write_memory_start();

		void read_memory_start();

		void set_tear_on();

		void set_tear_off();

		void set_address_mode(uint8_t mode);

		void enter_idle_mode();

		void exit_idle_mode();

		void enter_sleep_mode();

		void exit_sleep_mode();

		void set_extc();

		void set_rgb(uint8_t* mode);

		void set_osc(uint8_t mode);

		void set_panel(uint8_t mode);

		void set_power(uint8_t* mode);

		void set_stba(uint8_t* mode);

		void set_cyc(uint8_t* mode);

		void set_color_mode(uint8_t mode);

		void set_com(uint8_t mode);

#if MBED_USE_LVGL

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
#endif

};

#endif /* LVGL_DRIVERS_HX8357D_H_ */
