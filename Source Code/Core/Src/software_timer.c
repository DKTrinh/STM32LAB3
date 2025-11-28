/*
 * software_timer.c
 *
 *  Created on: Oct 27, 2025
 *      Author: ADMIN
 */
#include "main.h"
#include "input_reading.h"
#include "software_timer.h"

static int counter_main;
static int counter_led7;
static int counter_blink;

int timer0_flag = 0;
int led_7_flag = 0;
int blink_flag = 0;
int longpress_flag = 0;

void setTimer(int duration){
    counter_main = duration / TIMER_INTERVAL;
    timer0_flag = 0;
}

void setLED7Timer(int duration){
    counter_led7 = duration / (2 *TIMER_INTERVAL);
    led_7_flag = 0;
}

void setBlinkLedTimer(int duration){
    counter_blink = duration / TIMER_INTERVAL;
    blink_flag = 0;
}

void longpressTimer(int duration){
    longpress_flag = duration / TIMER_INTERVAL;
}

void timer_run(void){
    if(counter_main > 0){
        counter_main--;
        if(counter_main == 0) timer0_flag = 1;
    }
    if(counter_led7 > 0){
        counter_led7--;
        if(counter_led7 == 0) led_7_flag = 1;
    }
    if(counter_blink > 0){
        counter_blink--;
        if(counter_blink == 0) blink_flag = 1;
    }
}
