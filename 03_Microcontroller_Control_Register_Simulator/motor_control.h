
#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

/*
 * CONTROL register (8-bit)
 *
 * Bit 7   : Motor enable
 * Bit 6   : Motor direction
 * Bits 5:4: Operating mode
 * Bits 3:2: Motor speed
 * Bits 1:0: Reserved
 */
/*
 * STATUS register (8-bit)
 *
 * Bit 1 : READY
 * Bit 2 : LOW_VOLTAGE
 * Bit 3 : OVERHEAT
 *
 * Motor can run only when READY is set
 * and both fault bits are clear.
 */

uint8_t setMotorEnable(uint8_t control, bool enable);
uint8_t setMotorDirection(uint8_t control, bool direction);
uint8_t setMotorMode(uint8_t control, uint8_t mode);
uint8_t setMotorSpeed(uint8_t control, uint8_t speed);
uint8_t getMotorSpeed(uint8_t control);
bool canMotorRun(uint8_t status);

#endif /* MOTOR_CONTROL_H */

