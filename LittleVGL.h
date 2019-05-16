/**
 * @file LittleVGL.h
 * @brief Mbed OS port of LittleVGL graphics library - C++ driver
 *
 *  Created on: Jun 14, 2018
 *      Author: gdbeckstein
 */
#ifndef LVGL_LITTLEVGL_H_
#define LVGL_LITTLEVGL_H_

#include "LVGLDriver.h"

#include "platform/NonCopyable.h"
#include "drivers/Ticker.h"

extern "C" {
#include "lv_hal_disp.h"
#include "lv_task.h"
#include "lv_obj.h"
}

#if MBED_CONF_FILESYSTEM_PRESENT && USE_LV_FILESYSTEM
#include "platform/filesystem_wrapper.h"
#endif

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
		static LittleVGL& get_instance();

		/**
		 * Initializes LittleVGL
		 * @param[in] driver Display driver instance to use
		 */
		void init(LVGLDriver* driver);

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

#if LV_VDB_SIZE

		static void vdb_write(uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
						lv_color_t color, lv_opa_t opa);

#endif

	protected:

		/** Initialized flag */
		bool _inited;

		/** Underlying display driver instance */
		LVGLDriver* _driver;

		/** Ticker for updating LittleVGL ticker */
		mbed::Ticker _ticker;

		/** Display driver C structure */
		lv_disp_drv_t _disp_drv_instance;

#if MBED_CONF_FILESYSTEM_PRESENT && USE_LV_FILESYSTEM
		/** Filesystem driver */
		lv_fs_drv_t _fs_drv;
#endif

};



#endif /* LVGL_DRIVERS_LITTLEVGL_H_ */
