/*
 * fsm.h
 *
 *  Created on: Nov 28, 2025
 *      Author: ADMIN
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"
#include "input_reading.h"
#include "software_timer.h"
#include "traffic_led.h"
#include "led_display.h"

void fsm_output_handle(void);
void fsm_balancing_run(void);


#endif /* INC_FSM_H_ */
