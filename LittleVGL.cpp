/**
 * @file LittleVGL.cpp
 * @brief Mbed OS port of LittleVGL graphics library - C++ driver
 *
 *  Created on: Jun 14, 2018
 *      Author: gdbeckstein
 */

#include "LittleVGL.h"

#include "platform/mbed_assert.h"
#include "platform/mbed_debug.h"
#include "platform/Callback.h"

LittleVGL::LittleVGL() :
		_inited(false), _driver(NULL), _ticker()
{ }

LittleVGL::~LittleVGL()
{ }

LittleVGL& LittleVGL::get_instance(void) {
	static LittleVGL* singleton;
	if(singleton == NULL) {
		singleton = new LittleVGL();
	}
	return *singleton;
}

void LittleVGL::init(LVGLDriver* driver)
{
	// Keep a reference to the driver
	_driver = driver;

	// Initialize LittlevGL
	lv_init();

	// Initialize and register the display driver
	lv_disp_drv_init(&_disp_drv_instance);
	_disp_drv_instance.disp_flush	= &LittleVGL::flush;

	_disp_drv_instance.disp_fill 	= &LittleVGL::fill;
	_disp_drv_instance.disp_map 	= &LittleVGL::map;

#if USE_LV_GPU

	_disp_drv_instance.mem_blend 	= &LittleVGL::gpu_blend;
	_disp_drv_instance.mem_fill 	= &LittleVGL::gpu_fill;

#endif

#if LV_VDB_SIZE

	if(_driver->has_vdb_func()) {
		_disp_drv_instance.vdb_wr = &LittleVGL::vdb_write;
	}

#endif

	lv_disp_drv_register(&_disp_drv_instance);

	_inited = true;

}

void LittleVGL::start(void)
{
	_ticker.attach_us(mbed::callback(this, &LittleVGL::tick), 1000);
}

void LittleVGL::stop(void)
{
	_ticker.detach();
}

void LittleVGL::update(void)
{
	lv_task_handler();
}

#if MBED_CONF_FILESYSTEM_PRESENT && USE_LV_FILESYSTEM
void LittleVGL::filesystem_ready(void)
{
	if(_inited)
	{
		// Initialize and register the filesystem driver
		memset(&_fs_drv, 0, sizeof(lv_fs_drv_t));
		mbed_lvgl_fs_wrapper_default(&_fs_drv);
		_fs_drv.letter = 'M';
		lv_fs_add_drv(&_fs_drv);
	}
	else
	{
		debug("littlevgl: filesystem cannot be initialized before LittleVGL\n");
	}

}
#endif

void LittleVGL::tick(void)
{
	lv_tick_inc(1);
}

void LittleVGL::flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p)
{
	LittleVGL& instance = LittleVGL::get_instance();
	if(instance._driver != NULL) {
		instance._driver->flush(x1, y1, x2, y2, color_p);
	}

	// Tell lvgl flush is done
	lv_flush_ready();
}

void LittleVGL::map(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p)
{
	LittleVGL& instance = LittleVGL::get_instance();
	if(instance._driver != NULL) {
		instance._driver->map(x1, y1, x2, y2, color_p);
	}
}

void LittleVGL::fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		lv_color_t color)
{
	LittleVGL& instance = LittleVGL::get_instance();
	if(instance._driver != NULL) {
		instance._driver->fill(x1, y1, x2, y2, color);
	}
}

#if USE_LV_GPU

void LittleVGL::gpu_blend(lv_color_t* dest, const lv_color_t* src, uint32_t length, lv_opt_t opa)
{
	LittleVGL& instance = LittleVGL::get_instance();
	if(instance._driver != NULL) {
		instance._driver->gpu_blend(dest, src, length, opa);
	}
}

void LittleVGL::gpu_fill(lv_color_t* dest, uint32_t length, lv_color_t color)
{NoritakeLVGL
	LittleVGL& instance = LittleVGL::get_instance();
	if(instance._driver != NULL) {
		instance._driver->gpu_fill(dest, length, color);
	}
}

#endif

#if LV_VDB_SIZE

void LittleVGL::vdb_write(uint8_t* buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
		lv_color_t color, lv_opa_t opa)
{
	LittleVGL& instance = LittleVGL::get_instance();
	if(instance._driver != NULL) {
		LVGLDriver* drv = instance._driver;
		drv->vdb_write(buf, buf_w, x, y, color, opa);
	}
}

#endif

