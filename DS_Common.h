/*
 * Common.h
 *
 *  Created on: 10.04.2020
 *      Author: Daniel
 */

#ifndef UTILS_DS_COMMON_H_
#define UTILS_DS_COMMON_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

void convertT(char *str, uint16_t *data);
void convert(char *str, uint16_t *data);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_DS_COMMON_H_ */
