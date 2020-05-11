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

typedef enum {
	WAIT_FOR_START			= 0x00U,
	READY					= 0x01U,
} EBaseObject_Sate;

typedef enum {
	NO_ERROR				= 0x00U,
} EBaseObject_ErrorState;

class BaseObject {
public:
	EBaseObject_ErrorState ErrorCode = NO_ERROR;
	EBaseObject_Sate State = WAIT_FOR_START;
public:
	BaseObject();
	virtual ~BaseObject();
};

#endif /* UTILS_BASEOBJECT_HPP_ */
