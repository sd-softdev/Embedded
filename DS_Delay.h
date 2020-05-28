/*
 * DS_Delay.h
 *
 *  Created on: 07.04.2020
 *      Author: Daniel
 *
 *      This Library is only working for CortexM3 and CortexM4!
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

void DWT_Enable();
void DWT_CycCounterEn();
void DWT_CycCounterDis();
uint32_t DWT_CycCounterRead();
void DWT_CycCounterClear();

void DS_Delay_us(uint32_t time);
void DS_Delay_ms(uint32_t time);
void DS_Delay_init(void);

#ifdef __cplusplus
}
#endif


#endif /* UTILS_DS_DELAY_H_ */
