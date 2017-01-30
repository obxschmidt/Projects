//------------------------------------------------------------------------------
//
//  Description: This file contains the function which makes the buttons write 
//               different strings to the display
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

extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;
extern char posL1;
extern char posL2;
extern char posL3;
extern char posL4;
extern volatile unsigned int counter;

void Switches_Process(void){
//------------------------------------------------------------------------------
//
//  Description: This function outputs alternate display messages when a switch is pressed
//               which corresponds to the shape that is currently running. Switch 1 cycles 
//               through the different shapes, and switch 2 selects that shape.
//  Macros:
//  CIRCLE      (1)
//  TRIANGLE    (2)
//  FIGUREEIGHT (3)
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------
  
  if (!(P4IN & SW1)){
  }
  if (!(P4IN & SW2)) {
  }
//------------------------------------------------------------------------------
}