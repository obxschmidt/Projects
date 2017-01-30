//turn left wheel on
void Left_Forward_On (void) {
	if (!(P3IN & L_FORWARD)) { // If wheel is not on
	  Left_Reverse_Off();
	  P3OUT |= L_FORWARD;
	}
}
void Left_Wheel_Off (void) {
  P3OUT &= ~L_FORWARD;
  P3OUT &= ~L_REVERSE;
}

//triangle 
//make 6 short segments and a turn. 
//Start in middle of short segment. -T--T--T-

//circle 
//at least 50ms
//five_msec_delay(10)

//add delays to shape functions so that the car doesn't start immediately

//switch.c 
//shapes.c // include indiviudal movement section functions, then add those into circle/triangle/figure8. 
//wheels.c