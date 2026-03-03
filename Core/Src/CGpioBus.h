/*
 * CGpioPin.h
 *
 *  Created on: 09.04.2021
 *      Author: mnl
 */

#ifndef SRC_CGPIOBUS_H_
#define SRC_CGPIOBUS_H_

#include "stm32f4xx_hal.h"

/**
 * Models a list of pins as a bus and manipulates them using HAL functions.
 */
class CGpioBus {
private:
	/**
	 * The GPIO pins associated with the bus are stored as a link list
	 * of pairs of pointers to the HAL GPIO typedef and a mask for the pin.
	 */
	struct Pin {
		/** A pointer to the port's registers. */
		GPIO_TypeDef *port;
		/** A bit mask that denotes the pin. */
		uint16_t mask;
		/** A pointer to the next pin description of 0. */
		Pin *next;
	};
//	typedef struct Pin* pin;

	Pin *m_pins; //node head

	/**
	 * Clear linked list
	 */
	void clear();

	/** a private method would be used to perform deep copy
	 *  this method would be used in operator= and copy constructor
	 */
	CGpioBus& deepCopyList(const CGpioBus &orig);

public:
	/**
	 * Creates a new bus without any pins.
	 */
	CGpioBus();

	/**
	 * Copy constructor (required for deep copy).
	 */
	CGpioBus(const CGpioBus &orig);

	/**
	 * Assignment operator (required for deep copy behavior).
	 *
	 * @return the object by reference
	 */
	CGpioBus& operator=(const CGpioBus &src);

	/**
	 * Cleans up allocated resources.
	 */
	~CGpioBus();

	/**
	 * Adds a pin (specified by port and pin's mask) to the bus.
	 *
	 * See write(uint16_t value) for more information about the
	 * usage of the added pins.
	 *
	 * @param port the port
	 * @param pinMask the pin's mask (0x1, 0x2, 0x4, 0x8, 0x10, ...).
	 * @return the object for easy chaining
	 */
	CGpioBus& addPin(GPIO_TypeDef *port, uint16_t pinMask);

	/**
	 * Sets the pins of the bus according to the given value.
	 *
	 * The least significant bit of the given value (bit 0) determines the
	 * state of the GPIO pin added to the bus first. The next bit (bit 1)
	 * determines the state of the GPIO pin added to the bus as second
	 * pin and so on.
	 *
	 * The GPIO pin is set to high if the respective bit in value is set
	 * and set to low if the respective pin is reset (0).
	 *
	 * @param value the value to process
	 * @return the object for easy chaining
	 */
	CGpioBus& write(uint16_t value);

	/**
	 * Delegates to write(uint16_t value).
	 *
	 * @param value the value to use
	 * @return the object for easy chaining
	 */
	CGpioBus& operator=(uint16_t value);

	/**
	 * Reads the pins of the bus.
	 *
	 * The GPIO pin added to the bus first determines the
	 * value of the least significant bit (bit 0) of the value returned.
	 * The returned value's next bit (bit 1) is read from the GPIO pin
	 * added to the bus as second pin and so on.
	 *
	 * The bit of the value returned is 0 if the GPIO pin detects
	 * low voltage and it is 1 if the GPIO pin detects high voltage.
	 *
	 * @param value the value to process
	 * @return the value on the bus
	 */
	uint16_t read() const;

	/**
	 * Delegates to uint16_t read().
	 *
	 * @return the value on the bus
	 */
	operator uint16_t() const;
};

#endif /* SRC_CGPIOBUS_H_ */
