/**
 * @file LittleVGL.h
 * @brief Mbed OS port of LittleVGL graphics library - C++ driver
 *
 *  Created on: Jun 14, 2018
 *      Author: gdbeckstein
 */
#ifndef LVGL_LITTLEVGL_H_

#define LVGL_LITTLEVGL_H_

#include <LVGLDisplayDriver.h>

#include "platform/NonCopyable.h"
#include "drivers/Ticker.h"

#include "lv_hal_disp.h"
#include "lv_task.h"
#include "lv_obj.h"

#if MBED_CONF_FILESYSTEM_PRESENT && LV_USE_FILESYSTEM
#include "platform/filesystem_wrapper.h"
#endif

class LittlevGL : private mbed::NonCopyable<LittlevGL>
{
	public:

		virtual ~LittlevGL();

		/**
		 * @brief Return a single instance of the class
		 *
		 * Due to underlying C implementation of LittleVGL, it is simpler
		 * to implement this API as a singleton. Also, for most cases it
		 * doesn't make much sense to have more than one display
		 *
		 * @retval Singleton instance reference
		 */
		static LittlevGL& get_instance();

		/**
		 * Initializes LittleVGL
		 */
		void init();

		/**
		 * Add a display driver to LittlevGL
		 *
		 * @param[in] driver Display driver instance to add
		 *
		 * @note May be called multiple times when several displays are used
		 */
		void add_display_driver(LVGLDisplayDriver& driver);

		/**
		 * Select the given display to be used in all lvgl
		 * object creation calls until a new display is selected
		 *
		 * @param[in] driver Display driver to be used
		 *
		 * @note The driver must already be added to LittlevGL before
		 * calling this or the program will crash
		 */
		void set_default_display(LVGLDisplayDriver& driver);

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

#if MBED_CONF_FILESYSTEM_PRESENT && LV_USE_FILESYSTEM
		/**
		 * Tells littlevgl that a filesystem is ready to use
		 * @note this MUST be called AFTER LittleVGL::init() or your program will halt!
		 */
		void filesystem_ready(void);
#endif

	protected:

		/**
		 * Internal ticker function
		 */
		void tick(void);

	private:

		/** Private constructor, as class is a singleton */
		LittlevGL();

		/*
		 * @brief Internal function for bridging C/C++ to DisplayDriver instance
		 */
		static void flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

#if USE_LV_GPU

		/*
		 * @brief Internal function for bridging C/C++ to DisplayDriver instance
		 */
		static void gpu_blend(lv_disp_drv_t * disp_drv, lv_color_t * dest, const lv_color_t * src, uint32_t length,
				 lv_opa_t opa);

		/*
		 * @brief Internal function for bridging C/C++ to DisplayDriver instance
		 */
		static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
				const lv_area_t * fill_area, lv_color_t color);

#endif

		/** OPTIONAL: Extend the invalidated areas to match with the display drivers requirements
		 * E.g. round `y` to, 8, 16 ..) on a monochrome display*/
		static void round_lv_area(lv_disp_drv_t * disp_drv, lv_area_t * area);

		/*Optional: Set a pixel in a buffer according to the requirements of the display*/
		static void set_pixel(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
				lv_color_t color, lv_opa_t opa);

		/** OPTIONAL: Called after every refresh cycle to tell the rendering and flushing time + the
		 * number of flushed pixels */
		static void monitor(lv_disp_drv_t * disp_drv, uint32_t time, uint32_t px);

	protected:

		/** Initialized flag */
		bool initialized;

		/** Ticker for updating LittleVGL ticker */
		mbed::Ticker ticker;

};



#endif /* LVGL_DRIVERS_LITTLEVGL_H_ */
