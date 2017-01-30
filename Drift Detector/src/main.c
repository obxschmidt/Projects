/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include "gpio_defs.h"
#include "UART.h"
#include "LEDs.h"
#include "timers.h"		
#include "delay.h"
#include "functions.h"

#define TRANSMIT_MODE 0
#define RECEIVE_MODE 0

//char sentence_buffer[BUFFER_SIZE]; // used for testing
extern int buffer_state;

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	Init_UART0(115200);
	printf("Calvin Schmidt - ECE 492\n");
	printf("cnschmid\n");
	while (1) {
		if (buffer_state == FULL) {
			Task_NMEA_Decode();
		  Task_Report_Drift();
		}
	}	
}

// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
