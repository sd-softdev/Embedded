/*
 * DS_DS18x20.h
 *
 *  Created on: 07.04.2020
 *      Author: Daniel
 */

#ifndef UTILS_DS_DS18X20_H_
#define UTILS_DS_DS18X20_H_

#ifdef __cplusplus
 extern "C" {
#endif

#define ONEWIRE_CMD_READSCRATCH		0xbe
#define ONEWIRE_CMD_CONVERTT		0x44

#include <stddef.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "DS_OneWire.h"

void DS18x20_convertTemp(uint64_t *romId);

void convert(char *str, uint16_t *data);
void convertT(char *str, uint16_t *data);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_DS_DS18X20_H_ */
