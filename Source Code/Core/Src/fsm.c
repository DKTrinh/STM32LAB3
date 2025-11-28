/*
 * fsm.c
 *
 *  Created on: Nov 28, 2025
 *      Author: ADMIN
 */
#include "fsm.h"
#include "main.h"
#include "traffic_led.h"
#include "led_display.h"

#define RED 0
#define AMBER 1
#define GREEN 2

#define DEFAULT_RED 95
#define DEFAULT_AMBER 45
#define DEFAULT_GREEN 50

typedef enum { STATE_MODE1, STATE_MODE2, STATE_MODE3, STATE_MODE4 } ModeState;

static ModeState currentState = STATE_MODE1;


static uint8_t timeRed = DEFAULT_RED;
static uint8_t timeAmber = DEFAULT_AMBER;
static uint8_t timeGreen = DEFAULT_GREEN;

static uint8_t tempDisplayValue = 0;

static uint8_t isRedModified = 0;
static uint8_t isAmberModified = 0;
static uint8_t isGreenModified = 0;

static uint8_t isBalanceRequested = 0;

static uint8_t btn0Lock = 0, btn1Lock = 0, btn2Lock = 0;
static int firstRun = 0, segIndex = 0;

void balance_logic(void) {

    if ((uint16_t)timeAmber + (uint16_t)timeGreen > 99) {
        timeRed = DEFAULT_RED; timeAmber = DEFAULT_AMBER; timeGreen = DEFAULT_GREEN; return;
    }
    if (isRedModified) {
        if (isAmberModified) {
            if (timeRed <= timeAmber) { timeGreen = 1; timeRed = timeAmber + timeGreen; }
            else { timeGreen = timeRed - timeAmber; }
        } else if (isGreenModified) {
            if (timeRed <= timeGreen) { timeAmber = 1; timeRed = timeGreen + timeAmber; }
            else { timeAmber = timeRed - timeGreen; }
        } else {
            uint16_t sum = (uint16_t)timeAmber + (uint16_t)timeGreen;
            if (timeRed < sum) timeRed = (uint8_t)sum;
            else if (timeRed > sum) timeGreen = timeRed - timeAmber;
        }
    } else {
        timeRed = timeAmber + timeGreen;
    }
    if (timeRed > 99) {
        timeRed = 99;
        if (timeAmber >= 98) timeAmber = 1;
        timeGreen = timeRed - timeAmber;
    }
}

void fsm_output_handle(void){
    switch(currentState){
        case STATE_MODE1:
            // --- MODE 1: CHẠY BÌNH THƯỜNG ---
            if(led_7_flag){ scanLED7(segIndex++); if(segIndex >= 4) segIndex = 0; setLED7Timer(250); }


            if(getButtonState(0)){
                if(!btn0Lock){
                    btn0Lock = 1;
                    currentState = STATE_MODE2;


                    isRedModified = 0;
                    isAmberModified = 0;
                    isGreenModified = 0;


                    isBalanceRequested = 0;

                    tempDisplayValue = timeRed;
                    LED_TRAFFIC_INIT();
                    setTimer(500);
                }
            } else btn0Lock = 0;

            if(timer0_flag){
                if(!firstRun){
                    LED_TRAFFIC_INIT();
                    LED_TRAFFIC_STORE_BUFFER(DEFAULT_RED, RED);
                    LED_TRAFFIC_STORE_BUFFER(DEFAULT_AMBER, AMBER);
                    LED_TRAFFIC_STORE_BUFFER(DEFAULT_GREEN, GREEN);
                    LED_TRAFFIC_LOAD_BUFFER();
                    firstRun = 1;
                }
                LED_TRAFFIC_RUN();
                setTimer(1000);
            }
            break;

        case STATE_MODE2:
            // --- MODE 2: CHỈNH ĐỎ ---
            setAllClockBuffer(tempDisplayValue, 2);
            if(led_7_flag){ scanLED7(segIndex++); if(segIndex >= 4) segIndex = 0; setLED7Timer(250); }
            if(blink_flag){ HAL_GPIO_TogglePin(LED_RED1_GPIO_Port, LED_RED1_Pin); HAL_GPIO_TogglePin(LED_RED2_GPIO_Port, LED_RED2_Pin); setBlinkLedTimer(250); }


            if(getButtonState(0)){
                if(!btn0Lock){
                    btn0Lock = 1;
                    if(isRedModified) timeRed = tempDisplayValue; // Lưu số nếu có sửa
                    currentState = STATE_MODE3;
                    tempDisplayValue = timeAmber;
                    setTimer(500);
                }
            } else btn0Lock = 0;


            if(getButtonState(1)){
                if(!btn1Lock){
                    btn1Lock = 1;
                    tempDisplayValue++;
                    if(tempDisplayValue > 99) tempDisplayValue = 1;
                    isRedModified = 1;
                }
            } else btn1Lock = 0;


            if(getButtonState(2)){
                if(!btn2Lock){
                    btn2Lock = 1;
                    isBalanceRequested = 1;
                }
            } else btn2Lock = 0;
            break;

        case STATE_MODE3:
            // --- MODE 3: CHỈNH VÀNG ---
            setAllClockBuffer(tempDisplayValue, 3);
            if(led_7_flag){ scanLED7(segIndex++); if(segIndex >= 4) segIndex = 0; setLED7Timer(250); }
            if(blink_flag){
                HAL_GPIO_WritePin(LED_RED1_GPIO_Port, LED_RED1_Pin, SET); HAL_GPIO_WritePin(LED_RED2_GPIO_Port, LED_RED2_Pin, SET);
                HAL_GPIO_TogglePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin); HAL_GPIO_TogglePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin);
                setBlinkLedTimer(250);
            }


            if(getButtonState(0)){
                if(!btn0Lock){
                    btn0Lock = 1;
                    if(isAmberModified) timeAmber = tempDisplayValue;
                    currentState = STATE_MODE4;
                    tempDisplayValue = timeGreen;
                    setTimer(500);
                }
            } else btn0Lock = 0;


            if(getButtonState(1)){
                if(!btn1Lock){
                    btn1Lock = 1;
                    tempDisplayValue++;
                    if(tempDisplayValue > 99) tempDisplayValue = 1;
                    isAmberModified = 1;
                }
            } else btn1Lock = 0;


            if(getButtonState(2)){
                if(!btn2Lock){
                    btn2Lock = 1;
                    isBalanceRequested = 1;
                }
            } else btn2Lock = 0;
            break;

        case STATE_MODE4:
            // --- MODE 4: CHỈNH XANH ---
            setAllClockBuffer(tempDisplayValue, 4);
            if(led_7_flag){ scanLED7(segIndex++); if(segIndex >= 4) segIndex = 0; setLED7Timer(250); }
            if(blink_flag){
                HAL_GPIO_WritePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin, SET); HAL_GPIO_WritePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin, SET);
                HAL_GPIO_TogglePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin); HAL_GPIO_TogglePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin);
                setBlinkLedTimer(250);
            }


            if(getButtonState(0)){
                if(!btn0Lock){
                    btn0Lock = 1;
                    if(isGreenModified) timeGreen = tempDisplayValue;


                    if (isBalanceRequested == 1) {
                        balance_logic();
                    }

                    LED_TRAFFIC_STORE_BUFFER(timeRed, RED);
                    LED_TRAFFIC_STORE_BUFFER(timeAmber, AMBER);
                    LED_TRAFFIC_STORE_BUFFER(timeGreen, GREEN);
                    LED_TRAFFIC_LOAD_BUFFER();
                    LED_TRAFFIC_INIT();

                    currentState = STATE_MODE1;
                    setTimer(1000);
                }
            } else btn0Lock = 0;

            if(getButtonState(1)){
                if(!btn1Lock){
                    btn1Lock = 1;
                    tempDisplayValue++;
                    if(tempDisplayValue > 99) tempDisplayValue = 1;
                    isGreenModified = 1;
                }
            } else btn1Lock = 0;


            if(getButtonState(2)){
                if(!btn2Lock){
                    btn2Lock = 1;
                    isBalanceRequested = 1;
                }
            } else btn2Lock = 0;
            break;

        default:
            currentState = STATE_MODE1;
            break;
    }
}
