/*
 * CI2cDevice.cpp
 *
 *  Created on: Feb 3, 2023
 *      Author: mnl
 */

#include "CI2cDevice.h"

static const uint32_t I2C_TIMEOUT = 1000U;

CI2cDevice::CI2cDevice(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress) :
		m_hi2c(hi2c), m_deviceAddress(static_cast<uint8_t>(deviceAddress << 1U)) {

}

//HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c,
//	uint16_t DevAddress,
//	uint8_t *pData,
//	uint16_t Size,
//	uint32_t Timeout)
bool CI2cDevice::write(uint8_t *data, uint16_t length) {
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Transmit(m_hi2c, m_deviceAddress, data, length,
			I2C_TIMEOUT);
	return (status == HAL_OK);
}

//HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c,
//		uint16_t DevAddress,
//		uint8_t *pData,
//		uint16_t Size,
//		uint32_t Timeout)
bool CI2cDevice::read(uint8_t *data, uint16_t length) {
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Receive(m_hi2c, m_deviceAddress, data, length,
			I2C_TIMEOUT);
	return (status == HAL_OK);
}

// DMA but without callbackfunctions
bool CI2cDevice::writeDMA(uint8_t* data, uint16_t length){
	HAL_StatusTypeDef status;
	uint32_t start = HAL_GetTick();
	if(data == nullptr || length == 0){
		return false;
	}
	status = HAL_I2C_Master_Transmit_DMA(m_hi2c, m_deviceAddress,data,length);
	if(status != HAL_OK){
			return false;
		}
	//wait to finish with timeout
	while(HAL_I2C_GetState(m_hi2c) != HAL_I2C_STATE_READY){
		if(HAL_GetTick() - start > I2C_TIMEOUT){
			return false;
		}
	}

	return true;
}

bool CI2cDevice::readDMA(uint8_t* data, uint16_t length){

	return true;
}

bool CI2cDevice::writeRegister(uint8_t regAddress, uint16_t value) {
	uint8_t buff[3];
	buff[0] = regAddress;
	buff[1] = static_cast<uint8_t>(value >> 8U);
	buff[2] = static_cast<uint8_t>(value & 0xFFU);
	return write(buff, 3U);
}

bool CI2cDevice::readRegister(uint8_t regAddress, uint16_t &value) {

	if (!write(&regAddress, 1U)) {
		return false;
	}

	uint8_t buff[2];
	if (!read(buff, 2U)) {
		return false;
	}

	value = (static_cast<uint16_t>(buff[0]) << 8U)
			| static_cast<uint16_t>(buff[1]);
	return true;
}

