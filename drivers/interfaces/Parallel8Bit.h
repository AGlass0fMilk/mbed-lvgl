/*
 * Parallel8Bit.h
 *
 *  Created on: Jul 16, 2018
 *      Author: gdbeckstein
 */

#ifndef DRIVERS_PARALLEL8BIT_H_
#define DRIVERS_PARALLEL8BIT_H_

#include "DisplayInterface.h"

#include "hal/i8080_8bit_api.h"
#include "drivers/DigitalOut.h"

#include "platform/mbed_error.h"

#if defined(DEVICE_PARALLEL_8BIT)

class Parallel8Bit : public DisplayInterface
{
public:

	Parallel8Bit(PinName data_cmd_pin);

	virtual ~Parallel8Bit(void) { }

	virtual void write_command(uint8_t command);

	/**
	 * Writes a command with parameters
	 * @param[in] command command byte to send
	 * @param[in] params Pointer to parameter buffer
	 * @param[in] num_params number of parameter bytes to send
	 */
	virtual void write_command_with_params(uint8_t command, const uint8_t* params,
			uint8_t num_params) {
		// TODO - need to implement this for Parallel8Bit interface
	}

	/**
	 * Write data out in bytes
	 *
	 * @param[in] data Bytes to write out
	 * @param[in] length Number of bytes to write
	 */
	virtual void write_data(const uint8_t* data, uint32_t length);

	/**
	 * Write data out in shorts (16-bit)
	 *
	 * @param[in] data Data to write out
	 * @param[in] length Number of shorts (16-bit) to write out
	 */
	virtual void write_data(const uint16_t* data, uint32_t length);

	/**
	 * Write data out in words (32-bit)
	 *
	 * @param[in] data Data to write out
	 * @param[in] length Number of words (32-bit) to write out
	 */
	virtual void write_data(const uint32_t* data, uint32_t length);

	virtual uint8_t read(void);

protected:

	mbed::DigitalOut _data_cmd;

	i8080_8bit_t _bus;

};

#endif /* DEVICE_PARALLEL_8BIT */

#endif /* DRIVERS_PARALLEL8BIT_H_ */
