/*
 * led_display.h
 *
 *  Created on: Oct 27, 2025
 *      Author: ADMIN
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

void resetAllLEDs(void);
void scanLED7(uint8_t pos);
void showLED7(uint8_t pos);
void setAllClockBuffer(uint8_t t1, uint8_t t2);
void setHoriClockBuffer(uint8_t t);
void setVertClockBuffer(uint8_t t);

#endif /* INC_LED_DISPLAY_H_ */
