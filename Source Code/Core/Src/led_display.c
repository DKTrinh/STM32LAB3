/*
 * led_display.c
 *
 * Created on: Oct 27, 2025
 * Author: ADMIN
 */
#include "main.h"
#include "led_display.h"


#define LED7_COUNT 4

static uint8_t led7Map[10] = {
    0x3f, 0x06, 0x5b, 0x4f,
    0x66, 0x6d, 0x7d, 0x07,
    0x7f, 0x67
};

static uint8_t digitValue[LED7_COUNT];
static uint8_t ledData[LED7_COUNT];

void resetAllLEDs(void){
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_SET);
}

void setAllClockBuffer(uint8_t t1, uint8_t t2){
    digitValue[0] = t1 / 10;
    digitValue[1] = t1 % 10;
    digitValue[2] = t2 / 10;
    digitValue[3] = t2 % 10;
}

void setHoriClockBuffer(uint8_t t){
    digitValue[0] = t / 10;
    digitValue[1] = t % 10;
}

void setVertClockBuffer(uint8_t t){
    digitValue[2] = t / 10;
    digitValue[3] = t % 10;
}

void scanLED7(uint8_t pos){
    resetAllLEDs();
    ledData[pos] = led7Map[digitValue[pos]];
    showLED7(pos);
    switch(pos){
        case 0:
            HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, GPIO_PIN_RESET);
            break;
        case 1:
            HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_RESET);
            break;
        case 2:
            HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, GPIO_PIN_RESET);
            break;
        case 3:
            HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}

void showLED7(uint8_t pos){
    HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, !((ledData[pos] >> 0) & 1));
    HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, !((ledData[pos] >> 1) & 1));
    HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, !((ledData[pos] >> 2) & 1));
    HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, !((ledData[pos] >> 3) & 1));
    HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, !((ledData[pos] >> 4) & 1));
    HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, !((ledData[pos] >> 5) & 1));
    HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, !((ledData[pos] >> 6) & 1));
}
