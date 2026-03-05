
#ifndef CI2CDEVICE_H
#define CI2CDEVICE_H

#include "stm32f4xx_hal.h"
#include <cstdint>
#include <ctype.h>
#include "i2c.h"

/**
 * Basisklasse für I2C-Geräte am STM32F401RE.
 * Kapselt die grundlegenden I2C-Lese- und Schreiboperationen
 * über die STM32 HAL-Bibliothek.
 */
class CI2cDevice {
private:
    I2C_HandleTypeDef* m_hi2c;
    uint8_t m_deviceAddress;

protected:
    /**
     * Schreibt "length" Bytes an den I2C-Slave.
     * "data" zeigt auf das erste zu sendende Byte.
     * Gibt true zurück, wenn die Übertragung erfolgreich war,
     * false sonst.
     */
    bool write(uint8_t* data, uint16_t length);

    /*
     * Same as above but with DMA
     */
    bool writeDMA(uint8_t* data, uint16_t length);

    /**
     * Liest "length" Bytes vom I2C-Slave.
     * "data" zeigt auf den Puffer, in den gelesen wird.
     * Gibt true zurück, wenn die Übertragung erfolgreich war,
     * false sonst.
     */
    bool read(uint8_t* data, uint16_t length);

    /*
     * Same as above but with DMA
     */
    bool readDMA(uint8_t* data, uint16_t length);

    /**
     * Schreibt in das Register mit der Adresse "regAddress"
     * einen 16-Bit Wert (Big Endian).
     * Gibt true zurück, wenn die Übertragung erfolgreich war,
     * false sonst.
     */
    bool writeRegister(uint8_t regAddress, uint16_t value);

    /**
     * Liest aus dem Register mit der Adresse "regAddress"
     * einen 16-Bit Wert (Big Endian).
     * Schreibt den gelesenen Wert in "value".
     * Gibt true zurück, wenn die Übertragung erfolgreich war,
     * false sonst.
     */
    bool readRegister(uint8_t regAddress, uint16_t& value);

public:
    /**
     * Erzeugt ein neues CI2cDevice-Objekt.
     * "hi2c"          zeigt auf den konfigurierten I2C-Controller.
     * "deviceAddress" ist die 7-Bit Slave-Adresse des Geräts.
     */
    CI2cDevice(I2C_HandleTypeDef* hi2c, uint8_t deviceAddress);
};

#endif
