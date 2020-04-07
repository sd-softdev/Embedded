/*
 * OneWire.c
 *
 *  Created on: Apr 4, 2020
 *      Author: Daniel
 */

#include "DS_OneWire.h"

#include "DS_Delay.h"


void oneWire_readTemp(char *str){
	uint8_t scratch[9];
	oneWire_readScratchpad(scratch);

	uint16_t tr = scratch[1];
	tr = tr << 8;
	tr |= scratch[0];

	convert(str, &tr);
}

void oneWire_readScratchpad(uint8_t *scratch){
	oneWire_init();
	oneWire_writeByte(0xcc);
	oneWire_writeByte(0xbe);
//	oneWire_readByte();
//	oneWire_readByte();
//	oneWire_readByte();
//	oneWire_readByte();
//	oneWire_readByte();
//	oneWire_readByte();
//	oneWire_readByte();
//	oneWire_readByte();
//	oneWire_readByte();
	for(uint8_t idx = 0; idx < 9; idx++)
		scratch[idx] = oneWire_readByte();
}

uint8_t oneWire_readSlaveInfos(uint64_t *id, uint8_t *familyCode) {
//	uint8_t familyCode = 0x00;
//	uint64_t id = 0x00000000;
//	uint8_t crc = 0x00;

	oneWire_writeByte(ONEWIRE_CMD_READROM);
	*familyCode = oneWire_readByte();
	for(int idx = 0; idx < 6; idx++)
	{
		uint64_t tmp = oneWire_readByte();
		*id |= tmp << (8*idx);
	}

//	crc = oneWire_readByte();
	return 0;
}

void oneWire_writeBit(uint8_t bit)
{
	// set data-port as out
	oneWire_initDataPortAsWrite();

	// data-port = low
	HAL_GPIO_WritePin(ONEWIRE_DATA_GPIO_Port, ONEWIRE_DATA_Pin, GPIO_PIN_RESET);

	if (bit)
	{
		// port high
		//HAL_GPIO_WritePin(ONEWIRE_DATA_GPIO_Port, ONEWIRE_DATA_Pin, GPIO_PIN_SET);

		// port low
		// HAL_GPIO_WritePin(ONEWIRE_DATA_GPIO_Port, ONEWIRE_DATA_Pin, GPIO_PIN_RESET);

		// wait 15 us
		DS_Delay_us(8);

		// release data-port (set as inport)
		oneWire_initDataPortAsRead();

		// wait 45 us
		DS_Delay_us(48);

		// port low
		// oneWire_initDataPortAsWrite();
	} else
	{
		// wait 60 us
		DS_Delay_us(60);
		// release data-port (set as inport)
		oneWire_initDataPortAsRead();
	}
}

void oneWire_writeByte(uint8_t value){
	for (size_t i = 0; i < 8; i++)
		{
			uint8_t shifted = (value >> i);
			uint8_t bit = (shifted & 0b1);
			oneWire_writeBit(bit);
		}
}

uint8_t oneWire_readBit()
{
	uint8_t data = 0;
	// set data-port as outport
	oneWire_initDataPortAsWrite();

	// port low
	HAL_GPIO_WritePin(ONEWIRE_DATA_GPIO_Port, ONEWIRE_DATA_Pin, GPIO_PIN_RESET);

	// wait min. 1us
	DS_Delay_us(1);

	// release data-port (set as inport)
	oneWire_initDataPortAsRead();
	DS_Delay_us(5);

	// data = read port
	data = HAL_GPIO_ReadPin(ONEWIRE_DATA_GPIO_Port, ONEWIRE_DATA_Pin);

	// wait 59us
	DS_Delay_us(54);

	return data;
}

uint8_t oneWire_readByte()
{
	uint8_t buf = 0;
	for(uint8_t idx = 0; idx < 8; idx++){
		int bit = oneWire_readBit();
		// buf = buf << 1;
		buf = buf >> 1;
		// buf |= bit;
		buf |= bit << 7;
	}

	return buf;
}

void oneWire_initDataPortAsRead() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = ONEWIRE_DATA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ONEWIRE_DATA_GPIO_Port, &GPIO_InitStruct);
}

void oneWire_initDataPortAsWrite() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = ONEWIRE_DATA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	// GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ONEWIRE_DATA_GPIO_Port, &GPIO_InitStruct);
}

void oneWire_initIO()
{
	DS_Delay_init();
	// set data-port as output
	oneWire_initDataPortAsWrite();

	// set data-port to low
	HAL_GPIO_WritePin(ONEWIRE_DATA_GPIO_Port, ONEWIRE_DATA_Pin, GPIO_PIN_RESET);

	// TODO:set vcc-port as output

	// TODO:set power on
}

uint8_t oneWire_init() {
	uint8_t res = 0;
	oneWire_initDataPortAsWrite();

	HAL_GPIO_WritePin(ONEWIRE_DATA_GPIO_Port, ONEWIRE_DATA_Pin, GPIO_PIN_SET);
	DS_Delay_us(10);

	// set data-port low
	HAL_GPIO_WritePin(ONEWIRE_DATA_GPIO_Port, ONEWIRE_DATA_Pin, GPIO_PIN_RESET);

	// wait 480 us
	DS_Delay_us(480);

	// release data-port (data-port DDR as input)
	oneWire_initDataPortAsRead();

	// wait 60 us
	DS_Delay_us(60);

	// read pin ? 0 return 0 : return 1
	res = HAL_GPIO_ReadPin(ONEWIRE_DATA_GPIO_Port, ONEWIRE_DATA_Pin);
	DS_Delay_us(360);
	if (res)
		return 0;
	else
		return 1;
}



