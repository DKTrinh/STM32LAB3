/*
 * input_processing.c
 *
 * Created on: Oct 28, 2025
 * Author: ADMIN
 */

#include "input_processing.h"

#define RED 0
#define AMBER 1
#define GREEN 2

typedef enum { STATE_MODE1, STATE_MODE2, STATE_MODE3, STATE_MODE4 } ModeState;

static ModeState currentState = STATE_MODE1;

static uint8_t timeRed = 0, timeAmber = 0, timeGreen = 0;
static uint8_t btn0Lock = 0, btn1Lock = 0, btn2Lock = 0;
static int firstRun = 0, segIndex = 0;

void fsm_output_handle(void){
    switch(currentState){
        case STATE_MODE1:
            if(led_7_flag){
                scanLED7(segIndex++);
                if(segIndex >= 4) segIndex = 0;
                setLED7Timer(250);
            }
            if(getButtonState(0)){
                if(!btn0Lock){
                    btn0Lock = 1;
                    currentState = STATE_MODE2;
                    timeRed = 0;
                    LED_TRAFFIC_INIT();
                    setTimer(1000);
                }
            } else btn0Lock = 0;
            if(timer0_flag){
                if(!firstRun){ LED_TRAFFIC_INIT(); firstRun = 1; }
                LED_TRAFFIC_RUN();
                setTimer(1000);
            }
            break;
        case STATE_MODE2:
            setAllClockBuffer(timeRed, 2);
            if(led_7_flag){
                scanLED7(segIndex++);
                if(segIndex >= 4) segIndex = 0;
                setLED7Timer(250);
            }
            if(blink_flag){
                HAL_GPIO_TogglePin(LED_RED1_GPIO_Port, LED_RED1_Pin);
                HAL_GPIO_TogglePin(LED_RED2_GPIO_Port, LED_RED2_Pin);
                setBlinkLedTimer(250);
            }
            if(getButtonState(0)){
                if(!btn0Lock){
                    btn0Lock = 1;
                    currentState = STATE_MODE3;
                    timeAmber = 0;
                    LED_TRAFFIC_INIT();
                    setTimer(1000);
                }
            } else btn0Lock = 0;
            if(getButtonState(1)){
                if(!btn1Lock){ btn1Lock = 1; timeRed = (timeRed + 1) % 99; }
            } else btn1Lock = 0;
            if(getButtonState(2)){
                if(!btn2Lock){ btn2Lock = 1; LED_TRAFFIC_STORE_BUFFER(timeRed, RED); }
            } else btn2Lock = 0;
            if(timer0_flag) setTimer(1000);
            break;
        case STATE_MODE3:
            setAllClockBuffer(timeAmber, 3);
            if(led_7_flag){
                scanLED7(segIndex++);
                if(segIndex >= 4) segIndex = 0;
                setLED7Timer(250);
            }
            if(blink_flag){
                HAL_GPIO_TogglePin(LED_AMBER1_GPIO_Port, LED_AMBER1_Pin);
                HAL_GPIO_TogglePin(LED_GREEN2_GPIO_Port, LED_GREEN2_Pin);
                setBlinkLedTimer(250);
            }
            if(getButtonState(0)){
                if(!btn0Lock){
                    btn0Lock = 1;
                    currentState = STATE_MODE4;
                    timeGreen = 0;
                    LED_TRAFFIC_INIT();
                    setTimer(1000);
                }
            } else btn0Lock = 0;
            if(getButtonState(1)){
                if(!btn1Lock){ btn1Lock = 1; timeAmber = (timeAmber + 1) % 99; }
            } else btn1Lock = 0;
            if(getButtonState(2)){
                if(!btn2Lock){ btn2Lock = 1; LED_TRAFFIC_STORE_BUFFER(timeAmber, AMBER); }
            } else btn2Lock = 0;
            if(timer0_flag) setTimer(1000);
            break;
        case STATE_MODE4:
            setAllClockBuffer(timeGreen, 4);
            if(led_7_flag){
                scanLED7(segIndex++);
                if(segIndex >= 4) segIndex = 0;
                setLED7Timer(250);
            }
            if(blink_flag){
                HAL_GPIO_TogglePin(LED_GREEN1_GPIO_Port, LED_GREEN1_Pin);
                HAL_GPIO_TogglePin(LED_AMBER2_GPIO_Port, LED_AMBER2_Pin);
                setBlinkLedTimer(250);
            }
            if(getButtonState(0)){
                if(!btn0Lock){
                    btn0Lock = 1;
                    currentState = STATE_MODE1;
                    LED_TRAFFIC_LOAD_BUFFER();
                    LED_TRAFFIC_INIT();
                    setTimer(1000);
                }
            } else btn0Lock = 0;
            if(getButtonState(1)){
                if(!btn1Lock){ btn1Lock = 1; timeGreen = (timeGreen + 1) % 99; }
            } else btn1Lock = 0;
            if(getButtonState(2)){
                if(!btn2Lock){ btn2Lock = 1; LED_TRAFFIC_STORE_BUFFER(timeGreen, GREEN); }
            } else btn2Lock = 0;
            if(timer0_flag) setTimer(1000);
            break;
        default:
            currentState = STATE_MODE1;
            break;
    }
}
