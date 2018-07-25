/**
 * @file LittleVGL.cpp
 * @brief Mbed OS port of LittleVGL graphics library - C++ driver
 *
 *  Created on: Jun 14, 2018
 *      Author: gdbeckstein
 */

#include "LittleVGL.h"

#include "platform/Callback.h"

/** Singleton display driver instance */
static DisplayDriver* display_driver_instance;

LittleVGL::LittleVGL() :
		_driver(NULL), _ticker()
{
	display_driver_instance = NULL;
}

LittleVGL::~LittleVGL()
{ }

void LittleVGL::init(DisplayDriver* driver)
{
	// Keep a reference to the driver
	_driver = driver;
	display_driver_instance = driver;

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

	lv_disp_drv_register(&_disp_drv_instance);

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

void LittleVGL::tick(void)
{
	lv_tick_inc(1);
}

void LittleVGL::flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p)
{
	if (display_driver_instance)
	{
		display_driver_instance->flush(x1, y1, x2, y2, color_p);
	}

	// Tell lvgl flush is done
	lv_flush_ready();
}

void LittleVGL::map(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		const lv_color_t* color_p)
{
	if (display_driver_instance)
	{
		display_driver_instance->map(x1, y1, x2, y2, color_p);
	}
}

void LittleVGL::fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,
		lv_color_t color)
{
	if (display_driver_instance)
	{
		display_driver_instance->fill(x1, y1, x2, y2, color);
	}
}

#if USE_LV_GPU

void LittleVGL::gpu_blend(lv_color_t* dest, const lv_color_t* src, uint32_t length, lv_opt_t opa)
{
	if(display_driver_instance)
	{
		display_driver_instance->gpu_blend(dest, src, length, opa);
	}
}

void LittleVGL::gpu_fill(lv_color_t* dest, uint32_t length, lv_color_t color)
{
	if(display_driver_instance)
	{
		display_driver_instance->gpu_fill(dest, length, color);
	}
}

#endif
