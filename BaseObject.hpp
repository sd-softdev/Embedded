/*
 * BaseObject.hpp
 *
 *  Created on: 07.05.2020
 *      Author: Daniel
 */

#ifndef UTILS_BASEOBJECT_HPP_
#define UTILS_BASEOBJECT_HPP_

#include <stddef.h>
#include <stdint.h>
#include <string>
//#include <vector>

using std::string;

struct BASESTATE {
	const uint8_t ERROR					= 0x01U;	// Error detected (for more details show 'ErrorStae')
	const uint8_t WAIT_FOR_START		= 0x02U;	// Wait for Init/Start phase or precondotions
	const uint8_t INIT_ONGOING 			= 0x04U;	// Init sequenz is ongoing
	const uint8_t READY					= 0x08U;	// Object is ready
};
const BASESTATE BaseState = {};

class BaseObject {
public:
	uint32_t State = 0x00;
public:
	BaseObject();
	virtual ~BaseObject();
};

#endif /* UTILS_BASEOBJECT_HPP_ */
