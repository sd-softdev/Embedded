/*
 * OneWire.c
 *
 *  Created on: Apr 4, 2020
 *      Author: Daniel
 */

#include "OneWire.h"

void convertT(char *str, uint16_t *data) {
	str[0] = *data / 1000 + 48;						// t
	str[1] = (*data % 1000) / 100 + 48;				// h
	str[2] = ((*data % 1000) % 100) / 10 + 48;		// z
	str[3] = (((*data % 1000) % 100) % 10) + 48;	// e
}

void convert(char *str, uint16_t *data) {
	char tmp[4];
	uint8_t flags = 0;
	uint8_t ptrOfStr = 0;
	// Ganzzahlen
	uint16_t gz = (*data & 0x0ff0) >> 4;
	convertT(tmp, &gz);

	str[ptrOfStr++] = '+';

	// ersetze führenden '0' mit DEL
	for (size_t idx = 0; idx < 4; idx++) {
		if (tmp[idx] != '0' || flags & 0x01 || idx == 3) {
			str[ptrOfStr++] = tmp[idx];
			flags |= 0x01;
		}
	}

	// kommazahlen
	uint16_t k = (*data & 0xf) * 625;

	if (k) {
		convertT(tmp, &k);

		str[ptrOfStr++] = ',';

		// ersetze hintere '0' mit DEL
		for (size_t idx = 3; idx <= 3; idx--) {
			if (tmp[idx] == '0') { tmp[idx] = 0x7f; }
			else { break; }
		}

		for (size_t idx = 0; idx < 4; idx++) {
			if (tmp[idx] == 0x7f) { break; }
			str[ptrOfStr++] = tmp[idx];
		}
	}
	//str[ptrOfStr++] = ' ';
	str[ptrOfStr++] = 0xb0; // °
	str[ptrOfStr++] = 'C';
	str[ptrOfStr++] = 0x00;
}

void oneWire_readTemp(char *str){
	uint8_t scratch[9];
	oneWire_readScratchpad(scratch);

	uint16_t tr = scratch[1];
	tr = tr << 8;
	tr |= scratch[0];

	convert(str, &tr);
}

void oneWire_readScratchpad(uint8_t *scratch){
//	uint8_t scratch[9];
	oneWire_writeByte(ONEWIRE_CMD_READSCRATCH);
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

// DWT-Einheit aktivieren
void DWT_Enable() {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
}
// Takt-Zähler - Messen der Anzahl der Befehle des Prozessors:
void DWT_CycCounterEn() {
  DWT->CTRL = 0x40000001;
}
void DWT_CycCounterDis() {
  DWT->CTRL = 0x40000000;
}
uint32_t DWT_CycCounterRead() {
  return DWT->CYCCNT;
}
void DWT_CycCounterClear() {
  DWT->CYCCNT = 0;
}


void DS_Delay_init() {
	DWT_Enable();
}

void DS_Delay_us(uint32_t time) {
	uint32_t fCpuMhz = HAL_RCC_GetHCLKFreq();
	fCpuMhz/=1000000;
	time--;
	uint32_t iZ = 0;
	DWT_CycCounterEn(); // Zähler aktivieren
	DWT_CycCounterClear(); // Zähler löschen

	iZ = DWT_CycCounterRead(); // Zähler auslesen

	while((iZ/fCpuMhz) < time)
		iZ = DWT_CycCounterRead();
}


