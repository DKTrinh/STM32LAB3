/*
 * input_reading.c
 *
 *  Created on: Oct 27, 2025
 *      Author: ADMIN
 */
#include "main.h"
#include "input_reading.h"

#define TOTAL_BUTTONS 3
#define STATE_PRESSED GPIO_PIN_RESET
#define STATE_RELEASED GPIO_PIN_SET

static GPIO_PinState stableState[TOTAL_BUTTONS];
static GPIO_PinState sample1[TOTAL_BUTTONS];
static GPIO_PinState sample2[TOTAL_BUTTONS];

static uint16_t btnPin[TOTAL_BUTTONS] = {BUTTON_0_Pin, BUTTON_1_Pin, BUTTON_2_Pin};

void readButtonInit(void){
    for(int j = 0; j < TOTAL_BUTTONS; j++){
        sample1[j] = STATE_RELEASED;
        sample2[j] = STATE_RELEASED;
        stableState[j] = STATE_RELEASED;
    }
}

void updateButtonStatus(void){
    for(int j = 0; j < TOTAL_BUTTONS; j++){
        sample2[j] = sample1[j];
        sample1[j] = HAL_GPIO_ReadPin(GPIOB, btnPin[j]);
        if(sample1[j] == sample2[j]){
            stableState[j] = sample1[j];
        }
    }
}

unsigned char getButtonState(unsigned char id){
    if(id >= TOTAL_BUTTONS) return 0;
    return (stableState[id] == STATE_PRESSED);
}
