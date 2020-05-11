/*
 * DS18x20.h
 *
 *  Created on: 10.04.2020
 *      Author: Daniel
 */

#ifndef UTILS_DS18X20_H_
#define UTILS_DS18X20_H_

#define ONEWIRE_CMD_READSCRATCH		0xbe
#define ONEWIRE_CMD_CONVERTT		0x44

#include <stddef.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "DS_Common.hpp"
#include "OneWire.h"

using namespace SD_Utils;

namespace SD_Utils {

class DS18x20 : public OneWire {
public:
	DS18x20(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	DS18x20(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint64_t romID);
	virtual ~DS18x20();
	OneWire_ErrorState ConvertTemp(void);
	OneWire_ErrorState WaitForNotBusy(void);
	void ReadTemp();
	OneWire_ErrorState ReadTempAndWait();
	char TempAsString[12];
	uint64_t RomID = 0x00;
private:
	uint16_t ConvertTempTimeout = 750;	// in milliseconds
};

} /* namespace SD_Utils */

#endif /* UTILS_DS18X20_H_ */
