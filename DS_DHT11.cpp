/*
 * DS_DHT11.cpp
 *
 *  Created on: 08.04.2020
 *      Author: Daniel
 */

#include "DS_DHT11.h"


DS_DHT11::DS_DHT11(uint32_t dataPin, GPIO_TypeDef *dataPort) {
	this->Pin = dataPin;
	this->Port = dataPort;
}

DS_DHT11::~DS_DHT11() {
	this->DeInit_Io();
}


// protected methods
uint8_t DS_DHT11::ReadDataPackage(uint8_t *buf) {
	for(size_t idx = 0; idx < 8; idx++){
		this->ReadByte(&buf[idx]); // TODO: error handling
	}

	return 0;
}

uint8_t DS_DHT11::ReadByte(uint8_t *buf) {
	uint8_t bit = 0;
	for(uint8_t idx = 0; idx < 8; idx++){
		this->ReadBit(&bit); // TODO: error handling
	}

	return 0;
}
uint8_t DS_DHT11::ReadBit(uint8_t *bit) {
	// 1st implementation
	// TODO: time out needed => return value != 0

	uint8_t res = HAL_GPIO_ReadPin(this->Port, this->Pin);
	while(res == 1) // wait for low
		res = HAL_GPIO_ReadPin(this->Port, this->Pin);

	DS_Delay_us(25);
	*bit = HAL_GPIO_ReadPin(this->Port, this->Pin);

	while(res == 0) // wait for high
		res = HAL_GPIO_ReadPin(this->Port, this->Pin);

	return 0;
}
uint8_t DS_DHT11::WaitForResponse() {
	// 1st implementation
	// TODO: time out needed

	uint8_t res = HAL_GPIO_ReadPin(this->Port, this->Pin);
	while(res == 1)	// wait for low
		res = HAL_GPIO_ReadPin(this->Port, this->Pin);

	while(res == 0) // wait for high
			res = HAL_GPIO_ReadPin(this->Port, this->Pin);

	return 0;
}
void DS_DHT11::SendReset() {
	// Port = low
	HAL_GPIO_WritePin(this->Port, this->Pin, GPIO_PIN_RESET);
	//wait 18ms
	DS_Delay_us(18000);
	// release Port
	this->InitDataPortAsOutput();
}

// private methods
void DS_DHT11::InitDataPortAsInput() {
	this->IoStruct.Mode = GPIO_MODE_INPUT;
	this->Init_Io();
}

void DS_DHT11::InitDataPortAsOutput() {
	this->IoStruct.Mode = GPIO_MODE_OUTPUT_PP;	// TODO: maybe using open drain
	this->Init_Io();
}

void DS_DHT11::GenerateIoBasicStruct() {
	this->IoStruct = {0};
	this->IoStruct.Pin = this->Pin;
//	this->IoStruct.Pull = GPIO_NOPULL;	// TODO: maybe internal pull up
	this->IoStruct.Speed = GPIO_SPEED_FREQ_LOW;

}

void DS_DHT11::Init_Io() {
	HAL_GPIO_Init(this->Port, &this->IoStruct);
}
void DS_DHT11::DeInit_Io() {
	HAL_GPIO_DeInit(this->Port, this->Pin);
}
