/*
 * ESP8266.cpp
 *
 *  Created on: May 26, 2020
 *      Author: Daniel
 */

#include "ESP8266.hpp"

ESP8266AccessPoint::ESP8266AccessPoint(){

}

ESP8266::ESP8266() {
//	this->AccessPoints = (0);
	this->allowSignesRange.Start = 0x20;
	this->allowSignesRange.End = '~';
	this->State = EspState.WAIT_FOR_START;
	this->specialChar = "\r\n";
}

ESP8266::~ESP8266() {
	this->~UART();
}

void ESP8266::setHardwareResetPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	this->ResetPin = GPIO_Pin;
	this->ResetPort = GPIOx;

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = this->ResetPin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;	//GPIO_NOPULL GPIO_PULLUP GPIO_PULLDOWN
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(this->ResetPort, &GPIO_InitStruct);

	HAL_GPIO_WritePin(this->ResetPort, this->ResetPin, GPIO_PIN_SET);
}

void ESP8266::init(){
	this->initHardware();
	this->hardReset();
	this->wait4ReadySync();
	this->rx = "";
	this->State = EspState.READY;
	this->wait4WifiStateSync(5000);
}

void ESP8266::hardReset() {
	HAL_GPIO_WritePin(this->ResetPort, this->ResetPin, GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(this->ResetPort, this->ResetPin, GPIO_PIN_SET);
}

void ESP8266::wait4ReadySync(uint32_t timeout) {
	if (this->read2Sign("\r\nready\r\n", 2000)) {
		__NOP();
	} else {
		this->State |= EspState.WAIT4READY_TIMEOUT;
	}
}

void ESP8266::wait4WifiStateSync(uint32_t timeout) {
	this->State |= this->read2Sign("WIFI CONNECTED\r\n", timeout) ? EspState.WIFI_CONNECT : EspState.WIFI_DISCONNECT;
}

void ESP8266::requestAllApsSync() {
	vector<string> lines(0);
	this->rx = "";
	this->sendString("AT+CWLAP\r\n");

	if (this->read2Sign("\r\nOK\r\n", 5000)) {
		str_split(this->rx, lines, "\r\n");
		for (uint8_t idx = 0;idx < lines.size(); idx++) {
			if (lines[idx][0] == '+') {
				this->AccessPoints.push_back(lines[idx]);
			}
		}
	}

	if (this->State & EspState.READ2SIGN_TIMEOUT) {
		__NOP();
	}
	if (this->State & EspState.INVALID_CHAR_RECEIVED) {
		__NOP();
	}
	if (this->State & EspState.OVERRUN_ERROR) {
		__NOP();
	}
	this->rx = "";
}

void ESP8266::connect2WifiSync(string ssid, string pwd) {
	this->rx = "";
	this->sendString("AT+CWJAP=\"" + ssid + "\",\"" + pwd + "\"\r\n");
	if (this->read2Sign("\r\nOK\r\n", 2000)) {
		__NOP();
		this->State &= ~EspState.WIFI_DISCONNECT;
		this->State |= EspState.WIFI_CONNECT;
	} else {
		__NOP();
		this->State &= ~EspState.WIFI_CONNECT;
		this->State |= EspState.WIFI_DISCONNECT;
	}
}

void ESP8266::openTcpServerSync() {
	this->sendString("AT+CIPMUX=1\r\n");
	if (this->read2Sign("OK\r\n", 500)) {

	} else {
		//TODO:: set error code
		return;
	}

	this->sendString("AT+CIPSERVER=1,80\r\n");
	if (this->read2Sign("OK\r\n", 500)) {

	} else {
		//TODO:: set error code
		return;
	}
}
