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

//#if defined(DEVICE_PARALLEL_8BIT)

class Parallel8Bit : public DisplayInterface
{
public:

	Parallel8Bit(PinName data_cmd_pin);

	virtual ~Parallel8Bit(void) { }

	virtual void write_command(uint8_t command);

	virtual void write_data(uint8_t* data, uint32_t length);

	virtual uint8_t read(void);

protected:

	mbed::DigitalOut _data_cmd;

	i8080_8bit_t _bus;

};

//#endif /* DEVICE_PARALLEL_8BIT */

#endif /* DRIVERS_PARALLEL8BIT_H_ */
