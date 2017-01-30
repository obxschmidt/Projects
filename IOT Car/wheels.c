//------------------------------------------------------------------------------
//
//  Description: This file contains the functions for wheel movement using PWM
//
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

void Left_Wheel_Forward(int left_forward_rate) {
  Left_Wheel_Off();
  TB1CCR1 = left_forward_rate;
}

void Left_Wheel_Reverse(int left_reverse_rate) {
  Left_Wheel_Off();
  TB1CCR2 = left_reverse_rate;
}

void Right_Wheel_Forward(int right_forward_rate) {
  Right_Wheel_Off();
  TB2CCR1 = right_forward_rate;
}

void Right_Wheel_Reverse(int right_reverse_rate) {
  Right_Wheel_Off();
  TB2CCR2 = right_reverse_rate;
}

void Left_Wheel_Off(void) {
  TB1CCR1 = CLEAR;
  TB1CCR2 = CLEAR; 
}

void Right_Wheel_Off(void) {
  TB2CCR1 = CLEAR;
  TB2CCR2 = CLEAR;
}









