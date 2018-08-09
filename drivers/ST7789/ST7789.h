/**
 * @file ST7789.h
 * @brief DisplayDriver implementation for Sitronix ST7789 driver IC
 *
 * @copyright Copyright &copy; 2018 Heads Up Display, Inc.
 *
 *  Created on: Aug 9, 2018
 *      Author: gdbeckstein
 */
#ifndef MBED_LVGL_DRIVERS_ST7789_ST7789_H_
#define MBED_LVGL_DRIVERS_ST7789_ST7789_H_

#include "DisplayDriver.h"

#include "drivers/DigitalOut.h"
#include "drivers/PwmOut.h"

#include "st7789_registers.h"

class ST7789Display : public DisplayDriver
{

	public:

		/**
		 * Instantiate an ST7789 display
		 * @param[in] interface Display interface to use to talk to ST7789
		 * @param[in] reset Pin to use for resetting the display
		 * @param[in] backlight Backlight PWM control pin (optional)
		 */
		ST7789Display(DisplayInterface& interface,
				PinName reset, PinName backlight = NC);

		virtual ~ST7789Display() {}

		/*
		 * Initializes the display driver
		 */
		virtual void init(void);

		/**
		 * Sets the column address pointer
		 * @param[in] start starting address
		 * @param[in] end ending address
		 */
		void set_column_address(uint16_t start, uint16_t end);

		/**
		 * Sets the row address pointer
		 * @param[in] start starting address
		 * @param[in] end ending address
		 */
		void set_row_address(uint16_t start, uint16_t end);

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
		 * Resets the display to normal mode
		 */
		void display_normal_mode(void);

		/**
		 * Partial display mode
		 * @param[in] params Parameters specifying start and end rows of
		 * partial area (4 bytes total)
		 */
		void display_partial_mode(uint8_t* params);

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
		 * Tearing effect (synchronization signal) off
		 */
		void tearing_effect_off(void);

		/**
		 * Tearing effect (synchronization signal) on
		 * @param[in] mode Configures the tearing effect signal as follows:
		 * Mode 0 - v-blanking info only (low -> display is updating from memory)
		 * Mode 1 - v and h-blanking (low -> display is updating from memory)
		 *
		 * In mode 1, a complete frame is transferred
		 * every 321 pulses (1 v-sync and 320 h-sync)
		 */
		void tearing_effect_on(uint8_t mode);

		/**
		 * Set's the display's backlight brightness
		 * @param[in] percentage Percentage of backlight brightness desired
		 *
		 * @note requires a PWM output
		 */
		void set_brightness(float percentage);

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



	private:

		/** Active low output to reset the ST7789 */
		mbed::DigitalOut _reset;

		/** PWM Control for the backlight brightness */
		mbed::PwmOut* _backlight;

		/** Inversion status */
		bool _inverted;


};

#endif /* MBED_LVGL_DRIVERS_ST7789_ST7789_H_ */
