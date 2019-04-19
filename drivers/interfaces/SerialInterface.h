/*
 * SerialInterface.h
 *
 *  Created on: Oct 30, 2018
 *      Author: gdbeckstein
 */

#ifndef MBED_LVGL_DRIVERS_INTERFACES_SERIALINTERFACE_H_
#define MBED_LVGL_DRIVERS_INTERFACES_UARTINTERFACE_H_

// Defined if mbed target has this peripheral
#if defined(DEVICE_SERIAL)

#include "DisplayInterface.h"

#include "drivers/UARTSerial.h"

/**
 * Simple wrapper class to adapt RawSerial (UART) to a DisplayInterface
 */
class SerialInterface: public DisplayInterface, protected mbed::UARTSerial
{

public:

	/** Create a SerialInterface,
	 * connected to the specified transmit and receive pins,
	 * with the specified baud.
	 *
	 *  @param tx Transmit pin
	 *  @param rx Receive pin
	 *  @param baud The baud rate of the serial port (optional, defaults to MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
	 *
	 *  @note
	 *    Either tx or rx may be specified as NC if unused
	 */
	 SerialInterface(PinName tx, PinName rx,
			int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE,
			PinName rts = NC, PinName cts = NC) :
			mbed::UARTSerial(tx, rx, baud) {
		 if(rts != NC && cts != NC) {
			 this->set_flow_control(mbed::SerialBase::RTSCTS, rts, cts);
		 }
		 this->set_blocking(true);
	 }

	virtual ~SerialInterface(void) { }

#if MBED_USE_LVGL

	virtual void write_command(uint8_t command) { this->write(&command, 1); }

	virtual void write_command_with_params(uint8_t command, const uint8_t* params, uint8_t num_params) {
		this->write(&command, 1);
		this->write(params, num_params);
	}

	virtual void write_data(const uint8_t* data, uint32_t length) {
		this->write(data, length);
	}

	virtual void write_data(const uint16_t* data, uint32_t length) {
		// Cast to 8-bit pointer to save some code redundancy
		const uint8_t* ptr = (const uint8_t*) data;
		this->write_data(ptr, (length*sizeof(*data)));
	}

	virtual void write_data(const uint32_t* data, uint32_t length) {
		// Cast to 8-bit pointer to save some code redundancy
		const uint8_t* ptr = (const uint8_t*) data;
		this->write_data(ptr, (length*sizeof(*data)));
	}

	// You don't really read information from the display
	virtual uint8_t read(void) { return 0; }

#endif

};


#endif /** DEVICE_SERIAL */

#endif /* MBED_LVGL_DRIVERS_INTERFACES_SERIALINTERFACE_H_ */
