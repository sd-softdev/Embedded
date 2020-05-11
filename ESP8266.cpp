/*
 * ESP8266.cpp
 *
 *  Created on: May 1, 2020
 *      Author: Daniel
 *
 *
 *
 *      http://stefanfrings.de/mikrocontroller_buch/Einstieg%20in%20die%20Elektronik%20mit%20Mikrocontrollern%20-%20Band%203.pdf
 */

#include "ESP8266.hpp"
#include "DS_Delay.h"


ESP8266::ESP8266(UART_HandleTypeDef *hUart) {
	this->hUart = hUart;
	this->InitCore();
}

ESP8266::ESP8266(USART_TypeDef * USART) {
	this->hUart = hUart;

	UART_HandleTypeDef huart = {0};

	huart.Instance = USART;
	huart.Init.BaudRate = 115200;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.Mode = UART_MODE_RX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart) != HAL_OK)
	{
	Error_Handler();
	}

	this->InitCore();
}

ESP8266::~ESP8266() {
	// TODO Auto-generated destructor stub
}

void ESP8266::InitCore(void) {
	//	SET_BIT(huart2.Instance->CR1, USART_CR1_RXNEIE);
	hUart->Instance->CR1 |= 0x20;

	for (int idx = 0; idx < ESP8266_RX_BUF_SIZE; idx++)
		this->receivedBufRaw[idx] = 0x00;

//	this->receivedBufRaw = (uint8_t *)malloc(512);
	HAL_UART_Receive_IT(this->hUart, this->receivedBufRaw, ESP8266_RX_BUF_SIZE);
	this->receivedBuf = "";
}

//void ESP8266::SetTimer(){
//	TIM_HandleTypeDef htim;
//	htim.Instance = TIM14;
//	htim.Init.Prescaler = 999;
//	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
//	htim.Init.Period = 15999;
//	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//	htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
//	if (HAL_TIM_Base_Init(&htim) != HAL_OK)
//	{
//	Error_Handler();
//	}
//}

UART_HandleTypeDef * ESP8266::GetUsartInstance(void){
	return this->hUart;
}

void ESP8266::RequestSendData(uint8_t connectionId, string content){
	/* create header for esp => "AT+CIPSEND=<id>,<length>" */
	char conIdStr[3];
	char length[8];
	itoa(content.length(), length, 10);
	itoa(connectionId,conIdStr,10);
	string cmd = "AT+CIPSEND=";
	cmd+=conIdStr;
	cmd+=",";
	cmd+=length;
	cmd+="\r\n";

	this->UartSendString(cmd);

//	uint32_t tickstart = 0U;
//	tickstart = HAL_GetTick();

	/* wait for 500ms before sending https response */
	HAL_Delay(500);
	this->UartSendString(content);
}
void ESP8266::RequestCloseConnection(uint8_t connectionId){
	string cmd = "AT+CIPCLOSE=";
//	char conIdStr[3];
//	itoa(connectionId, conIdStr, 10);
	cmd+=to_string(connectionId);
	this->UartSendString(cmd);
}
void ESP8266::RequestSoftReset(){
	this->SendRequestCore("AT+RST");
}
void ESP8266::RequestStartTcpServer(uint16_t port){
	string cmd = "AT+CIPSERVER=1,";
	char pStr[5];
	itoa(port, pStr, 10);
	cmd+=pStr;
	this->SendRequestCore(cmd);
}
void ESP8266::RequestAllowMultipleConnections(){
	this->SendRequestCore("AT+CIPMUX=1");
}
void ESP8266::RequestDisallowMultipleConnections(){
	this->SendRequestCore("AT+CIPMUX=0");
}
void ESP8266::RequestIpAddress(){
	this->SendRequestCore("AT+CIFSR");
}
void ESP8266::RequestConnect2Ap(string ssid, string pwd){
	string cmd = "AT+CWJAP=";
	cmd+="\"" + ssid + "\"";
	cmd+=",";
	cmd+="\"" + pwd + "\"";
	this->SendRequestCore(cmd);
}
void ESP8266::RequestCheckConnection(){
	this->SendRequestCore("AT");
}
void ESP8266::RequestFirmwareVersion(){
	this->SendRequestCore("AT+GMR");
}
void ESP8266::RequestAllAps(){
	this->SendRequestCore("AT+CWLAP");
}
void ESP8266::UartDataReceivedCallback(void){
}
void ESP8266::CheckReadyResponse(void){
	string t = "";
	if(this->State == WAIT_FOR_START){

		for(int idx = 0; idx < ESP8266_RX_BUF_SIZE; idx++){
			char cc = (char)this->receivedBufRaw[idx];

			if (cc != 0x00){
				__NOP();
				t += cc;
			}
		}
	}
	if (t.find("ready") != string::npos){
		this->State = READY;
	}
}
void ESP8266::HardwareReset(void){
	HAL_GPIO_WritePin(this->ResetPort, this->ResetPin, GPIO_PIN_RESET);
//	DS_Delay_us(100);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(this->ResetPort, this->ResetPin, GPIO_PIN_SET);
}
void ESP8266::SetHardwareResetPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
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
/*** protected ***/
void ESP8266::SendRequestCore(string content){
	this->UartSendString(content);
}
void ESP8266::SeperateIpFromESP8266Response(string res){
	regex reIp(R"(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})");
	regex reMac(R"([0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2}:[0-9a-f]{2})");
	cmatch mIp, mMac;

	regex_search(res.c_str(), mIp, reIp);
	this->IpAddress = mIp.str();

	regex_search(res.c_str(), mMac, reMac);
	this->IpAddress = mMac.str();
}
void ESP8266::SeperateAcsFromESP8266Response(string res){
	vector<string> lines (0);
	str_split<vector<string>>(res, lines, "\r\n");
	for(uint8_t idx; idx < lines.size(); idx++){
		if (lines[idx].find("+CWLAP:") == string::npos)
			continue;
		lines[idx].replace(0,7,"");
		vector<string> infos (0);
		str_split<vector<string>>(lines[idx], infos, ",");
		// TODO: error detetion

		/* create access points */
		ESP8266AccessPoint ap = {0};
		ap.Channel = stoul(infos[0], nullptr, 10);
		ap.Name = infos[1].substr(1,infos[1].size() - 2);
		ap.Mac = infos[3].substr(1,infos[3].size() - 2);
		this->AccessPoints.push_back(ap);
	}

}
void ESP8266::FunctionSwitch(string res){
	if (res.find("+CWLAP:") != string::npos)
		this->SeperateAcsFromESP8266Response(res);

	//WIFI CONNECTED
	if (res.find("WIFI CONNECTED") != string::npos) // wifi is connected
	{
		this->RequestIpAddress();
		this->RequestAllowMultipleConnections();
	}
	if (res.find("+CIFSR") != string::npos) // response is the ip and mac
	{
		this->SeperateIpFromESP8266Response(res);
	}
}

void ESP8266::UartSendString(string content){
	HAL_UART_Transmit(this->hUart, (uint8_t *)content.c_str(), content.size(), 500);
}
