/*
 * systick_registers.h
 *
 *  Created on: Oct 24, 2026
 *      Author: Marsad Zoardar
 */

#ifndef SYSTICK_REGISTERS_H_
#define SYSTICK_REGISTERS_H_

#include <stdint.h>

#define SYSTICK_BASE 0xE000E010u
#define SYST_CSR (*((volatile uint32_t *)(SYSTICK_BASE)))
#define SYST_RVR (*((volatile uint32_t *)(SYSTICK_BASE + 0x04u)))
#define SYST_CVR (*((volatile uint32_t *)(SYSTICK_BASE + 0x08u)))


#endif /* SYSTICK_REGISTERS_H_ */
