/*
 * ESPWebserver.cpp
 *
 *  Created on: 28.05.2020
 *      Author: Daniel
 */

#include "ESPWebserver.hpp"

ESPWebserver::ESPWebserver() {
	// TODO Auto-generated constructor stub

}

ESPWebserver::~ESPWebserver() {
	// TODO Auto-generated destructor stub
}

void ESPWebserver::init(){
	ESP8266::init();
	if (this->State & EspState.WIFI_CONNECT) {
		this->openTcpServerSync();
	}
}

void ESPWebserver::check4IncomingRequests() {
//	uint32_t ipd = this->rx.rfind("+IPD");
	if (!this->read2Sign("+IPD", 100)) {
		return;
	}

	if (this->rx.rfind("+IPD") > this->rx.rfind("\r\n\r\n")) {
		return;
	}

	if(this->read2Sign("\r\n\r\n")) {
		uint16_t hBegin = this->rx.rfind("+IPD");
		string header = this->rx.substr(hBegin, string::npos);
		// method
		uint16_t mPos = header.rfind("GET");
		uint16_t hPos = header.rfind("HTTP/1.1");
		if (mPos != string::npos) {
			mPos += 4;
			string route = header.substr(mPos, --hPos - mPos);
			string channel = "";
			channel += header[5];
			this->rx = "";
			this->httpRequestSwitch(channel, HTTP_GET, &route);
		}
		// route
		__NOP();
	}
}

void ESPWebserver::httpRequestSwitch(string channel, EESP8266_Http_Req_Method method, string* route, string params) {
	__NOP();
	string header = "HTTP/1.1 200 OK\r\n\r\n";
	string content = header;
	string cSize = to_string(content.length());
	string cmd = "";
	if (method == HTTP_GET) {
		if (*route == "/") {
			__NOP();
		} else if (*route == "/info") {
			content+= "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>ESP-01</title></head><body><h2>Hallo</h2></body></html>";
			cSize = to_string(content.length());
			cmd = "AT+CIPSEND=0,";
			cmd+=cSize;
			cmd+="\r\n";
			this->sendString(&cmd);
			this->read2Sign(">");

			this->sendString(&content);

			this->read2Sign("SEND OK\r\n");
		}
	} else if (method != HTTP_GET) {

	}

	this->sendString("AT+CIPCLOSE="+channel+"\r\n");
	this->read2Sign("\r\nOK\r\n");
	this->rx = "";
}
