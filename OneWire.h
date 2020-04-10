/*
 * OneWire.h
 *
 *  Created on: 10.04.2020
 *      Author: Daniel
 */

#ifndef UTILS_ONEWIRE_H_
#define UTILS_ONEWIRE_H_

#define ONEWIRE_CMD_READROM			0x33
#define ONEWIRE_CMD_SKIPROM			0xcc

#include <stddef.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "DS_Delay.h"


#ifndef ONEWIRE_DATA_Pin
 #define ONEWIRE_DATA_Pin			GPIO_PIN_1
#endif

#ifndef ONEWIRE_DATA_GPIO_Port
 #define ONEWIRE_DATA_GPIO_Port		GPIOA
#endif

typedef enum
{
  NoError = 0x00,
  NoSlavePresence = 0x01,
  TimeOut = 0x02,
  Undefined = 0xff,
} OneWire_ErrorState;

namespace SD_Utils {

class OneWire {
public:
	OneWire();
	OneWire(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	virtual ~OneWire();
	void Init();
	uint8_t ReadSlaveInfos(uint64_t *id, uint8_t *familyCode);
	OneWire_ErrorState ReadScratchpad(uint8_t *scratch);
	uint32_t DataPin;
	GPIO_TypeDef *DataPort;
	uint32_t PresencePin;
	GPIO_TypeDef *PresencePort;
	uint32_t ErrorPin;
	GPIO_TypeDef *ErrorPort;
	bool ParasitePower = false;
	bool ExternalPullUp = false;

	void InitPresencePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	bool IsPresent = false;

	void InitErrorPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	bool IsError = false;

	OneWire_ErrorState ErrorState = Undefined;

protected:
	void WriteBit(bool bit);
	OneWire_ErrorState WriteByte(uint8_t value);
	bool ReadBit();
	OneWire_ErrorState ReadByte(uint8_t *buf);
	// common io's
	void InitDataPortAsRead();
	void InitDataPortAsWrite();
	void InitIO();

	// Presence Out
	void SetPresencePin();
	void SetPresencePin(bool on);
	bool UsingPresencePin = false;

	// Error Out
	void SetErrorPin();
	void SetErrorPin(bool on);
	bool UsingErrorPin = false;

	void SetErrorState(OneWire_ErrorState state);
};

} /* namespace SD_Utils */

#endif /* UTILS_ONEWIRE_H_ */
