

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "main.h"
#include "tim.h"
#include "delay.h"
#include "stdbool.h"
#include <stdlib.h>
#include "stdio.h"
#include "freertos.h"



void HCSR04_Trigger1(void);
void HCSR04_Trigger2(void);
void HCSR04_Trigger3(void);

void setLeftMotor(uint8_t forward, uint16_t pwm) ;

void setRightMotor(uint8_t forward, uint16_t pwm) ;


bool checkEscapeCondition();

void escapeRoutine();



void dir(uint8_t dir);

void moving();



#endif /* INC_ULTRASONIC_H_ */
