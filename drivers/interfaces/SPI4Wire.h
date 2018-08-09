/**
 * @file SPI4Wire.h
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
 * @copyright Copyright &copy; 2018 Heads Up Display, Inc.
 *
 *  Created on: Jul 26, 2018
 *      Author: gdbeckstein
 */
#ifndef MBED_LVGL_DRIVERS_INTERFACES_SPI4WIRE_H_
#define MBED_LVGL_DRIVERS_INTERFACES_SPI4WIRE_H_

#include "DisplayInterface.h"

#include "drivers/SPI.h"
#include "drivers/DigitalOut.h"

#if defined(DEVICE_SPI)

/** Low logic level on D/C pin means command */
#define SPI4WIRE_COMMAND_LOGIC_LEVEL 	0

/** High logic level on D/C pin means data */
#define SPI4WIRE_DATA_LOGIC_LEVEL		1

/**
 * 4-wire SPI display interface driver
 * This type of interface is supported by all mbed targets
 * that support SPI and GPIO digital outputs (almost universal).
 * That means it does not need a HAL interface
 */
class SPI4Wire : public DisplayInterface
{
	public:

		/**
		 * Instantiate a 4-wire SPI display interface
		 * @note This constructor does not allow a shared SPI bus
		 *
		 * @param[in] mosi MOSI pin for interface
		 * @param[in] sclk SCLK pin for interface
		 * @param[in] cs Chip select pin for interface
		 * @param[in] dc Data/Command pin for interface
		 */
		SPI4Wire(PinName mosi, PinName sclk, PinName cs, PinName dc) :
			_chip_select(cs, 1), _data_command(dc, 0), _shared_bus(false)
		{
			_spi = new mbed::SPI(mosi, NC, sclk, NC);
		}

		/**
		 * Instantiate a 4-wire SPI display interface
		 * @note This constructor allows a shared SPI bus.
		 * Not recommended for maximum drawing rate.
		 *
		 * @param[in] spi Shared SPI bus handle
		 * @param[in] cs Chip select pin for interface
		 * @param[in] dc Data/Command pin for interface
		 */
		SPI4Wire(mbed::SPI* spi, PinName cs, PinName dc) :
			_chip_select(cs, 1), _data_command(dc, 0), _shared_bus(true)
		{
			_spi = spi;
		}

		virtual ~SPI4Wire(void)
		{
			// If it's an unshared bus then we instantiated the driver
			// So we are responsible for deleting it
			if(!_shared_bus && _spi)
			{
				delete _spi;
				_spi = NULL;
			}
		}

		/**
		 * Sets the frequency of the underlying SPI interface
		 */
		void frequency(int hz)
		{
			_spi->frequency(hz);
		}

		/**
		 * Writes a command byte to the display interface
		 */
		virtual void write_command(uint8_t command)
		{
			_chip_select = 0;
			_data_command = SPI4WIRE_COMMAND_LOGIC_LEVEL;
			_spi->write(command);
			_chip_select = 1;
		}

		/**
		 * Writes a command with parameters
		 * @param[in] command command byte to send
		 * @param[in] params Pointer to parameter buffer
		 * @param[in] num_params number of parameter bytes to send
		 */
		virtual void write_command_with_params(uint8_t command, const uint8_t* params,
				uint8_t num_params)
		{
			_chip_select = 0;
			_data_command = SPI4WIRE_COMMAND_LOGIC_LEVEL;
			_spi->write(command);
			_data_command = SPI4WIRE_DATA_LOGIC_LEVEL;
			_spi->write((const char*) params, num_params, NULL, 0);
			_chip_select = 1;
		}

		/**
		 * Write data out in bytes
		 *
		 * @param[in] data Bytes to write out
		 * @param[in] length Number of bytes to write
		 */
		virtual void write_data(const uint8_t* data, uint32_t length)
		{
			_chip_select = 0;
			_data_command = SPI4WIRE_DATA_LOGIC_LEVEL;
			_spi->write((const char*) data, length, NULL, 0);
			_chip_select = 1;
		}

		/**
		 * Write data out in shorts (16-bit)
		 *
		 * @param[in] data Data to write out
		 * @param[in] length Number of shorts (16-bit) to write out
		 */
		virtual void write_data(const uint16_t* data, uint32_t length)
		{
			this->write_data((const uint8_t*) data, (length << 1));
		}


		/**
		 * Write data out in words (32-bit)
		 *
		 * @param[in] data Data to write out
		 * @param[in] length Number of words (32-bit) to write out
		 */
		virtual void write_data(const uint32_t* data, uint32_t length)
		{
			this->write_data((const uint8_t*) data, (length << 2));
		}

		/**
		 * Read a byte from the interface
		 *
		 * @note not supported by 4-wire SPI
		 */
		virtual uint8_t read(void) { return 0; }

	protected:

		/** Interface SPI bus handle */
		mbed::SPI* _spi;

		/** Chip select output */
		mbed::DigitalOut _chip_select;

		/** Data/Command output */
		mbed::DigitalOut _data_command;

		/** Indicates if the SPI bus is shared */
		const bool _shared_bus;

};

#endif



#endif /* MBED_LVGL_DRIVERS_INTERFACES_SPI4WIRE_H_ */
