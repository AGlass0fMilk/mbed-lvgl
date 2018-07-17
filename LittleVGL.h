/**
 * @file LittleVGL.h
 * @brief Mbed OS port of LittleVGL graphics library - C++ driver
 *
 *  Created on: Jun 14, 2018
 *      Author: gdbeckstein
 */
#ifndef LVGL_LITTLEVGL_H_
#define LVGL_LITTLEVGL_H_

#include "DisplayDriver.h"

#include "platform/NonCopyable.h"
#include "drivers/Ticker.h"

class LittleVGL : private mbed::NonCopyable<LittleVGL>
{
	public:

		virtual ~LittleVGL();

		/**
		 * @brief Return a single instance of the class
		 *
		 * Due to underlying C implementation of LittleVGL, it is simpler
		 * to implement this API as a singleton. Also, for most cases it
		 * doesn't make much sense to have more than one display
		 *
		 * @retval Singleton instance reference
		 */
		static LittleVGL& get_instance()
		{
			static LittleVGL instance;
			return instance;
		}

		/**
		 * Initializes LittleVGL
		 * @param[in] driver Display driver instance to use
		 */
		void init(DisplayDriver* driver);

		/**
		 * Start the LittleVGL ticker
		 */
		void start(void);

		/**
		 * Stop the LittleVGL ticker
		 */
		void stop(void);

		/**
		 * Updates the LitteVGL graphics system
		 * @note This should be called by the application every 1 to 10ms
		 */
		void update(void);

	protected:

		/**
		 * Internal ticker function
		 */
		void tick(void);

	private:

		/** Private constructor, as class is a singleton */
		LittleVGL();

		/*
		 * @brief Internal function for bridging C/C++ to DisplayDriver instance
		 */
		static void flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t* color_p);

		/*
		 * @brief Internal function for bridging C/C++ to DisplayDriver instance
		 */
		static void map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t* color_p);

		/*
		 * @brief Internal function for bridging C/C++ to DisplayDriver instance
		 */
		static void fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color);

#if USE_LV_GPU

		/*
		 * @brief Internal function for bridging C/C++ to DisplayDriver instance
		 */
		static void gpu_blend(lv_color_t* dest, const lv_color_t* src, uint32_t length, lv_opt_t opa) = 0;

		/*
		 * @brief Internal function for bridging C/C++ to DisplayDriver instance
		 */
		static void gpu_fill(lv_color_t* dest, uint32_t length, lv_color_t color) = 0;

#endif

	protected:

		/** Underlying display driver instance */
		DisplayDriver* _driver;

		/** Ticker for updating LittleVGL ticker */
		mbed::Ticker _ticker;

		/** Display driver C structure */
		lv_disp_drv_t _disp_drv_instance;

};



#endif /* LVGL_DRIVERS_LITTLEVGL_H_ */
