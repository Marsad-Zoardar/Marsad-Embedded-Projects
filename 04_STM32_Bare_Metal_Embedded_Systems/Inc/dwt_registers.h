/*
 * dwt_registers.h
 *
 *  Created on: Oct 26, 2026
 *      Author: zmmarsad
 */

#ifndef DWT_REGISTERS_H_
#define DWT_REGISTERS_H_

#include <stdint.h>

#define DWT_DEMCR (*((volatile uint32_t *)(0xE000EDFC)))
#define DWT_CTRL (*((volatile uint32_t *)(0xE0001000)))
#define DWT_CYCCNT (*((volatile uint32_t *)(0xE0001004)))


#endif /* DWT_REGISTERS_H_ */
