//------------------------------------------------------------------------------
//
//  Description: This file contains the functions required for the timer
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

extern volatile unsigned char control_state[CNTL_STATE_INDEX];
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
extern int msec_count;
//
extern int SW1Press;
extern int SW2Press;
extern int SW1Debounce;
extern int SW2Debounce;
// PWM
extern int right_forward_rate;
extern int right_reverse_rate;
extern int left_forward_rate;
extern int left_reverse_rate;


void Init_Timers(void){  
//------------------------------------------------------------------------------
// Description: This function contains the Timer Configurations
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)  
//------------------------------------------------------------------------------
  Init_Timer_A0();      //
//  Init_Timer_A1();    // 
//  Init_Timer_B0();    // 
  Init_Timer_B1();      //  For PWN  
  Init_Timer_B2();      //  Required for provided compiled code to work
//------------------------------------------------------------------------------
}

//------------------------------------------------------------------------------
// Description: Waits five milliseconds * delay
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1) 
//------------------------------------------------------------------------------
void five_msec_delay(int delay){
  msec_count = CLEAR;
   TA0CCTL1 |= CCIE;
  while(msec_count < delay){
  }
    TA0CCTL1 &= ~CCIE;
}

void Init_Timer_A0(void) {
//------------------------------------------------------------------------------
// Description: Timer A0 initialization sets up both A0_0 and A0_1-A0_2
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1) 
//------------------------------------------------------------------------------
 TA0CTL = TASSEL__SMCLK; // SMCLK source
 TA0CTL |= TACLR; // Resets TA0R, clock divider, count direction
 TA0CTL |= MC__CONTINOUS; // Continuous up
 TA0CTL |= ID__8; // Divide clock by 8
 TA0CTL &= ~TAIE; // Disable Overflow Interrupt
 TA0CTL &= ~TAIFG; // Clear Overflow Interrupt flag

 TA0EX0 = TAIDEX_7; // Divide clock by an additional 8
 
 TA0CCR0 = TA0CCR0_INTERVAL; // CCR0 (6250)
 TA0CCTL0 &= ~CCIE; // CCR0 Disable interrupt

 TA0CCR1 = TA0CCR1_INTERVAL; // CCR1
 TA0CCTL1 &= ~CCIE; // CCR1 Disable interrupt

// TA0CCR2 = TA0CCR2_INTERVAL; // CCR2
// TA0CCTL2 |= CCIE; // CCR2 enable interrupt
}
//------------------------------------------------------------------------------ 

void Init_Timer_B1(void) {
//------------------------------------------------------------------------------
// SMCLK source, up count mode, PWM Right Side
//------------------------------------------------------------------------------
TB1CTL = TBSSEL__SMCLK; // SMCLK
TB1CTL |= MC_1; // Up Mode
TB1CTL |= TBCLR; // Clear TAR

right_forward_rate = OFF; // Set Right Forward Off
right_reverse_rate = OFF; // Set Right Reverse Off

TB1CCR0 = WHEEL_PERIOD; // PWM Period
TB1CCTL1 = OUTMOD_7; // CCR1 reset/set
TB1CCR1 = right_forward_rate; // P3.4 Right Forward PWM duty cycle
TB1CCTL2 = OUTMOD_7; // CCR2 reset/set
TB1CCR2 = right_reverse_rate; // P3.5 Right Reverse PWM duty cycle
//------------------------------------------------------------------------------
}

void Init_Timer_B2(void) {
//------------------------------------------------------------------------------
// SMCLK source, up count mode, PWM Left Side
//------------------------------------------------------------------------------
TB2CTL = TBSSEL__SMCLK; // SMCLK
TB2CTL |= MC_1; // Up Mode
TB2CTL |= TBCLR; // Clear TAR

left_forward_rate = OFF; // Set Left Forward Off
left_reverse_rate = OFF; // Set Left Reverse Off

TB2CCR0 = WHEEL_PERIOD; // PWM Period
TB2CCTL1 = OUTMOD_7; // CCR1 reset/set
TB2CCR1 = left_forward_rate; // P3.4 Right Forward PWM duty cycle
TB2CCTL2 = OUTMOD_7; // CCR2 reset/set
TB2CCR2 = left_reverse_rate; // P3.5 Right Reverse PWM duty cycle
//------------------------------------------------------------------------------
}


