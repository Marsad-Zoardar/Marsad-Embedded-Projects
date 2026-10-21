
#include <stdio.h>
#include <assert.h>

#include "motor_control.h"

//Assert function to test
void testCanMotorRun(void){
    
    assert(canMotorRun(0x02u) == true);
    assert(canMotorRun(0x00u) == false);
    assert(canMotorRun(0x06u) == false);
    assert(canMotorRun(0x0Au) == false);
    assert(canMotorRun(0x0Eu) == false);
    assert(canMotorRun(0x03u) == true);

    printf("[Pass] canMotorRun!\n");
} 

void testSetMotorSpeed(void){
    
    assert(setMotorSpeed(0x00u, 0u) == 0x00u);
    assert(setMotorSpeed(0x00u, 1u) == 0x04u);
    assert(setMotorSpeed(0x00u, 2u) == 0x08u);
    assert(setMotorSpeed(0x00u, 3u) == 0x0Cu);
    assert(setMotorSpeed(0x0Cu, 1u) == 0x04u);
    assert(setMotorSpeed(0xFFu, 0u) == 0xF3u);
    assert(setMotorSpeed(0xA5u, 3u) == 0xADu);
    assert(setMotorSpeed(0xA5u, 5u) == 0xA5u);
    assert(setMotorSpeed(0xFFu, 4u) == 0xFFu);
    assert(setMotorSpeed(0x03u, 2u) == 0x0Bu);
    //invariant testing
    uint8_t original = 0xA5u; //1111 0011
    uint8_t modified = setMotorSpeed(original, 3u);
    assert((original & 0xF3u) == (modified & 0xF3u)); 
    
    printf("[Pass] setMotorSpeed!\n");
}

void testSetMotorMode(void){
    assert(setMotorMode(0x00u, 0u) == 0x00u);
    assert(setMotorMode(0x00u, 1u) == 0x10u);
    assert(setMotorMode(0x00u, 2u) == 0x20u);
    assert(setMotorMode(0x00u, 3u) == 0x30u);
    assert(setMotorMode(0x00u, 4u) == 0x00u);
    assert(setMotorMode(0xACu, 1u) == 0x9Cu);
    assert(setMotorMode(0xFFu, 0u) == 0xCFu);
    assert(setMotorMode(0xA5u, 3u) == 0xB5u);
    assert(setMotorMode(0xFFu, 5u) == 0xFFu);
    //invariant testing
    uint8_t original = 0xA5u; //1111 0011
    uint8_t modified = setMotorMode(original, 3u);
    assert((original & 0xCFu) == (modified & 0xCFu)); 
    
    printf("[Pass] setMotorMode!\n");
    
}

void testSetMotorEnable(void){
    assert(setMotorEnable(0xFFu, false) == 0x7Fu);
    assert(setMotorEnable(0x7Fu, true) == 0xFFu);
    assert(setMotorEnable(0x8Fu, false) == 0x0Fu);
    assert(setMotorEnable(0x0Fu, true) == 0x8Fu);
    assert(setMotorEnable(0xCFu, false) == 0x4Fu);
    assert(setMotorEnable(0x4Fu, true) == 0xCFu);
    assert(setMotorEnable(0xFFu, true) == 0xFFu);
    assert(setMotorEnable(0x7Fu, false) == 0x7Fu);
    //invariant testing
    uint8_t original = 0xA5u; //1111 0011
    uint8_t modified = setMotorEnable(original, 1u);
    assert((original & 0x7Fu) == (modified & 0x7Fu)); 
    
    printf("[Pass] setMotorEnable!\n");
}

void testSetMotorDirection(void){
    assert(setMotorDirection(0xFFu, false) == 0xBFu);
    assert(setMotorDirection(0x3Fu, true) == 0x7Fu);
    assert(setMotorDirection(0x8Fu, false) == 0x8Fu);
    assert(setMotorDirection(0x8Fu, true) == 0xCFu);
    assert(setMotorDirection(0xCFu, false) == 0x8Fu);
    assert(setMotorDirection(0x40u, false) == 0x00u);
    assert(setMotorDirection(0x00u, true) == 0x40u);
    assert(setMotorDirection(0x7Fu, false) == 0x3Fu);
    assert(setMotorDirection(0xFFu, true) == 0xFFu);
    //invariant testing
    uint8_t original = 0xA5u; //1111 0011
    uint8_t modified = setMotorDirection(original, 1u);
    assert((original & 0xBFu) == (modified & 0xBFu)); 
    
    printf("[Pass] setMotorDirection!\n");
}

void testGetMotorSpeed(void){
    
    assert(getMotorSpeed(0xADu) == 3u);
    assert(getMotorSpeed(0xF0u) == 0u);
    assert(getMotorSpeed(0x04u) == 1u);
    assert(getMotorSpeed(0xA8u) == 2u);
    assert(getMotorSpeed(0xFCu) == 3u);
    assert(getMotorSpeed(0x00u) == 0u);
    assert(getMotorSpeed(0x0Fu) == 3u);
    assert(getMotorSpeed(0x01u) == 0u);
    printf("[Pass] getMotorSpeed!\n");
}

int main(void){
    
    testCanMotorRun();
    testSetMotorSpeed();
    testSetMotorMode();
    testSetMotorEnable();
    testSetMotorDirection();
    testGetMotorSpeed();
    
    printf("All 54 test cases have passed!\n\n");
    
    return 0;
}


