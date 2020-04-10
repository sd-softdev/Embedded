/*
 * DS18x20.cpp
 *
 *  Created on: 10.04.2020
 *      Author: Daniel
 */

#include "DS18x20.h"

namespace SD_Utils {

OneWire_ErrorState DS18x20::ReadTempAndWait() {
	this->ConvertTemp();
	if (this->IsError) return this->ErrorState;
	this->WaitForNotBusy();
	if (this->IsError) return this->ErrorState;
	this->ReadTemp();
	return this->ErrorState;
}

void DS18x20::ReadTemp(){
	uint8_t scratch[9];
	this->ReadScratchpad(scratch);

	uint16_t tr = scratch[1];
	tr = tr << 8;
	tr |= scratch[0];

	convert(this->TempAsString, &tr);
}


OneWire_ErrorState DS18x20::ConvertTemp(void) {
	this->Init();
	if (this->ErrorState != NoError)
		return this->ErrorState;
	if(this->RomID == 0x00) {
		this->WriteByte(ONEWIRE_CMD_SKIPROM);
	}
	this->WriteByte(ONEWIRE_CMD_CONVERTT);
	return NoError;
}

OneWire_ErrorState DS18x20::WaitForNotBusy() {
	uint32_t ts = HAL_GetTick();
	uint8_t busy = 1;
	busy ^= this->ReadBit();
	while(busy) {
		busy ^= this->ReadBit();
		if ((HAL_GetTick() - ts) >= this->ConvertTempTimeout){
			this->SetErrorPin(true);
			return TimeOut;
		}
	}
	return NoError;
}


DS18x20::DS18x20(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	this->DataPin = GPIO_Pin;
	this->DataPort = GPIOx;
}

DS18x20::DS18x20(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint64_t romID) {
	this->DataPin = GPIO_Pin;
	this->DataPort = GPIOx;
	this->RomID = romID;
}

DS18x20::~DS18x20() {
	// TODO Auto-generated destructor stub
}

} /* namespace SD_Utils */
