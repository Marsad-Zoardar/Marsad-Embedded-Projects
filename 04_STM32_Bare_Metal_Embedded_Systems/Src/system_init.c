#include <stdint.h>

#define SCB_CPACR (*((volatile uint32_t *) 0xE000ED88u))

void SystemInit(void){

	/*FPU initialization will go here*/
	SCB_CPACR |= ((3u << 20) | (3u << 22));//(0xFu << 20) or (15u << 20) > all are valid

	__asm volatile ("dsb");
	__asm volatile ("isb");

}
