//Author: Md Marsad Zoardar
//Date: October 16, 2026
//Program: Microcontroller control register simulator


#include "motor_control.h"

#define SPEED_MASK 0x03u
#define SPEED_SHIFT 2u

#define ENABLE_MASK 0x01u
#define ENABLE_SHIFT 7u

#define DIRECTION_MASK 0x01u
#define DIRECTION_SHIFT 6u

#define MODE_MASK 0x03u
#define MODE_SHIFT 4u

#define READY_MASK 0x01u
#define READY_SHIFT 1u

#define FAULT_MASK 0x03u
#define FAULT_SHIFT 2u 


//Set Motor Enable
uint8_t setMotorEnable(uint8_t control, bool enable){
/*Requirements:
- If enable == true, set bit 7.
- If enable == false, clear bit 7.
- Preserve all other bits.
- Return the updated CONTROL register.
*/
    if(enable){
        return control | (ENABLE_MASK << ENABLE_SHIFT);
    }else{
        return control & ~(ENABLE_MASK << ENABLE_SHIFT);
    }

}

//Set Motor Direction
uint8_t setMotorDirection(uint8_t control, bool direction){
/*Requirements:
- false means clockwise.
- true means counterclockwise.
- Modify only bit 6.
- Return the updated register.
*/
    if(direction){
        return control | (DIRECTION_MASK << DIRECTION_SHIFT);
    }else{
        return control & ~(DIRECTION_MASK << DIRECTION_SHIFT);
    }

}

//Set Motor Mode
uint8_t setMotorMode(uint8_t control, uint8_t mode){
/*Requirements:
- Update bits [5:4].
- Accept only values from 0 to 3.
- If mode > 3, return the original register unchanged.
- Preserve all other bits.*/
    if(mode > 3u){
        return control;
    }
   
    control &= ~(MODE_MASK << MODE_SHIFT);
    control |= (mode << MODE_SHIFT);
    
    return control;
}

//Set Motor Speed
uint8_t setMotorSpeed(uint8_t control, uint8_t speed){
/*Requirements:
- Update bits [3:2].
- Accept only values from 0 to 3.
- If speed > 3, return the original register unchanged.
- Preserve all other bits.*/
    if(speed > 3u){
        return control;
    }

    control &= ~(SPEED_MASK << SPEED_SHIFT);
    control |= (speed << SPEED_SHIFT);
    
    return control;
    
}

//Function to get the motor run speed
uint8_t getMotorSpeed(uint8_t control){
/*Requirements:
- Extract bits [3:2].
- Return the speed as an integer between 0 and 3.
- Do not modify the register.
*/ 
    
    return (control & (SPEED_MASK << SPEED_SHIFT)) >> SPEED_SHIFT;

}

//Function to check if fault present or motor is ready to run
bool canMotorRun(uint8_t status){
/*Requirements:
- Return true only when READY is set and neither fault is present.
- Otherwise return false.
- Do not modify status.*/

    uint8_t readyMask = (READY_MASK << READY_SHIFT);
    uint8_t faultMask = (FAULT_MASK << FAULT_SHIFT);
    if(((status & faultMask) == 0) && ((status & readyMask) != 0u)) {
        return true;
    }else{
        return false;
    }
}


