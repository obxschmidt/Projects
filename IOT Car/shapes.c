//------------------------------------------------------------------------------
//
//  Description: This file contains the functions for the car creating shapes using
//               movement functions from wheels.c
//
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------

#include  "msp430.h"
#include  "functions.h"
#include  "macros.h"

void Triangle(void) {
//------------------------------------------------------------------------------
//
// Description: Causes the car to make a triangle shape twice
//
// Passed: No variables passed
// Locals: No local variables
// Returned: No values returned
//  
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------
  five_msec_sleep(TWOSECOND); // delay for user to remove hands from car
  // first triangle
  Short_Segment_Forward();
  Quarter_Turn_Left();
  Short_Segment_Forward();
  Short_Segment_Forward();
  Quarter_Turn_Left();
  Short_Segment_Forward();
  Short_Segment_Forward();
  Quarter_Turn_Left();
  Short_Segment_Forward();
  // second triangle
  Short_Segment_Forward();
  Quarter_Turn_Left();
  Short_Segment_Forward();
  Short_Segment_Forward();
  Quarter_Turn_Left();
  Short_Segment_Forward();
  Short_Segment_Forward();
  Quarter_Turn_Left();
  Short_Segment_Forward();
}

void Circle(void) {
//------------------------------------------------------------------------------
//
// Description: Causes the car to make two circles
//
// CIRCLESTART  (1)
// ENDOFCIRCLE  (30)
//
// Passed: No variables passed
// Locals: No local variables
// Returned: No values returned
//  
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------
  five_msec_sleep(TWOSECOND); // delay for user to remove hands from car
  // first circle
  Left_Wheel_Forward();
  Right_Wheel_Forward();
  // loop for varying left wheel speed
  int i;
  for (i=CIRCLESTART; i<ENDOFCIRCLE; i++) {
  five_msec_sleep(TENMSEC); // wheel is on for 10 msec
  Left_Wheel_Off();
  five_msec_sleep(FIFTYMSEC); // wheel is off for 50 msec
  Left_Wheel_Forward();
  }
  // turn off both wheels after loop
  Left_Wheel_Off();
  Right_Wheel_Off();
  
  // second circle
  Left_Wheel_Forward();
  Right_Wheel_Forward();
  // loop for varying left wheel speed
  for (i=CIRCLESTART; i<ENDOFCIRCLE; i++) {
  five_msec_sleep(TENMSEC); // wheel is on for 10 msec
  Left_Wheel_Off();
  five_msec_sleep(FIFTYMSEC); // wheel is off for 50 msec
  Left_Wheel_Forward();
  }
  // turn off both wheels after loop
  Left_Wheel_Off();
  Right_Wheel_Off();
  
}

void Figure_Eight(void) {
//------------------------------------------------------------------------------
//
// Description: Causes the car to make two figure eights
//
// CIRCLESTART  (1)
// ENDOFCIRCLE  (30)
// SECONDCIRCLE (38)
// THIRDCIRCLE  (32)
//
// Passed: No variables passed
// Locals: No local variables
// Returned: No values returned
//  
//
//  Calvin Schmidt
//  Feb 2016
//  Built with IAR Embedded Workbench Version: V7.3.1.3987 (6.40.1)
//------------------------------------------------------------------------------
  five_msec_sleep(TWOSECOND); // delay for user to remove hands from car
  // first figure8 first circle
  Left_Wheel_Forward();
  Right_Wheel_Forward();
  // loop for varying left wheel speed
  int i;
  for (i=CIRCLESTART; i<ENDOFCIRCLE; i++) {
  five_msec_sleep(TENMSEC);
  Left_Wheel_Off();
  five_msec_sleep(FIFTYMSEC);
  Left_Wheel_Forward();
  }
  // turn off both wheels after loop
  Left_Wheel_Off();
  Right_Wheel_Off();
  
  // first figure8 second circle
  Left_Wheel_Forward();
  Right_Wheel_Forward();
  // loop for varying left wheel speed
  for (i=CIRCLESTART; i<SECONDCIRCLE; i++) {
  five_msec_sleep(TENMSEC);
  Right_Wheel_Off();
  five_msec_sleep(FIFTYMSEC);
  Right_Wheel_Forward();
  }
  // turn off both wheels after loop
  Left_Wheel_Off();
  Right_Wheel_Off();
  // end of first figure 8
  
  // second figure8 first circle
  Left_Wheel_Forward();
  Right_Wheel_Forward();
  // loop for varying left wheel speed
  for (i=CIRCLESTART; i<THIRDCIRCLE; i++) {
  five_msec_sleep(TENMSEC);
  Left_Wheel_Off();
  five_msec_sleep(FIFTYMSEC);
  Left_Wheel_Forward();
  }
  // turn off both wheels after loop
  Left_Wheel_Off();
  Right_Wheel_Off();
  
  // second figure8 second circle
  Left_Wheel_Forward();
  Right_Wheel_Forward();
  // loop for varying left wheel speed
  for (i=CIRCLESTART; i<ENDOFCIRCLE; i++) {
  five_msec_sleep(TENMSEC);
  Right_Wheel_Off();
  five_msec_sleep(FIFTYMSEC);
  Right_Wheel_Forward();
  }
  // turn off both wheels after loop
  Left_Wheel_Off();
  Right_Wheel_Off();
  // end of second figure8
}



