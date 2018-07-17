/*
 * DisplayInterface.h
 *
 * Interface for communicating with a display
 *
 * Allows the underlying transport (Parallel, I2C, SPI, etc)
 * to be changed without modifying the behavior of the
 * DisplayDriver using the interface
 *
 *  Created on: Jul 16, 2018
 *      Author: gdbeckstein
 */

#ifndef DRIVERS_DISPLAYINTERFACE_H_
#define DRIVERS_DISPLAYINTERFACE_H_

#include "stdint.h"

class DisplayInterface
{

public:

	virtual ~DisplayInterface(void) { }

#if MBED_USE_LVGL

	virtual void write_command(uint8_t command) = 0;

	virtual void write_data(uint8_t* data, uint32_t length) = 0;

	virtual uint8_t read(void) = 0;

#endif

};

#endif /* DRIVERS_DISPLAYINTERFACE_H_ */
