//------------------------------------------------------------------------------
//
//  Description: This file contains the functions for adc
//
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

// display globals
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
//

extern int ADC_Thumb;
extern char adc_char[5];
extern int Current_Time;
extern int ADC_Channel;
extern int ADC_Left_Detector;
extern int ADC_Right_Detector;

extern char msec_display[6];

void Init_ADC(void){
//------------------------------------------------------------------------------
// Description: Configures ADC10_B
//
//  Calvin Schmidt
//  March 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1) 
//------------------------------------------------------------------------------
 ADC10CTL0 = RESET_STATE;       // Clear ADC10CTL0
 ADC10CTL0 |= ADC10SHT_2;       // 16 ADC clocks
 ADC10CTL0 &= ~ADC10MSC;        // Single Sequence
 ADC10CTL0 |= ADC10ON;          // ADC ON - Core Enabled
 
 ADC10CTL1 = RESET_STATE;       // Clear ADC10CTL1
 ADC10CTL1 |= ADC10SHS_0;       // ADC10SC bit
 ADC10CTL1 |= ADC10SHP;         // SAMPCON signal sourced from sampling timer
 ADC10CTL1 &= ~ADC10ISSH;       // The sample-input signal is not inverted.
 ADC10CTL1 |= ADC10DIV_0;       // ADC10_B clock divider – Divide by 1.
 ADC10CTL1 |= ADC10SSEL_0;      // MODCLK
 ADC10CTL1 |= ADC10CONSEQ_0;    // Single-channel, single-conversion
 
 ADC10CTL2 = RESET_STATE;       // Clear ADC10CTL2
 ADC10CTL2 |= ADC10DIV_0;       // Pre-divide by 1
 ADC10CTL2 |= ADC10RES;         // 10-bit resolution
 ADC10CTL2 &= ~ADC10DF;         // Binary unsigned
 ADC10CTL2 &= ~ADC10SR;         // supports up to approximately 200 ksps
 
 ADC10MCTL0 = RESET_STATE;      // Clear ADC10MCTL0
 ADC10MCTL0 |= ADC10SREF_0;     // V(R+) = AVCC and V(R-) = AVSS
 ADC10MCTL0 |= ADC10INCH_3;     // Channel A3 Thumb Wheel
 ADC10IE |= ADC10IE0;           // Enable ADC conversion complete interrupt
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ADC10 interrupt service routine
// ADC_Right_Detector; // A00 ADC10INCH_0 - P1.0
// ADC_Left_Detector; // A01 ADC10INCH_1 - P1.1
// ADC_Thumb; // A03 ADC10INCH_3 - P1.3
// ADC_Temp; // A10 ADC10INCH_10 – Temperature REF module
// ADC_Bat; // A11 ADC10INCH_11 - Internal
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void){
 switch(__even_in_range(ADC10IV,12)) {
 case 0: break; // No interrupt
 case 2: break; // conversion result overflow
 case 4: break; // conversion time overflow
 case 6: break; // ADC10HI
 case 8: break; // ADC10LO
 case 10: break; // ADC10IN
 case 12:
   ADC10CTL0 &= ~ADC10ENC; // Turn off the ENC bit of the ADC10CTL0
   switch (ADC_Channel){ // variable of the current active channel and increment each time through case statement
   case 0x00: // channel A0
   ADC10MCTL0 = ADC10INCH_1; // Next channel A1
   ADC_Right_Detector = ADC10MEM0; // Current Channel result for A0
   ADC_Channel++;
   break;
   case 0x01: // channel A1
   ADC10MCTL0 = ADC10INCH_3; // Next channel A3
   ADC_Left_Detector = ADC10MEM0; // Current Channel result for A1
   ADC_Channel++;
   break;
   case 0x02: // channel A3
   ADC10MCTL0 = ADC10INCH_0; // Next channel A0 (case 0x00)
   ADC_Thumb = ADC10MEM0; // Current Channel result for A3
   ADC_Channel = 0;
   break;
   default:
   break;
  }
  ADC10CTL0 |= ADC10ENC; // Turn on the ENC bit of the ADC10CTL0
  ADC10CTL0 |= ADC10SC; // Start next sample.
  break;
 default: break;
 }
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void ADC_Process(void){
 while (ADC10CTL1 & BUSY); // Wait if ADC10 core is active
 ADC10CTL0 |= ADC10ENC + ADC10SC; // Sampling and conversion start
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void dec_to_char(int value) {
  int count = 0;
  int current_array = 0;
  while (value >= 10000) {
    value = value - 10000;
    count ++;
  }
  msec_display[current_array] = 0x30 + count;
  current_array++;
  count = 0;
  while (value >= 1000) {
    value = value - 1000;
    count++;
  }
  msec_display[current_array] = 0x30 + count;
  current_array++;
  count = 0;
  while (value >= 100) {
    value = value - 100;
    count++;
  }
  msec_display[current_array] = 0x30 + count;
  current_array++;
  count = 0;
  while (value >= 10) {
    value = value - 10;
    count++;
  }
  msec_display[current_array] = 0x30 + count;
  current_array++;
  if ((value <= 9) && (value >= 0)) {
    msec_display[current_array] = 0x30 + value;
  }      
}
  
void offload_num(char data[]){
    
    for(int i = 0; i<6; i++){
    
    data[i] = msec_display[i];
}
  
}

/*
//ADC10INCH_0 is for Channel A0 Which signal is connected to the A0 pin?
//ADC10INCH_1 is for Channel A1 Which signal is connected to the A1 pin?
//ADC10INCH_3 is for Channel A3 Which signal is connected to the A3 pin?
case 12:
// Need this to change the ADC10INCH_x value.
 ADC10CTL0 &= ~ADC10ENC; // Turn off the ENC bit of the ADC10CTL0
 switch (variable of the current active channel and increment each time through case statement){
 case 0x00:
 ADC10MCTL0 = ADC10INCH_1; // Next channel A1
 (Channel A0 variable) = ADC10MEM0; // Current Channel result for A0
 break;
 case 0x01:
 ADC10MCTL0 = ADC10INCH_3; // Next channel A3
 (Channel A1 variable) = ADC10MEM0; // Current Channel result for A1
 break;
 case 0x02:
 ADC10MCTL0 = ADC10INCH_0; // Next channel A0
 (Channel A3 variable) = ADC10MEM0; // Current Channel result for A3
 Channel count value needs to be set back to 0;
 break;
 default:
 break;
}
ADC10CTL0 |= _______; // Turn on the ENC bit of the ADC10CTL0
ADC10CTL0 |= _______; // Start next sample.
break;
*/