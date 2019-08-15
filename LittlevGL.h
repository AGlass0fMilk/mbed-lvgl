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

#if MBED_CONF_FILESYSTEM_PRESENT && USE_LV_FILESYSTEM
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

#if MBED_CONF_FILESYSTEM_PRESENT && USE_LV_FILESYSTEM
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

	protected:

		/** Initialized flag */
		bool initialized;

		/** Ticker for updating LittleVGL ticker */
		mbed::Ticker ticker;

#if MBED_CONF_FILESYSTEM_PRESENT && LV_USE_FILESYSTEM
		/** Filesystem driver */
		lv_fs_drv_t _fs_drv;
#endif

};



#endif /* LVGL_DRIVERS_LITTLEVGL_H_ */
