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

#include "drivers/DigitalOut.h"

#include "ssd1351_registers.h"

class SSD1351Display : public DisplayDriver
{

	public:

		/**
		 * Instantiate an SSD1351 display
		 */
		SSD1351Display(DisplayInterface& interface,
				PinName reset, PinName shutdown = NC);

		virtual ~SSD1351Display() {}

		/*
		 * Initializes the display driver
		 */
		virtual void init(void);

		/**
		 * Enable/Disable the high voltage power supply (if supported)
		 * @note this handles most of the power on/off timing
		 *
		 * Power on timing:
		 * After VDDIO (3.3V) rises:
		 * - delay at least 1ms (application responsible for this)
		 * - set reset low for at least 2us
		 * - after reset goes low, wait at least 2us to power on VCC (ie: 13V)
		 * - after VCC stabilizes, send 0xAF command for display ON (this will take 200ms)
		 *
		 * Power off timing:
		 * - Send display off (0xAE)
		 * - Power down VCC
		 * - Wait typically 100ms before powering down VDDIO
		 *
		 * @param[in] enable true if high voltage should be ON
		 */
		void enable_power(bool enable);

		/**
		 * Sets the column address pointer
		 * @param[in] start starting address (0 to 127)
		 * @param[in] end endingaddress (0 to 127)
		 */
		void set_column_address(uint8_t start, uint8_t end);

		/**
		 * Sets the row address pointer
		 * @param[in] start starting address (0 to 127)
		 * @param[in] end endingaddress (0 to 127)
		 */
		void set_row_address(uint8_t start, uint8_t end);

		/**
		 * Enable MCU to write data into RAM
		 */
		void start_ram_write(void);

		// Not supported currently
		//void start_ram_read(void);

		/**
		 * Sets address mode
		 * Remap color depth
		 *
		 * @param[in] mode Combination of flags
		 */
		void set_address_mode(uint8_t mode);

		/**
		 * Turns all pixels on the display off (does not affect VRAM)
		 */
		void display_all_off(void);

		/**
		 * Turns all pixels on the display on (does not affect VRAM)
		 */
		void display_all_on(void);

		/**
		 * Resets the display to normal mode (displaying contents of VRAM)
		 */
		void display_normal_mode(void);

		/**
		 * Inverts the display
		 */
		void invert(void);

		// function_select - not supported

		/**
		 * Enters sleep mode (display off)
		 */
		void enter_sleep_mode(void);

		/**
		 * Exits sleep mode (display on)
		 */
		void exit_sleep_mode(void);

		/**
		 * Sets the reset (phase 1) and precharge (phase 2) period
		 * @param[in] setting byte setting (see datasheet for more info)
		 */
		void set_reset_precharge(uint8_t setting);

		/**
		 * Sets up the oscillator frequency
		 * (normally locked by command 0xFD)
		 */
		void set_oscillator_frequency(uint8_t setting);

		/**
		 * Sets the VSL voltage as follows:
		 * true - use internal vsl (VSL pin is NC)
		 * false - use external vsl (reset value)
		 */
		void set_segment_low_voltage(bool internal);

		//void set_gpio()

		/**
		 * Sets the second precharge period
		 */
		void set_second_precharge_period(uint8_t period);

		/**
		 * Sets the gray scale lookup table used by the SSD1351
		 * @param[in] table a 63-byte table to use as the gray scale LUT
		 */
		void set_gray_scale_LUT(uint8_t* table);

		/**
		 * Resets the SSD1351 gray scale look up table to default
		 */
		void default_gray_scale_LUT(void);

		/** Sets the precharge voltage */
		void set_precharge_voltage(uint8_t setting);

		/** Set COM deselect voltage leve */
		void set_vcomh_voltage(uint8_t setting);

		/** Sets contrast for each color */
		void set_white_balance(uint8_t a, uint8_t b, uint8_t c);

		/** Sets the global contrast control */
		void set_contrast(uint8_t setting);

		/** Sets the mux ratio */
		void set_mux_ratio(uint8_t setting);

		/** Sets the command lock settings */
		void set_command_lock(uint8_t setting);

		// horizontal scroll
		// stop scrolling
		// start scrolling

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

	private:

		/** Asserts the shutdown pin, if present */
		void shutdown(bool enable) {
			if(_shutdown)
				_shutdown = enable;
		}

	private:

		/** Active low output to reset the SSD1351 */
		mbed::DigitalOut _reset;

		/** Active low output to shutdown the SSD1351 high voltage power supply (ie: 13V) */
		mbed::DigitalOut* _shutdown;


};

#endif /* MBED_LVGL_DRIVERS_SSD1351_SSD1351_H_ */
