/*
 * ESP8266.h
 *
 *  Created on: May 1, 2020
 *      Author: Daniel
 */

#ifndef UTILS_ESP8266_HPP_
#define UTILS_ESP8266_HPP_

#ifndef ESP8266_RX_BUF_SIZE
 #define ESP8266_RX_BUF_SIZE 	1024
#endif

#include "DS_Common.hpp"
#include "BaseObject.hpp"

#include <string>
#include <stddef.h>
#include <stdint.h>
#include <regex>
#include <vector>

using std::string;
using std::to_string;
using std::regex;
using std::regex_search;
using std::cmatch;
using std::vector;
using std::stoul;


// TODO include the correct hal library dynamically
#include "stm32f4xx_hal.h"


extern void Error_Handler(void);


struct ESP8266AccessPoint {
	uint8_t Channel;
	string Name;
	string Mac;
};

class ESP8266 : public BaseObject {
public:
	string IpAddress = "0.0.0.0";
	string MacAddress = "00:00:00:00:00:00";
	bool IsReady;
	uint16_t receiveTimeOut = 500;	// in milliseconds
	vector<ESP8266AccessPoint> AccessPoints;

public:
	uint8_t receivedBufRaw[ESP8266_RX_BUF_SIZE];
	string receivedBuf;
	string transmitBuf;

//private:
	UART_HandleTypeDef *hUart;
	uint32_t ResetPin;
	GPIO_TypeDef *ResetPort;

public:
	ESP8266(USART_TypeDef * USART);
	ESP8266(UART_HandleTypeDef * hUart);
	virtual ~ESP8266();
	void InitCore(void);
	void RequestSendData(uint8_t connectionId, string content);
	void RequestCloseConnection(uint8_t connectionId);
	void RequestSoftReset();
	void RequestStartTcpServer(uint16_t port = 80);
	void RequestAllowMultipleConnections();
	void RequestDisallowMultipleConnections();
	void RequestIpAddress();
	void RequestConnect2Ap(string ssid, string pwd);
	void RequestCheckConnection();
	void RequestFirmwareVersion();
	void RequestAllAps();
	void UartDataReceivedCallback(void);
	void CheckReadyResponse(void);
	void HardwareReset(void);
	void SetHardwareResetPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	UART_HandleTypeDef * GetUsartInstance(void);
	void SetTimer(void);

protected:
	void SendRequestCore(string content);
	void SeperateIpFromESP8266Response(string res);
	void SeperateAcsFromESP8266Response(string res);
	void FunctionSwitch(string res);
	void UartSendString(string content);
};

#endif /* UTILS_ESP8266_HPP_ */
