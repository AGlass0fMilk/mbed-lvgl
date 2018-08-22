/*
 * assets.h
 *
 *  Created on: Aug 22, 2018
 *      Author: gdbeckstein
 * Copyright Quic 2018
 */

#ifndef GUI_MBED_LVGL_BUILT_IN_H_
#define GUI_MBED_LVGL_BUILT_IN_H_

// Add built-in fonts to the fonts namespace
// TODO add the rest of them
namespace fonts {

extern "C" {

#if USE_LV_FONT_DEJAVU_10
	extern lv_font_t lv_font_dejavu_10;
#endif

#ifdef USE_LV_FONT_SYMBOL_10
	extern lv_font_t lv_font_symbol_10;
#endif

#if USE_LV_FONT_DEJAVU_20
	extern lv_font_t lv_font_dejavu_20;
#endif

#if USE_LV_FONT_SYMBOL_20
	extern lv_font_t lv_font_symbol_20;
#endif

#if USE_LV_FONT_DEJAVU_30
	extern lv_font_t lv_font_dejavu_30;
#endif

#if USE_LV_FONT_SYMBOL_30
	extern lv_font_t lv_font_symbol_30;
#endif

#if USE_LV_FONT_DEJAVU_40
	extern lv_font_t lv_font_dejavu_40;
#endif

#if USE_LV_FONT_SYMBOL_40
	extern lv_font_t lv_font_symbol_40;
#endif

}

}



#endif /* GUI_MBED_LVGL_BUILT_IN_H_ */
