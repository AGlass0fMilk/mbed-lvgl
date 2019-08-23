/* LittlevGL for Mbed-OS library
 * Copyright (c) 2018-2019 George "AGlass0fMilk" Beckstein
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "LittlevGL.h"

#include "platform/mbed_assert.h"
#include "platform/mbed_debug.h"
#include "platform/Callback.h"

LittlevGL::LittlevGL() :
		initialized(false), ticker()
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

	// This class's flush implementation delegates to the correct display driver instance
	disp_drv.flush_cb = &LittlevGL::flush;

#if USE_LV_GPU

	disp_drv.gpu_blend_cb = &LittlevGL::gpu_blend;
	disp_drv.gpu_fill_cb = &LittlevGL::gpu_fill;

#endif

	if(driver.has_rounder()) {
		disp_drv.rounder_cb = &LittlevGL::round_lv_area;
	}

	if(driver.has_pix_write_func()) {
		disp_drv.set_px_cb = &LittlevGL::set_pixel;
	}

#if MBED_CONF_MBED_LVGL_ENABLE_FLUSH_MONITORING
	disp_drv.monitor_cb = &LittlevGL::monitor;
#endif

	lv_disp_t * disp;
	disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/

	/** Make sure no issues happened */
	MBED_ASSERT(disp != NULL);

	driver.set_lv_disp_obj(disp);

}

void LittlevGL::set_default_display(LVGLDisplayDriver& driver) {
	lv_disp_t* disp = driver.get_lv_disp_obj();
	MBED_ASSERT(disp != NULL);
	lv_disp_set_default(disp);
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
		lv_fs_drv_t fs_drv;
		// Initialize and register the filesystem driver
		lv_fs_drv_init(&fs_drv);
		mbed_lvgl_fs_wrapper_default(&fs_drv);
		fs_drv.letter = 'M';
		lv_fs_drv_register(&fs_drv);
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
	// Retrieve the C++ display driver instance (stored in user_data)
	LVGLDisplayDriver* driver = (LVGLDisplayDriver*)(disp_drv->user_data);
	MBED_ASSERT(driver != NULL);

	// Call the driver's flush function
	driver->flush(disp_drv, area, color_p);

	// Tell lvgl flush is done
	lv_disp_flush_ready(disp_drv);
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

void LittlevGL::round_lv_area(lv_disp_drv_t* disp_drv, lv_area_t* area) {
	// Retrieve the C++ display driver instance (stored in user_data)
	LVGLDisplayDriver* driver = (LVGLDisplayDriver*)(disp_drv->user_data);
	MBED_ASSERT(driver != NULL);

	driver->round_lv_area(disp_drv, area);

}

void LittlevGL::set_pixel(lv_disp_drv_t* disp_drv, uint8_t* buf,
		lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color,
		lv_opa_t opa) {
	// Retrieve the C++ display driver instance (stored in user_data)
	LVGLDisplayDriver* driver = (LVGLDisplayDriver*)(disp_drv->user_data);
	MBED_ASSERT(driver != NULL);

	driver->set_pixel(disp_drv, buf, buf_w, x, y, color, opa);
}

void LittlevGL::monitor(lv_disp_drv_t* disp_drv, uint32_t time, uint32_t px) {
#if MBED_CONF_MBED_LVGL_ENABLE_FLUSH_MONITORING
	// Retrieve the C++ display driver instance (stored in user_data)
	LVGLDisplayDriver* driver = (LVGLDisplayDriver*)(disp_drv->user_data);
	MBED_ASSERT(driver != NULL);

	driver->monitor(disp_drv, time, px);
#endif
}

