/**
 * @file filesystem_wrapper.c
 * @brief Filesystem wrapper functions for littlevgl on MBed-OS
 *
 * This provides wrapper functions that translate
 * mbed's retargeted filesystem/stdio to the API used by
 * lvgl. This allows you to load images from external
 * external non-volatile storage, among other things
 *  Created on: Aug 1, 2018
 *      Author: gdbeckstein
 */

#if MBED_CONF_FILESYSTEM_PRESENT && LV_USE_FILESYSTEM

#include "filesystem_wrapper.h"
#include "platform/mbed_retarget.h"
#include <stdio.h>

typedef FILE* file_ptr_t;

void mbed_lvgl_fs_wrapper_default(lv_fs_drv_t* fs_drv)
{
	// Set up the defaults for mbed-lvgl filesystem wrapper driver struct
	fs_drv->file_size	= sizeof(file_ptr_t);
	fs_drv->letter		= 'A';
	fs_drv->open_cb 		= lv_fs_wrapper_open;
	fs_drv->close_cb		= lv_fs_wrapper_close;
	fs_drv->read_cb 		= lv_fs_wrapper_read;
	fs_drv->seek_cb 		= lv_fs_wrapper_seek;
	fs_drv->tell_cb 		= lv_fs_wrapper_tell;
}

lv_fs_res_t lv_fs_wrapper_open(lv_fs_drv_t* fs_drv, void* file_p, const char* fn, lv_fs_mode_t mode)
{
	const char * flags = "";

	if(mode == LV_FS_MODE_WR) flags = "wb";
	else if(mode == LV_FS_MODE_RD) flags = "rb";
	else if(mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) flags = "a+";

	file_ptr_t f = fopen(fn, flags);
	if((long int)f <= 0) return LV_FS_RES_UNKNOWN;
	else {
	  fseek(f, 0, SEEK_SET);

	  /* 'file_p' is pointer to a file descriptor and
		* we need to store our file descriptor here*/
	  file_ptr_t *fp = file_p;         /*Just avoid the confusing castings*/
	  *fp = f;
	}

	return LV_FS_RES_OK;
}

lv_fs_res_t lv_fs_wrapper_close(lv_fs_drv_t* fs_drv, void* file_p)
{
	file_ptr_t *fp = file_p;         /*Just avoid the confusing castings*/
	fclose(*fp);
	return LV_FS_RES_OK;
}

lv_fs_res_t lv_fs_wrapper_read(lv_fs_drv_t* fs_drv, void* file_p, void* buf, uint32_t btr, uint32_t* br)
{
	file_ptr_t *fp = file_p;         /*Just avoid the confusing castings*/
	*br = fread(buf, 1, btr, *fp);
	return LV_FS_RES_OK;
}

lv_fs_res_t lv_fs_wrapper_seek(lv_fs_drv_t* fs_drv, void* file_p, uint32_t pos)
{
	file_ptr_t *fp = file_p;         /*Just avoid the confusing castings*/
	fseek(*fp, pos, SEEK_SET);
	return LV_FS_RES_OK;
}

lv_fs_res_t lv_fs_wrapper_tell(lv_fs_drv_t* fs_drv, void* file_p, uint32_t* pos_p)
{
	file_ptr_t *fp = file_p;         /*Just avoid the confusing castings*/
	*pos_p = ftell(*fp);
	return LV_FS_RES_OK;
}

#endif

