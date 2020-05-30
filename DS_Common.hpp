/*
 * Common.h
 *
 *  Created on: 10.04.2020
 *      Author: Daniel
 */

#ifndef UTILS_DS_COMMON_HPP_
#define UTILS_DS_COMMON_HPP_

#include <string>
#include <stddef.h>
#include <stdint.h>
#include <vector>

using std::string;
using std::vector;

//template <class Container>
void str_split(const string& str, vector<string>& cont, const string& delims = " ");

 /* legacy */

uint8_t CDC_Transmit_FS_String(char* Buf);
void convertT(char *str, uint16_t *data);
void convert(char* str, float* tAsF, uint16_t *data);


#endif /* UTILS_DS_COMMON_HPP_ */
