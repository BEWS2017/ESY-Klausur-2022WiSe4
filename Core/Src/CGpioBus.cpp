/*
 * CGpioBus.cpp
 *
 *  Created on: Apr 30, 2022
 *      Author: Will
 */

#include "CGpioBus.h"

CGpioBus::CGpioBus() {
	// TODO Auto-generated constructor stub
	m_pins = NULL; //node head = NULL
}

CGpioBus::~CGpioBus() {
	// TODO Auto-generated destructor stub
	this->clear();
}

void CGpioBus::clear() {
	//Creates a tmp-pointer
	Pin *tmp;
	// run until reach the last node
	while (m_pins != NULL) {
		tmp = m_pins->next;
		delete m_pins;
		m_pins = tmp;
	}
}

/////////////////////////////////////////////////////////////////////

CGpioBus& CGpioBus::deepCopyList(const CGpioBus &orig) {
	Pin *pCurrent = NULL; // Current Pointer;
	Pin *pNext = NULL; // Next Pointer;
	//m_pins = new Pin; //if allocate new memory here then when orig.m_pins == NULL
	//when orig.m_pins == NULL we gotta delete m_pin and set it = NULL again
	//but this is not smart ;)

	//if orig.head node points at NULL
	//which means the linked list is empty
	if (orig.m_pins == NULL) {
		//then copied head node = NULL
		//delete m_pins;
		m_pins = NULL;
		return *this;
	} else {
		m_pins = new Pin;
		//copy orig.head's values
		m_pins->port = orig.m_pins->port;
		m_pins->mask = orig.m_pins->mask;

		pCurrent = m_pins;
		pNext = orig.m_pins->next;
	}
	//now after creating the head node, we can create next nodes and copy data from orginal
	while (pNext != NULL) {
		//allocate new memory for the next nodes (Pins)
		pCurrent->next = new Pin;
		pCurrent = pCurrent->next;
		//copy data
		pCurrent->mask = pNext->mask;
		pCurrent->port = pNext->port;

		pNext = pNext->next;
	}
	//end node then next pointer points at NULL
	pCurrent->next = NULL;
	return *this;
}

CGpioBus::CGpioBus(const CGpioBus &orig) {
	this->deepCopyList(orig);
}

CGpioBus& CGpioBus::operator=(const CGpioBus &orig) {
	//if size of the left operator is not equal to the right (orig)
	if (m_pins != NULL) {
		//then clears
		this->clear();
	}
	//performs deep copy
	return this->deepCopyList(orig);
}

///////////////////////////////////////////////////////////////////////////////////////////////

//CGpioBus::CGpioBus(const CGpioBus &orig) {
//	//	allocation of new memory
//	Pin *pCurrent = NULL; // Current Pointer;
//	Pin *pNext = NULL; // Next Pointer;
//	m_pins = new Pin;
//	if (orig.m_pins == NULL) {	//if orig.head node points at NULL
//		m_pins = NULL;			//then copied head node = NULL
//		return;					//return;
//	} else {
//		//copy orig.head's values
//		m_pins->port = orig.m_pins->port;
//		m_pins->mask = orig.m_pins->mask;
//
//		pCurrent = m_pins;
//		pNext = orig.m_pins->next;
//	}
//	//if the list is not empty, then copy data from the original
//	while (pNext != NULL) {
//		pCurrent->next = new Pin;
//		pCurrent = pCurrent->next;
//		pCurrent->mask = pNext->mask;
//		pCurrent->port = pNext->port;
//
//		pNext = pNext->next;
//	}
//	pCurrent->next = NULL;
//}
//
//CGpioBus& CGpioBus::operator=(const CGpioBus &orig) {
//	//	allocation of new memory
//	if(m_pins != NULL){
//		this->clear();
//	}
//
//	Pin *pCurrent = NULL; // pCurrent;
//	Pin *pNext = NULL; // pNext;
//	m_pins = new Pin;
//	if (orig.m_pins == NULL) {
//		m_pins = NULL;			//then copied head node = NULL
//		return *this;					//return;
//	} else {
//		//copy orig.head's values
//		m_pins->port = orig.m_pins->port;
//		m_pins->mask = orig.m_pins->mask;
//
//		pCurrent = m_pins;
//		pNext = orig.m_pins->next;
//	}
//	//if the list is not empty, then copy data from the original
//	while (pNext != NULL) {
//		pCurrent->next = new Pin;
//		pCurrent = pCurrent->next;
//		pCurrent->mask = pNext->mask;
//		pCurrent->port = pNext->port;
//
//		pNext = pNext->next;
//	}
//	pCurrent->next = NULL;
//	return *this;
//}

//////////////////////////////////////////////////////////////////////////////////
// GPIO_TypeDef *port: pointer to pointer
// GPIO_TypeDef &port: when using typedef Pin* pin
CGpioBus& CGpioBus::addPin(GPIO_TypeDef *port, uint16_t pinMask) {
	//Creates new memory for the new pin and initialize attributes with the transfer parameters (makes a new node).
	Pin *aPin = new Pin;
	aPin->mask = pinMask;
	aPin->port = port;
	aPin->next = NULL;

//		Checks if the linked list is empty
//	 	then adds the new Pin to the end of the list
//	 	otherwise: insertLast (or insertFirst)

	if (m_pins == NULL) {	//the linked list is empty
		m_pins = aPin;		//then add aPin to the first position of the list
	}

//Finds the last Pin (Node) of the list
//then add the new Pin after it.
	else {
		//creates a pointer to "run" from the first node to the last node
		Pin *tmp = m_pins;
		while (tmp->next != NULL) {
			//find the last node (pin)
			tmp = tmp->next;
		}
		tmp->next = aPin;	//the tmp Node points to the new Node
	}
//	else {			//InsertFirst but it was not required in the exercise
//		aPin->next = m_pins;
//		m_pins = aPin;
//	}
	return *this;
}

CGpioBus& CGpioBus::write(uint16_t value) {
	return *this = value;
}

CGpioBus& CGpioBus::operator =(uint16_t value) {
	Pin *tmp = m_pins;
	uint16_t bitmask = 0x01;	//Mask of the first pin
	while (tmp != NULL) {		//run from head to tail
		//uses AND to check, which pins were set.
		if ((bitmask & value) == bitmask) {
			HAL_GPIO_WritePin(tmp->port, tmp->mask, GPIO_PIN_SET);
		} else if ((bitmask & value) == 0) {
			HAL_GPIO_WritePin(tmp->port, tmp->mask, GPIO_PIN_RESET);
		}
		tmp = tmp->next; // points to the next pin
		bitmask = bitmask << 1; // shifts 1 bit to left
	}
	return *this;
}

uint16_t CGpioBus::read() const {
	Pin *tmp = m_pins;
	uint16_t bitmask = 0x01;	//Mask of the first pin
	while (tmp != NULL) {
		//if button is pressed
		if ((HAL_GPIO_ReadPin(tmp->port, tmp->mask)) == GPIO_PIN_SET) {
			return bitmask;
		}
		bitmask *= 2;
		tmp = tmp->next; // points to the next pin
	}
	return bitmask;
}

CGpioBus::operator uint16_t() const {
	return read();
}

