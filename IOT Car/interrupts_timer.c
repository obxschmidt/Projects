//------------------------------------------------------------------------------
//
//  Description: This file contains the interrupts for timers
//
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

//Display Globals
extern char display_line_1[LENGTHOFDISPLAY];
extern char display_line_2[LENGTHOFDISPLAY];
extern char display_line_3[LENGTHOFDISPLAY];
extern char display_line_4[LENGTHOFDISPLAY];
extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;
extern char posL1;
extern char posL2;
extern char posL3;
extern char posL4;
// 5msec delay global
extern int msec_count;
// ADC globals
extern int ADC_Thumb;
extern int ADC_Left_Detector;
extern int ADC_Right_Detector;
extern char adc_char[5];
extern int ADC_Flag;
extern char msec_display[MSEC_LENGTH]; // For dec_to_char (hextobcd)
// Temporary test globals
extern int adc_display_flag;



//------------------------------------------------------------------------------
// TimerA0 0 Interrupt handler
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void){
//------------------------------------------------------------------------------
// Description: Interrupt that will occur every 200ms, as set by the clock speed and TA0CCR0 interval macro.
//              Performs function calls based on current Timer_Counter phase to perform movement for Project 4
//              
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1) 
//------------------------------------------------------------------------------
 TA0CCR0 += TA0CCR0_INTERVAL; // Add Offset to TACCR0
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// TimerA0 1-2, Overflow Interrupt Vector (TAIV) handler
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void){
  switch(__even_in_range(TA0IV,14)){
  case 0: break; // No interrupt
  case 2: // CCR1 not used
    msec_count++;
    TA0CCR1 += TA0CCR1_INTERVAL; // Add Offset to TACCR1
  break;
  case 4: // CCR2 not used
    TA0CCR2 += TA0CCR2_INTERVAL; // Add Offset to TACCR2
  break;
  case 14: // overflow
  break;
  default: break;
}
}
//----------------------------------------------------------------------------










