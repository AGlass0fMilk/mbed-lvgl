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

	/**
	 * Writes a command with parameters
	 * @param[in] command command byte to send
	 * @param[in] params Pointer to parameter buffer
	 * @param[in] num_params number of parameter bytes to send
	 */
	virtual void write_command_with_params(uint8_t command, const uint8_t* params,
			uint8_t num_params) = 0;

	virtual void write_data(const uint8_t* data, uint32_t length) = 0;

	virtual void write_data(const uint16_t* data, uint32_t length) = 0;

	virtual void write_data(const uint32_t* data, uint32_t length) = 0;

	virtual uint8_t read(void) = 0;

#endif

};

#endif /* DRIVERS_DISPLAYINTERFACE_H_ */
