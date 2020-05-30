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
		uint32_t mPos = header.rfind("GET");
		uint32_t hPos = header.rfind("HTTP/1.1");
		uint32_t qPos = 0;
		if (mPos != string::npos) {
			mPos += 4;
			string route = header.substr(mPos, --hPos - mPos);
			string queries = "";
			qPos = route.rfind('?');
			if (qPos != string::npos) {
				queries = route.substr(qPos, hPos - qPos);
				route = route.substr(0, qPos);
			}
			string channel = "";
			channel += header[5];
			this->rx = "";
			this->httpRequestSwitch(channel, HTTP_GET, &route, &queries);
		}
		// route
		__NOP();
	}
}

void ESPWebserver::httpRequestSwitch(string channel, EESP8266_Http_Req_Method method, string* route, string * queries, string params) {
	string content = "";
	string cSize = "";
	string cmd = "AT+CIPSEND=0,";
	if (method == HTTP_GET) {
		if (*route == "/") {
			__NOP();
		} else if (*route == "/info") {
			content = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
			content+= "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>ESP-01</title></head><body><h2>Hallo</h2></body></html>";
			cSize = to_string(content.length());

			cmd+=cSize;
			cmd+="\r\n";
			this->sendString(&cmd);
			this->read2Sign(">");

			this->sendString(&content);

			this->read2Sign("SEND OK\r\n");
		} else if (*route == "/favicon.ico") {
			content = "HTTP/1.1 404 OK\r\n\r\n";
			cSize = to_string(content.length());
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
