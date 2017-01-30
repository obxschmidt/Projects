//------------------------------------------------------------------------------
//
//  Description: This file contains the functions required for initializations
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

extern char display_line_1[LENGTHOFDISPLAY];
extern char display_line_2[LENGTHOFDISPLAY];
extern char display_line_3[LENGTHOFDISPLAY];
extern char display_line_4[LENGTHOFDISPLAY];
extern char *display_1;
extern char *display_2;
extern char *display_3;
extern char *display_4;

void Init_Conditions(void){
//------------------------------------------------------------------------------
// Description: This function contains the Initializations Configurations
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------
// Interrupts are disabled by default, enable them. 
  enable_interrupts();
  display_1 = &display_line_1[LSB];
  display_2 = &display_line_2[LSB];
  display_3 = &display_line_3[LSB];
  display_4 = &display_line_4[LSB];

//------------------------------------------------------------------------------
}



