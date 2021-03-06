/*
 * Common.c
 *
 *  Created on: 10.04.2020
 *      Author: Daniel
 */

#include "DS_Common.h"

void convertT(char *str, uint16_t *data) {
	str[0] = *data / 1000 + 48;						// t
	str[1] = (*data % 1000) / 100 + 48;				// h
	str[2] = ((*data % 1000) % 100) / 10 + 48;		// z
	str[3] = (((*data % 1000) % 100) % 10) + 48;	// e
}

void convert(char *str, uint16_t *data) {
	char tmp[4];
	uint8_t flags = 0;
	uint8_t ptrOfStr = 0;

	while(ptrOfStr < 12)
		str[ptrOfStr++] = ' ';

	ptrOfStr = 0;

	// Ganzzahlen
	uint16_t gz = (*data & 0x0ff0) >> 4;
	convertT(tmp, &gz);

	str[ptrOfStr++] = '+';

	// ersetze führenden '0' mit DEL
	for (size_t idx = 0; idx < 4; idx++) {
		if (tmp[idx] != '0' || flags & 0x01 || idx == 3) {
			str[ptrOfStr++] = tmp[idx];
			flags |= 0x01;
		}
	}

	// kommazahlen
	uint16_t k = (*data & 0xf) * 625;

	if (k) {
		convertT(tmp, &k);

		str[ptrOfStr++] = ',';

		// ersetze hintere '0' mit DEL
		for (size_t idx = 3; idx <= 3; idx--) {
			if (tmp[idx] == '0') { tmp[idx] = 0x7f; }
			else { break; }
		}

		for (size_t idx = 0; idx < 4; idx++) {
			if (tmp[idx] == 0x7f) { break; }
			str[ptrOfStr++] = tmp[idx];
		}
	}
	str[ptrOfStr++] = 'd'; //r0xb0; // °
	str[ptrOfStr++] = 'C';
	str[ptrOfStr++] = 0x00;
}
