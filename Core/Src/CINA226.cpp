/*
 * CINA226.cpp
 *
 *  Created on: Feb 12, 2026
 *      Author: Nam Ha
 */

#include "CINA226.h"

#define INA226_SHUNT_VOLT_REG	(0x01U)
#define INA226_CONFIG_REG	(0x00U)
#define INA226_BUS_VOLT_REG	(0x02U)
#define INA226_POW_REG	(0x03U)
#define INA226_CURR_REG	(0x04U)
#define INA226_CALI_REG	(0x05U)
#define INA226_M_ID_REG	(0xFEU)

CINA226::CINA226(I2C_HandleTypeDef *hi2c, uint8_t deviceAddress, float rShunt,
		float maxCurrent) :
		CI2cDevice(hi2c, deviceAddress) {
	m_failed = false;
	m_currentLsb = maxCurrent / 32768.0f;

	if(!writeRegister(INA226_CONFIG_REG,0x4127U)){
		m_failed = true;
	}
	calibrate(m_currentLsb,rShunt);
}

bool CINA226::hasFailed() {
	if (m_failed) {
		m_failed = false;
		return true;
	}
	return false;
}

/*
 */
void CINA226::calibrate(float currentLsb, float rShunt) {
	const float CAL_CONST = 0.00512f;
	uint16_t CAL = (uint16_t)(CAL_CONST / (currentLsb * rShunt));
	if(!writeRegister(INA226_CALI_REG, CAL)){
		m_failed = true;
	}
}

/*
 * doc gia tri Shunt-Spannung va dua ra gia tri Volt
 * void *memcpy(void *dest, const void *src, size_t n);
 * dest: destination, con tro toi vung nho Ziel
 * src: con tro vung nho nguon
 * n: so byte can copy
 */

float CINA226::readShuntVoltage() {
	uint16_t rawdata = 0U;
	int16_t signedRaw = 0;
	float result = 0.0f;
	const float shuntVolt_const = 0.0000025f;
	if (readRegister(INA226_SHUNT_VOLT_REG, rawdata)) {
		(void)memcpy(&signedRaw,&rawdata,sizeof(rawdata));
		result = (float)signedRaw * shuntVolt_const;
	}else{
		m_failed = true;
	}
	return result;
}

/*
 *	 readBusVoltage = rawBus * 0.00125;
 *	 why: each LSB = 0.00125
 */

float CINA226::readBusVoltage() {
	float result = 0.0f;
	uint16_t rawBus = 0U;
	const float busVolt_const = 0.00125f;
	if(readRegister(INA226_BUS_VOLT_REG,rawBus)){
		result = (float)rawBus * busVolt_const;
	}else{
		m_failed = true;
	}
	return result;
}

float CINA226::readCurrent() {
	uint16_t rawCurrent = 0U;
	int16_t signedCurrent = 0;
	float resultCurrent = 0.0f;
	if(readRegister(INA226_CURR_REG,rawCurrent)){
		(void)memcpy(&signedCurrent,&rawCurrent,sizeof(rawCurrent));
		resultCurrent = (float)signedCurrent * m_currentLsb;
	}else{
		m_failed = true;
	}
	return resultCurrent;
}

float CINA226::readPower() {
	uint16_t rawPower = 0U;
	float resultPower = 0.0f;
	if(readRegister(INA226_POW_REG,rawPower)){
		resultPower = (float)rawPower * (25.0f * m_currentLsb);
	}else{
		m_failed = true;
	}
	return resultPower;
}

bool CINA226::isConnected() {
	uint16_t valueFromID = 0U;
	const uint16_t iDValue_const = 0x5449U;
	if(readRegister(INA226_M_ID_REG,valueFromID)){
		if(valueFromID == iDValue_const){
			return true;
		}
	}else{
		m_failed = true;
	}
	return false;
}
