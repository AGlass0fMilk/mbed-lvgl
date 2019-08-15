/**
#include <LittlevGL.h>
 * @file LittlevGL.cpp
 * @brief Mbed OS port of LittlevGL graphics library - C++ driver
 *
 *  Created on: Jun 14, 2018
 *      Author: gdbeckstein
 */

#include "LittlevGL.h"

#include "platform/mbed_assert.h"
#include "platform/mbed_debug.h"
#include "platform/Callback.h"

LittlevGL::LittlevGL() :
		initialized(false), _driver(NULL), ticker()
{ }

LittlevGL::~LittlevGL()
{ }

LittlevGL& LittlevGL::get_instance(void) {
	static LittlevGL* singleton;
	if(singleton == NULL) {
		singleton = new LittlevGL();
	}
	return *singleton;
}

void LittlevGL::init()
{
	// Initialize LittlevGL
	lv_init();
	initialized = true;
}

void LittlevGL::add_display_driver(LVGLDisplayDriver& driver) {

	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);

	// Set the resolution
	driver.get_resolution(&disp_drv.hor_res, &disp_drv.ver_res);

	// Set the display buffer(s)
	disp_drv.buffer = driver.get_lv_buf();  /*Set an initialized buffer*/

	// Store a pointer to the display driver C++ instance in the user data field
	disp_drv.user_data = (void*) &driver;

	disp_drv.flush_cb = &LittlevGL::flush;
	lv_disp_t * disp;
	disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/

}

void LittlevGL::start(void)
{
	ticker.attach_us(mbed::callback(this, &LittlevGL::tick), 1000);
}

void LittlevGL::stop(void)
{
	ticker.detach();
}

void LittlevGL::update(void)
{
	lv_task_handler();
}

#if MBED_CONF_FILESYSTEM_PRESENT && LV_USE_FILESYSTEM
void LittlevGL::filesystem_ready(void)
{
	if(initialized)
	{
		// Initialize and register the filesystem driver
		memset(&_fs_drv, 0, sizeof(lv_fs_drv_t));
		mbed_lvgl_fs_wrapper_default(&_fs_drv);
		_fs_drv.letter = 'M';
		lv_fs_add_drv(&_fs_drv);
	}
	else
	{
		debug("LittlevGL: filesystem cannot be initialized before LittlevGL\n");
	}

}
#endif

void LittlevGL::tick(void)
{
	lv_tick_inc(1);
}

void LittlevGL::flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
	LittlevGL& instance = LittlevGL::get_instance();
	if(instance._driver != NULL) {
		instance._driver->flush(x1, y1, x2, y2, color_p);
	}

	// Tell lvgl flush is done
	lv_flush_ready();
}

void LittlevGL::map(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p)
{
	LittlevGL& instance = LittlevGL::get_instance();
	if(instance._driver != NULL) {
		instance._driver->map(x1, y1, x2, y2, color_p);
	}
}

void LittlevGL::fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		lv_color_t color)
{
	LittlevGL& instance = LittlevGL::get_instance();
	if(instance._driver != NULL) {
		instance._driver->fill(x1, y1, x2, y2, color);
	}
}

#if USE_LV_GPU

void LittlevGL::gpu_blend(lv_color_t* dest, const lv_color_t* src, uint32_t length, lv_opt_t opa)
{
	LittlevGL& instance = LittlevGL::get_instance();
	if(instance._driver != NULL) {
		instance._driver->gpu_blend(dest, src, length, opa);
	}
}

void LittlevGL::gpu_fill(lv_color_t* dest, uint32_t length, lv_color_t color)
{NoritakeLVGL
	LittlevGL& instance = LittlevGL::get_instance();
	if(instance._driver != NULL) {
		instance._driver->gpu_fill(dest, length, color);
	}
}

#endif

#if LV_VDB_SIZE

void LittlevGL::vdb_write(uint8_t* buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
		lv_color_t color, lv_opa_t opa)
{
	LittlevGL& instance = LittlevGL::get_instance();
	if(instance._driver != NULL) {
		LVGLDisplayDriver* drv = instance._driver;
		drv->vdb_write(buf, buf_w, x, y, color, opa);
	}
}

#endif

