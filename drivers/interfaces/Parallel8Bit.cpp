/*
 * Parallel8Bit.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: gdbeckstein
 */

#include "Parallel8Bit.h"

#if defined(DEVICE_PARALLEL_8BIT)

Parallel8Bit::Parallel8Bit(PinName data_cmd_pin) : _data_cmd(data_cmd_pin)
{
	i8080_8bit_init(&_bus);
}

void Parallel8Bit::write_command(uint8_t command) {
	_data_cmd = 0;
	i8080_8bit_write(&_bus, command);
}

void Parallel8Bit::write_data(const uint8_t* data, uint32_t length) {
	_data_cmd = 1;
	i8080_8bit_write_bytes(&_bus, data, length);
}

void Parallel8Bit::write_data(const uint16_t* data, uint32_t length) {
	_data_cmd = 1;
	i8080_8bit_write_int16(&_bus, data, length);
}

void Parallel8Bit::write_data(const uint32_t* data, uint32_t length) {
	_data_cmd = 1;
	i8080_8bit_write_word32(&_bus, data, length);
}

uint8_t Parallel8Bit::read(void) {
	uint8_t result;
	i8080_8bit_read(&_bus, &result, 1);
	return result;
}

#endif /* DEVICE_PARALLEL_8BIT */
