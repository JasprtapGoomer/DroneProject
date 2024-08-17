/*
 * NRF24L01.c
 *
 *  Created on: Aug 16, 2024
 *      Author: Victo
 */
#include "stm32f4xx.h"
#include "NRF24L01.h"
extern SPI_HandleTypeDef hspi2;
#define NRF24_SPI &hspi2
#define NRF24_CE_PORT GPIOG
#define NRF24_CE_PIN GPIO_PIN_1

#define NRF24_CSN_PORT GPIOG
#define NRF24_CSN_PIN GPIO_PIN_4

void CS_Select (void){ // Selecting Slave Device, setting pin to LOW
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_RESET);
}

void CS_unSelect (void){ // Unselecting Slave Device, setting pin to HIGH
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_SET);
}

void CE_Enable (void){ // Enable the Device
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_SET);
}

void CE_Disable (void){ // Disable the Device
	HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_RESET);
}


void nrf24WriteReg(uint8_t Reg, uint8_t Data){ // Writes data to a register
	uint8_t buf[2]; // Holds 2 bytes of data
	buf[0] = Reg | 1 << 5; // 0 is the register, writing 1 in 5th position
	buf[1] = Data;
	CS_Select();
	HAL_SPI_Transmit(NRF24_SPI, buf, 2, 1000);

	// Pull CS Pin HIGH to release device
	CS_unSelect();

}

void nrf24_WriteRegMulti (uint8_t Reg, uint8_t *data, int size){
	uint8_t buf[2]; // Holds 2 bytes of data
	buf[0] = Reg | 1 << 5; // 0 is the register, writing 1 in 5th position
	CS_Select();
	HAL_SPI_Transmit(NRF24_SPI, buf, 1, 100); // send register address
	HAL_SPI_Transmit(NRF24_SPI, data, size, 100);

	// Pull CS Pin HIGH to release device
	CS_unSelect();
}


uint8_t nrf24_ReadReg (uint8_t Reg){
	uint8_t data = 0;
	CS_Select();
	HAL_SPI_Transmit(NRF24_SPI, &Reg, 1, 100);
	HAL_SPI_Receive(NRF24_SPI, &data, 1, 100);
	// Pull CS Pin HIGH to release device
	CS_unSelect();
	return data;
}


uint8_t nrf24_ReadReg_Multi (uint8_t Reg, uint8_t *data, int size){ // Read multiple data bytes starting from Reg, Pointer to variable where you want to store data, and # of bytes to be read.

	CS_Select();
	HAL_SPI_Transmit(NRF24_SPI, &Reg, 1, 100);
	HAL_SPI_Receive(NRF24_SPI, data, size, 1000);
	// Pull CS Pin HIGH to release device
	CS_unSelect();
	return data;
}

void nrfsendCmd(uint8_t cmd){
	CS_Select();
	HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 100);
	CS_unSelect();
}

void NRF24_Init (void){
	//Disable the chip before configuring the device
	CE_Disable();
	nrf24_WriteReg(CONFIG, 0);

	nrf24_WriteReg(EN_AA, 0); // Not Enabling AutoAck

	nrf24_WriteReg(EN_RXADDR, 0); // Not enabling any data for pipe

	nrf24_WriteReg(SETUP_AW, 0x03); // 5 bytes for TX/RX

	nrf24_WriteReg(SETUP_RETR, 0); // No Retransmission

	nrf24_WriteReg(RF_CH, 0); // WIll be setup during TX or RX

	nrf24_WriteReg(RF_SETUP, 0x0E); // Power = 0dB, Data Rate = 2Mbps

}
