/**
 * @file filesystem_wrapper.c
 * @brief Filesystem wrapper functions for littlevgl on MBed-OS
 *
 * @copyright Copyright &copy; 2018 Quic
 *
 *  Created on: Aug 1, 2018
 *      Author: gdbeckstein
 */

#include "filesystem_wrapper.h"
#include "platform/mbed_retarget.h"
#include <stdio.h>

typedef FILE* file_ptr_t;

lv_fs_res_t lv_fs_wrapper_open(void* file_p, const char* fn, lv_fs_mode_t mode)
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

lv_fs_res_t lv_fs_wrapper_close(void* file_p)
{
	file_ptr_t *fp = file_p;         /*Just avoid the confusing castings*/
	fclose(*fp);
	return LV_FS_RES_OK;
}

lv_fs_res_t lv_fs_wrapper_read(void* file_p, void* buf, uint32_t btr, uint32_t* br)
{
	file_ptr_t *fp = file_p;         /*Just avoid the confusing castings*/
	*br = fread(buf, 1, btr, *fp);
	return LV_FS_RES_OK;
}

lv_fs_res_t lv_fs_wrapper_seek(void* file_p, uint32_t pos)
{
	file_ptr_t *fp = file_p;         /*Just avoid the confusing castings*/
	fseek(*fp, pos, SEEK_SET);
	return LV_FS_RES_OK;
}

lv_fs_res_t lv_fs_wrapper_tell(void* file_p, uint32_t* pos_p)
{
	file_ptr_t *fp = file_p;         /*Just avoid the confusing castings*/
	*pos_p = ftell(*fp);
	return LV_FS_RES_OK;
}

