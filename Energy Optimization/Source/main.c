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
#include "LPTimer.h"
#include "Delay.h"

void Init_Accel(void) {
	Delay(50);
	if (!init_mma()) {												/* init mma peripheral */
		Control_RGB_LEDs(1, 0, 0);							/* light red error LED */
		while (1)																/* not able to initialize mma */
			;
	}
	Delay(50);
}

void Tilt( void ) {
	int16_t accel[3];
	float roll, pitch;
	
	read_full_xyz(accel);
	convert_xyz_to_roll_pitch(accel, &roll, &pitch);
	if ((fabs(roll) > 30) || (fabs(pitch) > 30))
		Control_RGB_LEDs(1, 0, 0);
	else if ((fabs(roll) > 15) || (fabs(pitch) > 15))
		Control_RGB_LEDs(1, 1, 0);
	else
		Control_RGB_LEDs(0, 1, 0);
	Delay(1);
}

int main (void) {
	Init_RGB_LEDs();
	i2c_init();											// init i2c
	Init_Accel();										// init accelerometer
	
	// Allow low leakage stop mode
	SMC->PMPROT = SMC_PMPROT_ALLS_MASK; // 
	// Enable low-leakage stop mode and regular run mode
	SMC->PMCTRL = SMC_PMCTRL_STOPM(3) | SMC_PMCTRL_RUNM(0);
	SMC->STOPCTRL = SMC_STOPCTRL_PSTOPO(0) | SMC_STOPCTRL_VLLSM(3);

	// Enable LLWU
	// allow LPTMR0 to wake LLWU
	LLWU->ME |= LLWU_ME_WUME0_MASK;

	// Enable stop mode (deep sleep)
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

	Init_LPTMR(500);
	Start_LPTMR();

	__enable_irq();

	// work is in interrupt
	while (1) {
	__wfi() ; // then go to sleep	
	}
}
