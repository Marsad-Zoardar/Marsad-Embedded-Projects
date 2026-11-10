/*
 * gpio_registers.h
 *
 *  Created on: Oct 23, 2026
 *      Author: zmmarsad
 */

#ifndef GPIO_REGISTERS_H_
#define GPIO_REGISTERS_H_


#include <stdint.h>

#define GPIOA_BASE 0x40020000u
#define GPIOA_MODER (*((volatile uint32_t *)(GPIOA_BASE + 0x00u)))
#define GPIOA_ODR (*((volatile uint32_t *)(GPIOA_BASE + 0x014u)))
#define GPIOA_AFRL (*((volatile uint32_t *)(GPIOA_BASE + 0x20u)))

#define RCC_BASE 0x40023800u
#define RCC_AHB1ENR (*((volatile uint32_t *)(RCC_BASE + 0x30u)))
#define RCC_APB1ENR (*((volatile uint32_t *)(RCC_BASE + 0x40u)))
#define RCC_CFGR (*((volatile uint32_t *)(RCC_BASE + 0x08u)))

#define USART2_BASE  0x40004400u
#define USART2_SR (*((volatile uint32_t *)(USART2_BASE + 0x00u)))
#define USART2_DR (*((volatile uint32_t *)(USART2_BASE + 0x04u)))
#define USART2_BRR (*((volatile uint32_t *)(USART2_BASE + 0x08u)))
#define USART2_CR1 (*((volatile uint32_t *)(USART2_BASE + 0x0Cu)))


#endif /* GPIO_REGISTERS_H_ */
