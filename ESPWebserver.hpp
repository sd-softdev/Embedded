/*
 * ESPWebserver.hpp
 *
 *  Created on: 28.05.2020
 *      Author: Daniel
 */

#ifndef UTILS_ESPWEBSERVER_HPP_
#define UTILS_ESPWEBSERVER_HPP_

#include "ESP8266.hpp"

using std::to_string;

typedef enum {
	HTTP_GET,
} EESP8266_Http_Req_Method;

class ESPWebserver: public ESP8266 {
public:
	ESPWebserver();
	virtual ~ESPWebserver();
	virtual void init();
	void check4IncomingRequests();
	virtual void httpRequestSwitch(string channel, EESP8266_Http_Req_Method method, string* route, string * queries, string params = "");
};

#endif /* UTILS_ESPWEBSERVER_HPP_ */
