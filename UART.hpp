/*
 * UART.hpp
 *
 *  Created on: May 12, 2020
 *      Author: Daniel
 */

#ifndef UTILS_UART_HPP_
#define UTILS_UART_HPP_

#include "stm32f4xx_hal.h"
#include "BaseObject.hpp"
#include "DS_Delay.h"

extern void Error_Handler(void);
void USART2_IRQHandler(void);

struct UARTSTATE : BASESTATE {
//	const uint8_t ERROR					= 0x01U;	// Error detected (for more details show 'ErrorStae')
//	const uint8_t WAIT_FOR_START		= 0x02U;	// Wait for Init/Start phase or precondotions
//	const uint8_t INIT_ONGOING 			= 0x04U;	// Init sequenz is ongoing
//	const uint8_t READY					= 0x08U;	// Object is ready
	const uint8_t INVALID_CHAR_RECEIVED = 0x10U;
	const uint8_t READ2SIGN_TIMEOUT		= 0x20U;
	const uint8_t OVERRUN_ERROR		= 0x40U;
};
const UARTSTATE UartState = {};

struct AllowSignesStruct {
	char Start;
	char End;
};

class UART : public BaseObject {
public:
	UART_HandleTypeDef huart = {0};
	string rx = "";

public:
	UART();
	virtual ~UART();
	void initHardware(void);
	void receiveCallback(void);
	bool read2Sign(string end, uint16_t timeout = 500);
	void sendString(string content);
	void sendString(string * content);

protected:
	AllowSignesStruct allowSignesRange = {};
	string specialChar = "\r\n";
};

#endif /* UTILS_UART_HPP_ */
