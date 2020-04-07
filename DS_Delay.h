/*
 * DS_Delay.h
 *
 *  Created on: 07.04.2020
 *      Author: Daniel
 */

#ifndef UTILS_DS_DELAY_H_
#define UTILS_DS_DELAY_H_

#ifdef __cplusplus
 extern "C" {
#endif

#define F_CPU 		168000000UL
#define F_CPU_MHZ 	(F_CPU/1000000UL)

#include <stddef.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

void DS_Delay_us(uint32_t time);
void DS_Delay_init(void);

#ifdef __cplusplus
}
#endif


#endif /* UTILS_DS_DELAY_H_ */
