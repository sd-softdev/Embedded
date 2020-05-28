/*
 * UART.cpp
 *
 *  Created on: May 12, 2020
 *      Author: Daniel
 */

#include "UART.hpp"

UART::UART() {
	DWT_Enable();
	this->allowSignesRange.Start = 0x00;
	this->allowSignesRange.End = 0xff;
//	this->State = UartState.WAIT_FOR_START;
}

UART::~UART() {
	HAL_UART_DeInit(&this->huart);
}

void UART::initHardware(){
	this->State = UartState.INIT_ONGOING;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	this->huart.Instance = USART2;
	this->huart.Init.BaudRate = 115200;
	this->huart.Init.WordLength = UART_WORDLENGTH_8B;
	this->huart.Init.StopBits = UART_STOPBITS_1;
	this->huart.Init.Parity = UART_PARITY_NONE;
	this->huart.Init.Mode = UART_MODE_TX_RX;
	this->huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	this->huart.Init.OverSampling = UART_OVERSAMPLING_16;
	this->State = HAL_UART_Init(&this->huart) == HAL_OK ? UartState.READY : UartState.ERROR;

	SET_BIT(this->huart.Instance->CR1, UART_IT_RXNE);

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void UART::receiveCallback() {
	bool rxne = READ_BIT(this->huart.Instance->SR, UART_FLAG_RXNE);	//check if data in buffer exist
	char rxC = 0x00;

	while(rxne) {
		rxC = (char)this->huart.Instance->DR;	// get sign from buffer
		if ((rxC >= this->allowSignesRange.Start && rxC <= this->allowSignesRange.End) || (this->specialChar.rfind(rxC) != string::npos)){
			this->rx+=rxC;
		} else {
			this->State |= UartState.INVALID_CHAR_RECEIVED;
			continue;
		}

		rxne = READ_BIT(this->huart.Instance->SR, UART_FLAG_RXNE);	//check if data in buffer exist
	}

}

bool UART::read2Sign(string end, uint16_t timeout) {
	bool To = false;
	uint32_t tickstart = HAL_GetTick();

	if (rx.rfind(end) != string::npos) {
	  return true;
	}
	while(!To) {
		if (this->rx.rfind(end) != string::npos) {
			return true;
		}
		To = (HAL_GetTick() - tickstart) > timeout;
	}
	this->State |= UartState.READ2SIGN_TIMEOUT;
	return false;
}

void UART::sendString(string content) {
	HAL_UART_Transmit(&this->huart, (uint8_t *)content.c_str(), content.length(), 500);
}
void UART::sendString(string * content) {
	HAL_UART_Transmit(&this->huart, (uint8_t *)content->c_str(), content->length(), 500);
}
