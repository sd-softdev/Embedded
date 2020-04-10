/*
 * OneWire.cpp
 *
 *  Created on: 10.04.2020
 *      Author: Daniel
 */

#include "OneWire.h"

namespace SD_Utils {

OneWire_ErrorState OneWire::ReadScratchpad(uint8_t *scratch) {
	this->Init();
	this->WriteByte(0xcc);
	this->WriteByte(0xbe);

	for(uint8_t idx = 0; idx < 9; idx++)
		this->ReadByte(&scratch[idx]);
	return NoError;
}

void OneWire::WriteBit(bool bit) {
	// set data-port as out
	this->InitDataPortAsWrite();

	// data-port = low
	HAL_GPIO_WritePin(this->DataPort, this->DataPin, GPIO_PIN_RESET);

	if (bit)
	{
		// wait 15 us
		DS_Delay_us(8);

		// release data-port (set as inport)
		this->InitDataPortAsRead();

		// wait 45 us
		DS_Delay_us(48);
	} else
	{
		// wait 60 us
		DS_Delay_us(60);
		// release data-port (set as inport)
		this->InitDataPortAsRead();
	}
}

OneWire_ErrorState OneWire::WriteByte(uint8_t value) {
	for (size_t i = 0; i < 8; i++){
		uint8_t shifted = (value >> i);
		uint8_t bit = (shifted & 0b1);
		this->WriteBit(bit);
	}

	return NoError;
}

bool OneWire::ReadBit() {
	bool data = 0;
	// set data-port as outport
	this->InitDataPortAsWrite();

	// port low
	HAL_GPIO_WritePin(this->DataPort, this->DataPin, GPIO_PIN_RESET);

	// wait min. 1us
	DS_Delay_us(1);

	// release data-port (set as inport)
	this->InitDataPortAsRead();
	DS_Delay_us(5);

	// data = read port
	data = HAL_GPIO_ReadPin(this->DataPort, this->DataPin);

	// wait 59us
	DS_Delay_us(54);

	return data;
}

OneWire_ErrorState OneWire::ReadByte(uint8_t *buf) {

	for(uint8_t idx = 0; idx < 8; idx++){
		int bit = this->ReadBit();
		*buf = *buf >> 1;
		*buf |= bit << 7;
	}

	return NoError;
}

void OneWire::InitDataPortAsRead() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = this->DataPin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(this->DataPort, &GPIO_InitStruct);
}

void OneWire::InitDataPortAsWrite() {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = this->DataPin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	// GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(this->DataPort, &GPIO_InitStruct);
}

void OneWire::Init() {
	this->InitDataPortAsWrite();

	HAL_GPIO_WritePin(this->DataPort, this->DataPin, GPIO_PIN_SET);
	DS_Delay_us(10);

	// set data-port low
	HAL_GPIO_WritePin(this->DataPort, this->DataPin, GPIO_PIN_RESET);

	// wait 480 us
	DS_Delay_us(480);

	// release data-port (data-port DDR as input)
	this->InitDataPortAsRead();

	this->IsPresent = HAL_GPIO_ReadPin(this->DataPort, this->DataPin);
	this->SetErrorState(this->IsPresent ? NoError : NoSlavePresence);
	if(!this->IsPresent) {
		return;
	}
	// wait 60 us
	DS_Delay_us(60);

	// read pin ? 0 return 0 : return 1
	this->IsPresent = HAL_GPIO_ReadPin(this->DataPort, this->DataPin) == GPIO_PIN_RESET;
	this->SetErrorState(this->IsPresent ? NoError : NoSlavePresence);
	if(!this->IsPresent) {
		return;
	}
	DS_Delay_us(360);
}

/* IO's */

/* Presence */
void OneWire::InitPresencePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	this->PresencePort = GPIOx;
	this->PresencePin = GPIO_Pin;

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = this->PresencePin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(this->PresencePort, &GPIO_InitStruct);

	this->UsingPresencePin = true;
}

void OneWire::SetPresencePin() {
	if (!this->UsingPresencePin)
		return;
	HAL_GPIO_WritePin(this->PresencePort, this->PresencePin, (GPIO_PinState)this->IsPresent);
}

void OneWire::SetPresencePin(bool on) {
	this->IsPresent = on;
	this->SetPresencePin();
}

/* Error State Flow */
void OneWire::SetErrorState(OneWire_ErrorState state) {
	if(this->ErrorState == state)
		return;
	this->ErrorState = state;
	switch ( this->ErrorState ) {
		 case NoError:
			this->SetErrorPin(false);
			this->SetPresencePin(true);
			break;
		 case NoSlavePresence:
			this->SetErrorPin(true);
			this->SetPresencePin(false);
			break;
		 case TimeOut:
			this->SetErrorPin(true);
			this->SetPresencePin(true);
			break;
		 default:
			break;
	}
}


/* Error */
void OneWire::InitErrorPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	this->ErrorPort = GPIOx;
	this->ErrorPin = GPIO_Pin;

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = this->ErrorPin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(this->ErrorPort, &GPIO_InitStruct);

	this->UsingErrorPin = true;
}

void OneWire::SetErrorPin() {
	if (!this->UsingErrorPin)
		return;
	HAL_GPIO_WritePin(this->ErrorPort, this->ErrorPin, (GPIO_PinState)this->IsError);
}

void OneWire::SetErrorPin(bool on) {
	this->IsError = on;
	this->SetErrorPin();
}

void OneWire::InitIO(){
	// set data-port as output
	this->InitDataPortAsWrite();

	// set data-port to low
	HAL_GPIO_WritePin(this->DataPort, this->DataPin, GPIO_PIN_RESET);
}

OneWire::OneWire(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	DS_Delay_init();
	this->DataPin = GPIO_Pin;
	this->DataPort = GPIOx;
	this->InitIO();
}

OneWire::OneWire() {
	DS_Delay_init();
	this->DataPin = ONEWIRE_DATA_Pin;
	this->DataPort = ONEWIRE_DATA_GPIO_Port;
	this->InitIO();
}

OneWire::~OneWire() {
	// TODO deinit all pins
}

} /* namespace SD_Utils */
