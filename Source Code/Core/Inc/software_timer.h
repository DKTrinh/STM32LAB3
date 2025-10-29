/*
 * software_timer.h
 *
 *  Created on: Oct 28, 2025
 *      Author: ADMIN
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define TIMER_INTERVAL 10

extern int timer0_flag;
extern int led_7_flag;
extern int blink_flag;
extern int longpress_flag;

void setTimer(int duration);
void setLED7Timer(int duration);
void setBlinkLedTimer(int duration);
void longpressTimer(int duration);
void timer_run(void);


#endif /* INC_SOFTWARE_TIMER_H_ */
