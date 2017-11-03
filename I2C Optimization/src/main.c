/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "gpio_defs.h"
#include "LEDs.h"
#include "i2c.h"
#include "mma8451.h"
#include "delay.h"

#define FLASH_DELAY 10
#define ACC_SENSITIVITY 40

int volatile mode;

void Init_Debug_Signals(void) {
	// Debug signal 1
	PORTB->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[1] |= PORT_PCR_MUX(1);
	// Debug signal 2
	PORTB->PCR[2] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[2] |= PORT_PCR_MUX(1);
	// Debug signal 3
	PORTB->PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[3] |= PORT_PCR_MUX(1);
	// Set ports to outputs
	PTB->PDDR |= MASK(1);
	PTB->PDDR |= MASK(2);
	PTB->PDDR |= MASK(3);
}

void Init_Config_Signals(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; // Set clock to port E
	// Pin 3
	PORTE->PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[3] |= PORT_PCR_PS_MASK;
	PORTE->PCR[3] |= PORT_PCR_PE_MASK;
	PORTE->PCR[3] |= PORT_PCR_MUX(1);
	// Pin 4
	PORTE->PCR[4] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[4] |= PORT_PCR_PS_MASK;
	PORTE->PCR[4] |= PORT_PCR_PE_MASK;
	PORTE->PCR[4] |= PORT_PCR_MUX(1);
	// Pin 5
	PORTE->PCR[5] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[5] |= PORT_PCR_PS_MASK;
	PORTE->PCR[5] |= PORT_PCR_PE_MASK;
	PORTE->PCR[5] |= PORT_PCR_MUX(1);
	// Set ports to input
	PTE->PDDR &= ~MASK(3);
	PTE->PDDR &= ~MASK(4);
	PTE->PDDR &= ~MASK(5);
}

void Config(void) {
	if (PTE->PDIR == BLOCKING) {
		mode = 1;
	}
  else if (PTE->PDIR == FSM) {
		mode = 2;
	}
	else if (PTE->PDIR == ISR) {
		mode = 3;
	}
}

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	int16_t prev_acc_X, prev_acc_Y, prev_acc_Z;
	int n;
	
	Init_RGB_LEDs();
	Init_Config_Signals();
	Init_Debug_Signals();

	Control_RGB_LEDs(1, 1, 0);								/* yellow: starting up */
	i2c_init();																/* init i2c	*/
	Delay(200);
	
	if (!init_mma()) {												/* init mma peripheral */
		Control_RGB_LEDs(1, 0, 0);							/* Light red error LED */
		while (1)																/* not able to initialize mma */
			;
	}
	Control_RGB_LEDs(0, 0, 0);							
	
	Delay(50);
	Config(); // Selects mode based on Port E pins
	
	while (1) {
		#if 0
		// mode = 1 hardcodes to use Blocking
		// mode = 2 hardcodes to use FSM
		// mode = 3 hardcodes to use ISR
		mode = 2; 
		#endif
		
		Delay(50);
		prev_acc_X = acc_X;
		prev_acc_Y = acc_Y;
		prev_acc_Z = acc_Z;
		
		if (mode == 1) {
			read_full_xyz(); // blocking
		}
		else if (mode == 2) {
			read_full_xyz_fsm();
		}
		else if (mode == 3) {
			// Init i2c interrupt
			NVIC_SetPriority(I2C0_IRQn, 2);
			NVIC_ClearPendingIRQ(I2C0_IRQn);
			NVIC_EnableIRQ(I2C0_IRQn);
			//I2C0->C1 |= I2C_C1_IICIE_MASK; 		// enable interrrupt
			//I2C0->C1 &= ~I2C_C1_IICIE_MASK;   // disables interrrupt
			__enable_irq();
			
			read_full_xyz_isr();
		}
		
		if ((abs(prev_acc_X - acc_X) > ACC_SENSITIVITY) || 
			(abs(prev_acc_Y - acc_Y) > ACC_SENSITIVITY) || 
			(abs(prev_acc_Z - acc_Z) > ACC_SENSITIVITY)) {
			// Flash LEDs
				#if 1
				for (n=0; n<2; n++) {
					Control_RGB_LEDs(1, 1, 1);
					Delay(FLASH_DELAY);
					Control_RGB_LEDs(0, 0, 0);							
					Delay(FLASH_DELAY*2);		
				}
				#endif
			}
		I2C0->C1 |= I2C_C1_IICIE_MASK; // enable interrrupt
	}
}

