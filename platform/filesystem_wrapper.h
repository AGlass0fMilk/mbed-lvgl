/**
 * @file filesystem_wrapper.h
 * @brief Filesystem wrapper commands for lvgl API
 *
 * This header provides wrapper functions that translate
 * mbed's retargeted filesystem/stdio to the API used by
 * lvgl. This allows you to load images from external
 * external non-volatile storage, among other things
 *
 *  Created on: Aug 1, 2018
 *      Author: gdbeckstein
 */
#ifndef MBED_LVGL_FILESYSTEM_WRAPPER_H_
#define MBED_LVGL_FILESYSTEM_WRAPPER_H_

#if MBED_CONF_FILESYSTEM_PRESENT && LV_USE_FILESYSTEM

#ifdef __cplusplus
extern "C" {
#endif

#include "lv_fs.h"

/**
 * Sets up the default mbed fileystem wrapper structure for lvgl
 * @param[in/out] driver driver instance to configure to defaults
 */
void mbed_lvgl_fs_wrapper_default(lv_fs_drv_t* fs_drv);

/**
 * Open a file using mbed's retargeted filesystem
 * @param file_p pointer to a FILE* variable
 * @param fn name of the file.
 * @param mode element of 'fs_mode_t' enum or its 'OR' connection (e.g. FS_MODE_WR | FS_MODE_RD)
 * @return LV_FS_RES_OK: no error, the file is opened
 *         any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fs_wrapper_open(lv_fs_drv_t* fs_drv, void* file_p, const char* fn, lv_fs_mode_t mode);

/**
 * Close an opened file
 * @param file_p pointer to a FILE* variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fs_wrapper_close(lv_fs_drv_t* fs_drv, void* file_p);

/**
 * Read data from an opened file
 * @param file_p pointer to a FILE variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fs_wrapper_read(lv_fs_drv_t* fs_drv, void* file_p, void* buf, uint32_t btr, uint32_t* br);

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param file_p pointer to a FILE* variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fs_wrapper_seek(lv_fs_drv_t* fs_drv, void* file_p, uint32_t pos);

/**
 * Give the position of the read write pointer
 * @param file_p pointer to a FILE* variable.
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fs_wrapper_tell(lv_fs_drv_t* fs_drv, void* file_p, uint32_t* pos_p);

#ifdef __cplusplus
}
#endif

#endif /* MBED_CONF_FILESYSTEM_PRESENT */

#endif /* MBED_LVGL_FILESYSTEM_WRAPPER_H_ */
