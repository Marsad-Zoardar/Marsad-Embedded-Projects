Microcontroller Control Register Simulator
Language: C99
Category: Embedded Systems / Register-Level Programming
Hardware: None required
Testing: 50 assertions using the C standard library
********************************************************************************************
1. Overview
This project implements a modular C99 library that simulates register-level configuration and status checking for a motor controller.

The simulated controller uses two 8-bit registers:
- CONTROL: Configures motor enable, direction, operating mode, and speed.
- STATUS: Indicates controller readiness and fault conditions.
The library provides six functions for manipulating register fields and evaluating motor readiness.

The implementation uses bitwise operations, masks, and shifts to modify individual fields while preserving unrelated bits.

The project includes an automated test suite and a Makefile for reproducible compilation and testing.

********************************************************************************************
2. Learning Objectives
This project demonstrates:
- Bitwise operations and register field manipulation.
- Bit masking and shifting.
- Preservation of unrelated register bits.
- Input validation for multi-bit fields.
- Modular C programming using header and source files.
- Fixed-width integer types from <stdint.h>.
- Boolean operations using <stdbool.h>.
- Unit testing with <assert.h>.
- Compilation, linking, and incremental builds using Make.

********************************************************************************************
3. Register Architecture
CONTROL Register
The CONTROL register is 8 bits wide.
Bit:    7   6   5   4   3   2   1   0
       +---+---+---+---+---+---+---+---+
       | E | D | M | M | S | S | R | R |
       +---+---+---+---+---+---+---+---+

Field	    Bits	    Description
ENABLE	    7	        Motor enable/disable
DIRECTION	6	        Clockwise/counterclockwise
MODE	    5:4	        Motor operating mode
SPEED	    3:2	        Motor speed selection
RESERVED	1:0	        Preserved during register updates


ENABLE: 0 = disabled, 1 = enabled.

DIRECTION: 0 = clockwise, 1 = counterclockwise.

MODE: 0 = Idle, 1 = Manual, 2 = Automatic, 3 = Calibration.

SPEED: 0 = Stopped, 1 = Low, 2 = Medium, 3 = High.

All configuration functions preserve register bits outside their target fields.
STATUS Register
Bit:    7   6   5   4   3   2   1   0
       +---+---+---+---+---+---+---+---+
       | X | X | X | X | O | V | R | X |
       +---+---+---+---+---+---+---+---+

Field	    Bit	    Description
READY	    1	    Controller ready
LOW_VOLTAGE	2	    Low-voltage fault
OVERHEAT	3	    Overheat fault


The motor is permitted to run only when READY is set and both fault bits are clear.

Other STATUS bits are ignored by the readiness check.

************************************************************************************************
4. Library Interface

The public interface is declared in motor_control.h.

uint8_t setMotorEnable(uint8_t control, bool enable);
uint8_t setMotorDirection(uint8_t control, bool direction);
uint8_t setMotorMode(uint8_t control, uint8_t mode);
uint8_t setMotorSpeed(uint8_t control, uint8_t speed);
uint8_t getMotorSpeed(uint8_t control);
bool canMotorRun(uint8_t status);

Function Descriptions
-------------------------------------------------------
Function	Responsibility

setMotorEnable()	Sets or clears the motor enable bit
setMotorDirection()	Configures the motor direction bit
setMotorMode()	Updates the two-bit operating mode
setMotorSpeed()	Updates the two-bit speed field
getMotorSpeed()	Extracts the configured speed
canMotorRun()	Evaluates READY and fault conditions


All register values are passed by value.

The setter functions return updated register values without modifying caller-owned variables directly.

Invalid MODE or SPEED values greater than 3 cause the corresponding setter to return the original CONTROL value unchanged.

*****************************************************************************************************
5. Implementation Design
Register Field Preservation
Register configuration functions use bit masks and shifts to modify only the intended field.

For multi-bit fields, the implementation:
1. Validates the requested field value.
2. Clears the existing field bits.
3. Shifts the new value into position.
4. Combines the new field value with the preserved register bits.
This prevents unrelated configuration settings from being overwritten.
Modular Design
The project separates responsibilities across three C files:

motor_control.h

Declares the public library interface and documents the register layout.

motor_control.c

Implements register manipulation and status checking.

test_motor_control.c

Contains the automated test suite and its main() entry point.

The library implementation does not depend on the test runner.

*******************************************************************************************************
6. Building the Project
Requirements
- GCC with C99 support.
- GNU Make.
- A Linux development environment.
The supplied Makefile uses gcc-14.

To compile:
make

The build produces the motor_tests executable.

Compiler options include:
-std=c99 -Wall -Wextra -Wpedantic -g

These enable C99 compilation, additional diagnostics, and debugging information.

********************************************************************************************************
7. Running Unit Tests
Execute the test suite using:
make test

Alternatively:
./motor_tests

The suite contains 50 assertions across six test functions.
Test function	            Assertions
testCanMotorRun()	            6
testSetMotorSpeed()	            11
testSetMotorMode()	            10
testSetMotorEnable()	        9
testSetMotorDirection()	        10
testGetMotorSpeed()	            8
Total	                        54


The tests cover:
- Setting and clearing individual bits.
- Configuring all supported mode and speed values.
- Rejecting invalid mode and speed inputs.
- Preserving unrelated register bits.
- Extracting multi-bit fields.
- Evaluating READY and fault combinations.

Successful execution prints:
[Pass] canMotorRun!
[Pass] setMotorSpeed!
[Pass] setMotorMode!
[Pass] setMotorEnable!
[Pass] setMotorDirection!
[Pass] getMotorSpeed!
All 54 test cases have passed!

The test suite uses C's assert() macro. Tests must be compiled without NDEBUG, because defining it disables assertions.

********************************************************************************************************
8. Cleaning Build Files
Run:
make clean

This removes the generated object files and test executable without deleting source code.

********************************************************************************************************
9. Engineering Considerations
The implementation follows several design principles relevant to embedded firmware:

Fixed-width register representation: uint8_t provides an 8-bit unsigned type suitable for the simulated registers.

Input validation: Unsupported MODE and SPEED values are rejected without changing the original register.

Bit preservation: Register setters preserve all bits outside their target fields, including reserved bits.

Separation of concerns: Register operations are implemented independently of the test suite.

Reproducible builds: The Makefile tracks source and header dependencies and supports incremental compilation.

********************************************************************************************************
10. Limitations
This project is a software simulation of register-level operations.

It does not:
- Interface with physical microcontroller peripherals.
- Access memory-mapped hardware registers.
- Implement actual motor control or PWM generation.
- Model timing, interrupts, or hardware concurrency.
- Simulate real voltage or temperature measurements.
The library demonstrates register manipulation techniques that can be extended to hardware-oriented firmware.

*********************************************************************************************************
11. Future Improvements
Potential extensions include:
- Defining a register abstraction for memory-mapped peripherals.
- Introducing volatile register access in a hardware-oriented implementation.
- Expanding tests to cover all possible 8-bit register inputs.
- Integrating register manipulation with an STM32 peripheral driver.

                                                              --*--
