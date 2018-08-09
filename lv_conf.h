/**
 * @file lv_conf.h
 * 
 * Modified to use mbed's compile-time configuration system
 *
 */

#ifndef LV_CONF_H
#define LV_CONF_H

/*----------------
 * Dynamic memory
 *----------------*/

/* Memory size which will be used by the library
 * to store the graphical objects and other data */
#define LV_MEM_CUSTOM      1                /*1: use custom malloc/free, 0: use the built-in lv_mem_alloc/lv_mem_free*/
#if LV_MEM_CUSTOM == 0
#define LV_MEM_SIZE    (32U * 1024U)        /*Size memory used by `lv_mem_alloc` in bytes (>= 2kB)*/
#define LV_MEM_ATTR                         /*Complier prefix for big array declaration*/
#define LV_MEM_AUTO_DEFRAG  1               /*Automatically defrag on free*/
#else       /*LV_MEM_CUSTOM*/
#define LV_MEM_CUSTOM_INCLUDE <stdlib.h>   /*Header for the dynamic memory function*/
#define LV_MEM_CUSTOM_ALLOC   malloc       /*Wrapper to malloc*/
#define LV_MEM_CUSTOM_FREE    free         /*Wrapper to free*/
#endif     /*LV_MEM_CUSTOM*/

/*===================
   Graphical settings
 *===================*/

/* Horizontal and vertical resolution of the library.*/
#ifndef LV_HOR_RES
#define LV_HOR_RES          (320)
#endif

#ifndef LV_VER_RES
#define LV_VER_RES          (240)
#endif

#ifndef LV_DPI
#define LV_DPI              100
#endif

/* Size of VDB (Virtual Display Buffer: the internal graphics buffer).
 * Required for buffered drawing, opacity and anti-aliasing
 * VDB makes the double buffering, you don't need to deal with it!
 * Typical size: ~1/10 screen */
#ifndef LV_VDB_SIZE
#define LV_VDB_SIZE         (20 * LV_HOR_RES)  /*Size of VDB in pixel count (1/10 screen size is good for first)*/
#endif

#ifndef LV_VDB_ADR
#define LV_VDB_ADR          0                  /*Place VDB to a specific address (e.g. in external RAM) (0: allocate automatically into RAM)*/
#endif

/* Use two Virtual Display buffers (VDB) parallelize rendering and flushing (optional)
 * The flushing should use DMA to write the frame buffer in the background*/
#ifndef LV_VDB_DOUBLE
#define LV_VDB_DOUBLE       0       /*1: Enable the use of 2 VDBs*/
#endif

#ifndef LV_VDB2_ADR
#define LV_VDB2_ADR         0       /*Place VDB2 to a specific address (e.g. in external RAM) (0: allocate automatically into RAM)*/
#endif

/* Enable anti-aliasing (lines, and radiuses will be smoothed) */
#ifndef LV_ANTIALIAS
#define LV_ANTIALIAS        1       /*1: Enable anti-aliasing*/
#endif

/*Screen refresh settings*/
#ifndef LV_REFR_PERIOD
#define LV_REFR_PERIOD      50    /*Screen refresh period in milliseconds*/
#endif

#ifndef LV_INV_FIFO_SIZE
#define LV_INV_FIFO_SIZE    32    /*The average count of objects on a screen */
#endif

/*=================
   Misc. setting
 *=================*/

/*Input device settings*/
#ifndef LV_INDEV_READ_PERIOD
#define LV_INDEV_READ_PERIOD            50                     /*Input device read period in milliseconds*/
#endif

#ifndef LV_INDEV_POINT_MARKER
#define LV_INDEV_POINT_MARKER           0                      /*Mark the pressed points  (required: USE_LV_REAL_DRAW = 1)*/
#endif

#ifndef LV_INDEV_DRAG_LIMIT
#define LV_INDEV_DRAG_LIMIT             10                     /*Drag threshold in pixels */
#endif

#ifndef LV_INDEV_DRAG_THROW
#define LV_INDEV_DRAG_THROW             20                     /*Drag throw slow-down in [%]. Greater value means faster slow-down */
#endif

#ifndef LV_INDEV_LONG_PRESS_TIME
#define LV_INDEV_LONG_PRESS_TIME        400                    /*Long press time in milliseconds*/
#endif

#ifndef LV_INDEV_LONG_PRESS_REP_TIME
#define LV_INDEV_LONG_PRESS_REP_TIME    100                    /*Repeated trigger period in long press [ms] */
#endif

/*Color settings*/
#ifndef LV_COLOR_DEPTH
#define LV_COLOR_DEPTH     16                     /*Color depth: 1/8/16/24*/
#endif

#ifndef LV_COLOR_TRANSP
#define LV_COLOR_TRANSP    LV_COLOR_LIME          /*Images pixels with this color will not be drawn (with chroma keying)*/
#endif

/*Text settings*/
#ifndef LV_TXT_UTF8
#define LV_TXT_UTF8             1                /*Enable UTF-8 coded Unicode character usage */
#endif

#ifndef LV_TXT_BREAK_CHARS
#define LV_TXT_BREAK_CHARS     " ,.;:-_"         /*Can break texts on these chars*/
#endif

/*Graphics feature usage*/
#ifndef USE_LV_ANIMATION
#define USE_LV_ANIMATION        1               /*1: Enable all animations*/
#endif

#ifndef USE_LV_SHADOW
#define USE_LV_SHADOW           1               /*1: Enable shadows*/
#endif

#ifndef USE_LV_GROUP
#define USE_LV_GROUP            1               /*1: Enable object groups (for keyboards)*/
#endif

#ifndef USE_LV_GPU
#define USE_LV_GPU              0               /*1: Enable GPU interface*/
#endif

#ifndef USE_LV_REAL_DRAW
#define USE_LV_REAL_DRAW        0               /*1: Enable function which draw directly to the frame buffer instead of VDB (required if LV_VDB_SIZE = 0)*/
#endif

#ifndef USE_LV_FILESYSTEM
#define USE_LV_FILESYSTEM       1               /*1: Enable file system (required by images*/
#endif

/*Compiler settings*/
//#ifndef LV_ATTRIBUTE_TICK_INC
//#define LV_ATTRIBUTE_TICK_INC                 /* Define a custom attribute to `lv_tick_inc` function */
//#endif
//
//#ifndef LV_ATTRIBUTE_TASK_HANDLER
//#define LV_ATTRIBUTE_TASK_HANDLER             /* Define a custom attribute to `lv_task_handler` function */
//#endif

#ifndef LV_COMPILER_VLA_SUPPORTED
#define LV_COMPILER_VLA_SUPPORTED    1        /* 1: Variable length array is supported*/
#endif

/*================
 *  THEME USAGE
 *================*/

#ifndef USE_LV_THEME_TEMPL
#define USE_LV_THEME_TEMPL		0	/*Just for test*/
#endif

#ifndef USE_LV_THEME_DEFAULT
#define USE_LV_THEME_DEFAULT	1	/*Built mainly from the built-in styles. Consumes very few RAM*/
#endif

#ifndef USE_LV_THEME_ALIEN
#define USE_LV_THEME_ALIEN		1	/*Dark futuristic theme*/
#endif

#ifndef USE_LV_THEME_NIGHT
#define USE_LV_THEME_NIGHT		1	/*Dark elegant theme*/
#endif

#ifndef USE_LV_THEME_MONO
#define USE_LV_THEME_MONO		1	/*Mono color theme for monochrome displays*/
#endif

#ifndef USE_LV_THEME_MATERIAL
#define USE_LV_THEME_MATERIAL	1	/*Flat theme with bold colors and light shadows*/
#endif

#ifndef USE_LV_THEME_ZEN
#define USE_LV_THEME_ZEN		1	/*Peaceful, mainly light theme */
#endif

/*==================
 *    FONT USAGE
 *===================*/

/* More info about fonts: https://littlevgl.com/basics#fonts
 * To enable a built-in font use 1,2,4 or 8 values
 * which will determine the bit-per-pixel */
#define LV_FONT_DEFAULT        &lv_font_dejavu_20     /*Always set a default font from the built-in fonts*/

#ifndef USE_LV_FONT_DEJAVU_10
#define USE_LV_FONT_DEJAVU_10              0
#endif

#ifndef USE_LV_FONT_DEJAVU_10_LATIN_SUP
#define USE_LV_FONT_DEJAVU_10_LATIN_SUP    0
#endif

#ifndef USE_LV_FONT_DEJAVU_10_CYRILLIC
#define USE_LV_FONT_DEJAVU_10_CYRILLIC     0
#endif

#ifndef USE_LV_FONT_SYMBOL_10
#define USE_LV_FONT_SYMBOL_10              0
#endif

#ifndef USE_LV_FONT_DEJAVU_20
#define USE_LV_FONT_DEJAVU_20              4
#endif

#ifndef USE_LV_FONT_DEJAVU_20_LATIN_SUP
#define USE_LV_FONT_DEJAVU_20_LATIN_SUP    0
#endif

#ifndef USE_LV_FONT_DEJAVU_20_CYRILLIC
#define USE_LV_FONT_DEJAVU_20_CYRILLIC     0
#endif

#ifndef USE_LV_FONT_SYMBOL_20
#define USE_LV_FONT_SYMBOL_20              4
#endif

#ifndef USE_LV_FONT_DEJAVU_30
#define USE_LV_FONT_DEJAVU_30              0
#endif

#ifndef USE_LV_FONT_DEJAVU_30_LATIN_SUP
#define USE_LV_FONT_DEJAVU_30_LATIN_SUP    0
#endif

#ifndef USE_LV_FONT_DEJAVU_30_CYRILLIC
#define USE_LV_FONT_DEJAVU_30_CYRILLIC     0
#endif

#ifndef USE_LV_FONT_SYMBOL_30
#define USE_LV_FONT_SYMBOL_30              0
#endif

#ifndef USE_LV_FONT_DEJAVU_40
#define USE_LV_FONT_DEJAVU_40              0
#endif

#ifndef USE_LV_FONT_DEJAVU_40_LATIN_SUP
#define USE_LV_FONT_DEJAVU_40_LATIN_SUP    0
#endif

#ifndef USE_LV_FONT_DEJAVU_40_CYRILLIC
#define USE_LV_FONT_DEJAVU_40_CYRILLIC     0
#endif

#ifndef USE_LV_FONT_SYMBOL_40
#define USE_LV_FONT_SYMBOL_40              0
#endif

/*===================
 *  LV_OBJ SETTINGS
 *==================*/
#define LV_OBJ_FREE_NUM_TYPE    uint32_t    /*Type of free number attribute (comment out disable free number)*/
#define LV_OBJ_FREE_PTR         1           /*Enable the free pointer attribute*/

/*==================
 *  LV OBJ X USAGE 
 *================*/
/*
 * Documentation of the object types: https://littlevgl.com/object-types
 */

/*****************
 * Simple object
 *****************/

/*Label (dependencies: -*/

#ifndef USE_LV_LABEL
#define USE_LV_LABEL    1
#endif

#if USE_LV_LABEL != 0
#define LV_LABEL_SCROLL_SPEED       25     /*Hor, or ver. scroll speed [px/sec] in 'LV_LABEL_LONG_SCROLL/ROLL' mode*/
#endif

/*Image (dependencies: lv_label*/
#ifndef USE_LV_IMG
#define USE_LV_IMG      1
#endif

/*Line (dependencies: -*/
#ifndef USE_LV_LINE
#define USE_LV_LINE     1
#endif

/*******************
 * Container objects
 *******************/

/*Container (dependencies: -*/
#ifndef USE_LV_CONT
#define USE_LV_CONT     1
#endif

/*Page (dependencies: lv_cont)*/
#ifndef USE_LV_PAGE
#define USE_LV_PAGE     1
#endif

/*Window (dependencies: lv_cont, lv_btn, lv_label, lv_img, lv_page)*/
#ifndef USE_LV_WIN
#define USE_LV_WIN      1
#endif

/*Tab (dependencies: lv_page, lv_btnm)*/
#ifndef USE_LV_TABVIEW
#define USE_LV_TABVIEW      1
#endif

#if USE_LV_TABVIEW != 0
#define LV_TABVIEW_ANIM_TIME    300     /*Time of slide animation [ms] (0: no animation)*/
#endif

/*************************
 * Data visualizer objects
 *************************/

/*Bar (dependencies: -)*/
#ifndef USE_LV_BAR
#define USE_LV_BAR      1
#endif

/*Line meter (dependencies: *;)*/
#ifndef USE_LV_LMETER
#define USE_LV_LMETER   1
#endif

/*Gauge (dependencies:bar, lmeter)*/
#ifndef USE_LV_GAUGE
#define USE_LV_GAUGE    1
#endif

/*Chart (dependencies: -)*/
#ifndef USE_LV_CHART
#define USE_LV_CHART    1
#endif

/*LED (dependencies: -)*/
#ifndef USE_LV_LED
#define USE_LV_LED      1
#endif

/*Message box (dependencies: lv_rect, lv_btnm, lv_label)*/
#ifndef USE_LV_MBOX
#define USE_LV_MBOX     1
#endif

/*Text area (dependencies: lv_label, lv_page)*/
#ifndef USE_LV_TA
#define USE_LV_TA       1
#endif

#if USE_LV_TA != 0
#define LV_TA_CURSOR_BLINK_TIME 400     /*ms*/
#define LV_TA_PWD_SHOW_TIME     1500    /*ms*/
#endif

/*************************
 * User input objects
 *************************/

/*Button (dependencies: lv_cont*/
#ifndef USE_LV_BTN
#define USE_LV_BTN      1
#endif

/*Button matrix (dependencies: -)*/
#ifndef USE_LV_BTNM
#define USE_LV_BTNM     1
#endif

/*Keyboard (dependencies: lv_btnm)*/
#ifndef USE_LV_KB
#define USE_LV_KB       1
#endif

/*Check box (dependencies: lv_btn, lv_label)*/
#ifndef USE_LV_CB
#define USE_LV_CB       1
#endif

/*List (dependencies: lv_page, lv_btn, lv_label, (lv_img optionally for icons ))*/
#ifndef USE_LV_LIST
#define USE_LV_LIST     1
#endif

#if USE_LV_LIST != 0
#define LV_LIST_FOCUS_TIME  100 /*Default animation time of focusing to a list element [ms] (0: no animation)  */
#endif

/*Drop down list (dependencies: lv_page, lv_label)*/
#ifndef USE_LV_DDLIST
#define USE_LV_DDLIST    1
#endif

#if USE_LV_DDLIST != 0
#define LV_DDLIST_ANIM_TIME     200     /*Open and close default animation time [ms] (0: no animation)*/
#endif

/*Roller (dependencies: lv_ddlist)*/
#ifndef USE_LV_ROLLER
#define USE_LV_ROLLER    1
#endif

#if USE_LV_ROLLER != 0
#define LV_ROLLER_ANIM_TIME     200     /*Focus animation time [ms] (0: no animation)*/
#endif

/*Slider (dependencies: lv_bar)*/
#ifndef USE_LV_SLIDER
#define USE_LV_SLIDER    1
#endif

/*Switch (dependencies: lv_slider)*/
#ifndef USE_LV_SW
#define USE_LV_SW       1
#endif

/*************************
 * Non-user section
 *************************/
#ifdef _MSC_VER                               /* Disable warnings for Visual Studio*/
# define _CRT_SECURE_NO_WARNINGS
#endif

#endif /*LV_CONF_H*/

