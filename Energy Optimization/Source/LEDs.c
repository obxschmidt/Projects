#include <MKL25Z4.H>
#include "LEDs.h"
#include "gpio_defs.h"

void Init_RGB_LEDs(void) {
	// Enable clock to ports B and D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	// Make 3 pins GPIO
	PORTB->PCR[RED_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);          
	PORTB->PCR[GREEN_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(1);          
	PORTD->PCR[BLUE_LED_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(1);          
	
	// Set ports to outputs
	PTB->PDDR |= MASK(RED_LED_POS) | MASK(GREEN_LED_POS);
	PTD->PDDR |= MASK(BLUE_LED_POS);
}

void Control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on) {
	if (red_on) {
			PTB->PCOR = MASK(RED_LED_POS);
	} else {
			PTB->PSOR = MASK(RED_LED_POS); 
	}
	if (green_on) {
			PTB->PCOR = MASK(GREEN_LED_POS);
	}	else {
			PTB->PSOR = MASK(GREEN_LED_POS); 
	} 
	if (blue_on) {
			PTD->PCOR = MASK(BLUE_LED_POS);
	}	else {
			PTD->PSOR = MASK(BLUE_LED_POS); 
	}
}	

void Toggle_RGB_LEDs(unsigned int red, unsigned int green, unsigned int blue){
	if (red) {
			PTB->PTOR = MASK(RED_LED_POS);
	}
	if (green) {
			PTB->PTOR = MASK(GREEN_LED_POS);
	}	
	if (blue) {
			PTD->PTOR = MASK(BLUE_LED_POS);
	}	
}		

void Init_PWM() {
	//turn on clock to TPM0, Port E, Port B, Port D
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
	
	//turn on clock to TPM2
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	
	// Set mux to connect TPM2 Ch 2 to PTB18 for Red LED
	PORTB->PCR[18] &= ~PORT_PCR_MUX(7);
	PORTB->PCR[18] |= PORT_PCR_MUX(3); //ALT3 is free
	
	// Set mux to connect TPM2 Ch 2 to PTB19 for Green LED
	PORTB->PCR[19] &= ~PORT_PCR_MUX(7);
	PORTB->PCR[19] |= PORT_PCR_MUX(3); //ALT3 is free
	
	// Set mux to connect TPM2 Ch 2 to PTB19 for Blue LED
	PORTD->PCR[1] &= ~PORT_PCR_MUX(7);
	PORTD->PCR[1] |= PORT_PCR_MUX(4); //ALT4 is free
	
	//set clock source for tpm
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK);

	//load the counter and mod
	TPM0->MOD = PWM_PERIOD-1;
	TPM2->MOD = PWM_PERIOD-1;
		
	//set TPM controls
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
	
	//set trigger mode
	TPM0->CONF |= TPM_CONF_TRGSEL(0xA);
	TPM2->CONF |= TPM_CONF_TRGSEL(0xA);
	
	//set PWM to edge-aligned
	TPM2->CONTROLS[0].CnV = 0; // Red LED
	TPM2->CONTROLS[1].CnV = 0; // Green LED
	TPM0->CONTROLS[1].CnV = 0; // Blue LED
	
	//set TPM to divide by 2 prescaler 
	TPM0->SC = (TPM_SC_CMOD(1) | TPM_SC_PS(5));
	TPM2->SC = (TPM_SC_CMOD(1) | TPM_SC_PS(5));
}

void Set_PWM_TPM0_Value(uint16_t channel, uint16_t duty) {
	if (duty > PWM_PERIOD)
		duty = PWM_PERIOD;
	
	TPM0->CONTROLS[channel].CnV = duty;
}

void Set_PWM_TPM2_Value(uint16_t channel, uint16_t duty) {
	if (duty > PWM_PERIOD)
		duty = PWM_PERIOD;
	
	TPM2->CONTROLS[channel].CnV = duty;
}

void Set_PWM_Red_LED(int red_pwm) { // Red LED uses TPM2 Channel 0
	Set_PWM_TPM2_Value(CHANNEL_ZERO, red_pwm);
}

void Set_PWM_Green_LED(int green_pwm) { // Green LED uses TPM2 Channel 1
	Set_PWM_TPM2_Value(CHANNEL_ONE, green_pwm);
}

void Set_PWM_Blue_LED(int blue_pwm) { // Blue LED uses TPM0 Channel 1
	Set_PWM_TPM0_Value(CHANNEL_ONE, blue_pwm);
}

void Reset_LEDs(void) { //Resets all LEDs to be off
	Set_PWM_Blue_LED(PWM_PERIOD);
  Set_PWM_Green_LED(PWM_PERIOD);
	Set_PWM_Red_LED(PWM_PERIOD);
}
