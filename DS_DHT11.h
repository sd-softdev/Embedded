/*
 * DS_DHT11.h
 *
 *  Created on: 08.04.2020
 *      Author: Daniel
 */

#ifndef UTILS_DS_DHT11_H_
#define UTILS_DS_DHT11_H_

#include <stddef.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "DS_Delay.h"

class DS_DHT11 {
public:
	DS_DHT11(uint32_t dataPin, GPIO_TypeDef *dataPort);
	virtual ~DS_DHT11();

private:
	uint32_t Pin;
	GPIO_TypeDef *Port;
	GPIO_InitTypeDef IoStruct;
	uint8_t buf[5];

	void Init_Io();
	void DeInit_Io();
	void GenerateIoBasicStruct();
	void InitDataPortAsInput();
	void InitDataPortAsOutput();
	void SendReset();
	uint8_t WaitForResponse();
	uint8_t ReadBit(uint8_t *bit);
	uint8_t ReadByte(uint8_t *buf);
	uint8_t ReadDataPackage(uint8_t *buf);
};

#endif /* UTILS_DS_DHT11_H_ */
