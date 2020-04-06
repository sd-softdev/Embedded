/*
 * OneWire.h
 *
 *  Created on: Apr 4, 2020
 *      Author: Daniel
 */


#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

#define DELAY		50
#define F_CPU 		168000000UL
#define F_CPU_MHZ 	(F_CPU/1000000UL)

#define ONEWIRE_DATA_Pin			GPIO_PIN_1
#define ONEWIRE_DATA_GPIO_Port		GPIOA

#define ONEWIRE_CMD_READROM			0x33
#define ONEWIRE_CMD_READSCRATCH		0xbe

void DS_Delay_us(uint32_t time);
void DS_Delay_init(void);

void oneWire_initDataPortAsRead();
void oneWire_initDataPortAsWrite();
void oneWire_initIO();
uint8_t oneWire_init();
void oneWire_writeBit(uint8_t bit);
void oneWire_writeByte(uint8_t value);
uint8_t oneWire_readBit();
uint8_t oneWire_readByte();
uint8_t oneWire_readSlaveInfos(uint64_t *id, uint8_t *familyCode);
void oneWire_readScratchpad(uint8_t *scratch);
void oneWire_readTemp(char *str);

void convertT(char *str, uint16_t *data);
void convert(char *str, uint16_t *data);

#ifdef __cplusplus
}
#endif

#endif /* ONEWIRE_H_ */
