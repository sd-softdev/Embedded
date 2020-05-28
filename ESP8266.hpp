/*
 * ESP8266.hpp
 *
 *  Created on: May 26, 2020
 *      Author: Daniel
 */

#ifndef UTILS_ESP8266_HPP_
#define UTILS_ESP8266_HPP_

#include "DS_Common.hpp"
#include "UART.hpp"

#include <vector>

using std::vector;

struct ESPSTATE : UARTSTATE {
	const uint16_t WAIT4READY_TIMEOUT 		= 0x0100UL;
	const uint16_t WAIT4WIFISTATE_TIMEOUT 	= 0x0200UL;
	const uint16_t WIFI_DISCONNECT 			= 0x0400UL;
	const uint16_t WIFI_CONNECT 			= 0x0800UL;
//	const uint8_t INVALID_CHAR_RECEIVED 	= 0x0010U;
};
const ESPSTATE EspState = {};

class ESP8266AccessPoint {
	public:
	ESP8266AccessPoint();
	string Name;
	string Mac;
};

class ESP8266: public UART {
public:
	UART_HandleTypeDef *hUart;
	uint32_t ResetPin;
	GPIO_TypeDef *ResetPort;
	vector<string> AccessPoints; //(0);

public:
	ESP8266();
	virtual ~ESP8266();
	void setHardwareResetPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	virtual void init();
	void hardReset();
	void wait4ReadySync(uint32_t timeout = 5000);
	void wait4WifiStateSync(uint32_t timeout = 1000);
	void requestAllApsSync();
	void connect2WifiSync(string ssid, string pwd);
	void openTcpServerSync();
};

#endif /* UTILS_ESP8266_HPP_ */
