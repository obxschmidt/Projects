#include "LPTimer.h"
#include "MKL25Z4.h"
#include "LEDs.h"
#include "GPIO_defs.h"

volatile int32_t LPT_ticks=0;
void Tilt( void );

void Init_LPTMR(uint32_t freq) {
	SIM->SCGC5 |=  SIM_SCGC5_LPTMR_MASK;

	// Configure LPTMR
	// select 1 kHz LPO clock with prescale factor 0, dividing clock by 2
	// resulting in 500 Hz clock
	LPTMR0->PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PRESCALE(0); 
	LPTMR0->CSR = LPTMR_CSR_TIE_MASK;
	LPTMR0->CMR = 50; // Generate interrupt every 50 clock ticks or 100 ms
	
	// Configure NVIC 
	NVIC_SetPriority(LPTimer_IRQn, 3); 
	NVIC_ClearPendingIRQ(LPTimer_IRQn); 
	NVIC_EnableIRQ(LPTimer_IRQn);	
}

void Start_LPTMR(void) {
	LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
}

void Stop_LPTMR(void) {
	LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
}

void LPTimer_IRQHandler(void) {
	static uint8_t n=5; // LED period (5*100ms)
	LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;

	if (n==0) {
		Tilt();
		n = 5;
	} else {
		Control_RGB_LEDs(0,0,0);
	}
	n--;
}