/**
 * @file filesystem_wrapper.h
 * @brief Brief Description
 * 
 * Detailed Description
 *
 * Link to [markdown like this](@ref PageTag)
 * Make sure you tag the markdown page like this:
 * # Page title {#PageTag}
 * 
 * <a href='MyPDF.pdf'> Link to PDF documents like this</a>
 * If you add document files, make sure to add them into a directory inside a "docs" folder
 * And then run hud-devices/tools/copy-dox-files.py 
 *
 * To use images, make sure they're in an "images" folder and follow the doxygen user manual to add images.
 * You must run copy-dox-files.py after adding images as well.
 *
 * @copyright Copyright &copy; 2018 Quic
 *
 *  Created on: Aug 1, 2018
 *      Author: gdbeckstein
 */
#ifndef QUIC_FILESYSTEM_WRAPPER_H_
#define QUIC_FILESYSTEM_WRAPPER_H_

#include "lv_fs.h"

#ifdef _cplusplus
extern "C" {
#endif

/**
 * Open a file using mbed's retargeted filesystem
 * @param file_p pointer to a FILE* variable
 * @param fn name of the file.
 * @param mode element of 'fs_mode_t' enum or its 'OR' connection (e.g. FS_MODE_WR | FS_MODE_RD)
 * @return LV_FS_RES_OK: no error, the file is opened
 *         any error from lv_fs_res_t enum
 */
lv_fs_res_t lv_fs_wrapper_open(void* file_p, const char* fn, lv_fs_mode_t mode);

/**
 * Close an opened file
 * @param file_p pointer to a FILE* variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fs_wrapper_close(void* file_p);

/**
 * Read data from an opened file
 * @param file_p pointer to a FILE variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fs_wrapper_read(void* file_p, void* buf, uint32_t btr, uint32_t* br);

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param file_p pointer to a FILE* variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fs_wrapper_seek(void* file_p, uint32_t pos);

/**
 * Give the position of the read write pointer
 * @param file_p pointer to a FILE* variable.
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv__fs_res_t enum
 */
lv_fs_res_t lv_fs_wrapper_tell(void* file_p, uint32_t* pos_p);

#ifdef _cplusplus
}
#endif

#endif /* QUIC_FILESYSTEM_WRAPPER_H_ */
