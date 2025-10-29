/*
 * input_reading.h
 *
 *  Created on: Oct 28, 2025
 *      Author: ADMIN
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

void readButtonInit(void);
void updateButtonStatus(void);
unsigned char getButtonState(unsigned char id);

#endif /* INC_INPUT_READING_H_ */
