/*
 * CGpioPin.h
 *
 *  Created on: 09.04.2021
 *      Author: mnl
 */

#ifndef SRC_CGPIOPIN_H_
#define SRC_CGPIOPIN_H_

#include "stm32f4xx_hal.h"

/**
 * Wrapper for simplified usage of the GPIO HAL functions.
 */
class CGpioPin {
private:

	/**
	 * Represents a GPIO pin configuration.
	 */
	class CConfiguration {
	public:
		GPIO_TypeDef *m_port = 0;
		GPIO_InitTypeDef m_data = { .Mode = GPIO_MODE_INPUT,
			.Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_LOW,
			.Alternate = 0};

		/**
		 * Sets the port to use.
		 *
		 * @param port the port
		 */
		CConfiguration& port(GPIO_TypeDef* port) {
			m_port = port;
			return *this;
		}

		/**
		 * Sets the pin to use.
		 *
		 * @param pin the pin number (0...15).
		 */
		CConfiguration& pin(uint8_t pin) {
			m_data.Pin = 1 << pin;
			return *this;
		}

		/**
		 * Sets the pin mode.
		 *
		 * @param mode the mode
		 */
		CConfiguration& mode(uint32_t mode) {
			m_data.Mode = mode;
			return *this;
		}

		/**
		 * Sets the pin's push/pull mode.
		 *
		 * @param pull the mode
		 */
		CConfiguration& pull(uint32_t pull) {
			m_data.Pull = pull;
			return *this;
		}

		/**
		 * Sets the pin's speed.
		 *
		 * @param speed the speed
		 */
		CConfiguration& speed(uint32_t speed) {
			m_data.Speed = speed;
			return *this;
		}

		/**
		 * Selects an alternate function for the pin.
		 *
		 * @param alternate specifies the function
		 */
		CConfiguration& alternate(uint32_t alternate) {
			m_data.Alternate = alternate;
			return *this;
		}
	};

	CConfiguration m_configuration;

public:
	/**
	 * Create a new configuration that can be modified as required and
	 * can then be pass to the {@link CGpioPin}'s constructor.
	 *
	 * @return a configuration with all values set to their respective defaults
	 */
	static CConfiguration defaults() {
		return CConfiguration();
	}

	/**
	 * Configures a GPIO pin by calling {@link HAL_GPIO_Init} with the
	 * configuration specified and creates an object that can be used
	 * to control the pin.
	 *
	 * @param configuration the desired configuration
	 */
	CGpioPin(CConfiguration configuration) {
		m_configuration = configuration;
		HAL_GPIO_Init(configuration.m_port, &configuration.m_data);
	}

	/**
	 * Creates an object that can be used to control a pin that has
	 * already been initialized by other means (e.g. by the CubeIDE
	 * startup code).
	 *
	 * @param port the pin's port
	 * @param the pin number (0...15)
	 */
	CGpioPin(GPIO_TypeDef* port, uint8_t pin) {
		m_configuration.m_port = port;
		m_configuration.m_data.Pin = 1 << pin;
	}

	/**
	 * Turns the pin on (true) or off (false). Effectively calls
	 * {@link HAL_GPIO_WritePin}.
	 *
	 * @param on the desired state.
	 * @return the object representing the pin
	 */
	CGpioPin& setState(bool on) {
		HAL_GPIO_WritePin(m_configuration.m_port, m_configuration.m_data.Pin,
				on ? GPIO_PIN_SET : GPIO_PIN_RESET);
		return *this;
	}

	/**
	 * Turn the pin on (true) or off (false). Effectively calls
	 * {@link HAL_GPIO_WritePin}.
	 *
	 * @param on the desired state.
	 * @return the object representing the pin
	 */
	CGpioPin& operator=(bool on) {
		return setState(on);
	}

	/**
	 * Returns the state of the pin. Effectively calls {@link HAL_GPIO_ReadPin}.
	 *
	 * @return the state as bool (true if pin set)
	 */
	bool state() {
		return HAL_GPIO_ReadPin(m_configuration.m_port,
				m_configuration.m_data.Pin) == GPIO_PIN_SET ? true : false;
	}

	/**
	 * Returns the state of the pin. Effectively calls {@link HAL_GPIO_ReadPin}.
	 *
	 * @return the state as bool (true if pin set)
	 */
	operator bool () {
		return state();
	}
};

#endif /* SRC_CGPIOPIN_H_ */
