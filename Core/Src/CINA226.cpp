/*
 * CINA226.cpp
 *
 *  Created on: Feb 12, 2026
 *      Author: Nam Ha
 */

#include "CINA226.h"

CINA226::CINA226(I2C_HandleTypeDef* hi2c,uint8_t deviceAddress,float rShunt,float maxCurrent):CI2cDevice(hi2c,deviceAddress){

}

bool CINA226::hasFailed(){

	return false;
}

/*
 */
void CINA226::calibrate(float currentLsb, float rShunt){

}

/*
 * doc gia tri Shunt-Spannung va dua ra gia tri Volt
 */

float CINA226::readShuntVoltage(){

		return 0;
}

float CINA226::readBusVoltage(){

	return 0;
}

float CINA226::readCurrent(){

	return 0;
}

float CINA226::readPower(){

	return 0;
}

bool CINA226::isConnected(){

	return true;
}
