/*
 * DS_Delay.c
 *
 *  Created on: 07.04.2020
 *      Author: Daniel
 */

#include "DS_Delay.h"

// DWT-Einheit aktivieren
void DWT_Enable() {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
}
// Takt-Zähler - Messen der Anzahl der Befehle des Prozessors:
void DWT_CycCounterEn() {
  DWT->CTRL = 0x40000001;
}
void DWT_CycCounterDis() {
  DWT->CTRL = 0x40000000;
}
uint32_t DWT_CycCounterRead() {
  return DWT->CYCCNT;
}
void DWT_CycCounterClear() {
  DWT->CYCCNT = 0;
}


void DS_Delay_init() {
	DWT_Enable();
}

void DS_Delay_us(uint32_t time) {
	uint32_t fCpuMhz = HAL_RCC_GetHCLKFreq();
	fCpuMhz/=1000000;
	time--;
	uint32_t iZ = 0;
	DWT_CycCounterEn(); // Zähler aktivieren
	DWT_CycCounterClear(); // Zähler löschen

	iZ = DWT_CycCounterRead(); // Zähler auslesen

	while((iZ/fCpuMhz) < time)
		iZ = DWT_CycCounterRead();
}
