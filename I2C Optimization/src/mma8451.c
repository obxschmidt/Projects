#include <MKL25Z4.H>
#include "mma8451.h"
#include "gpio_defs.h"
#include "i2c.h"
#include "delay.h"

int16_t acc_X=0, acc_Y=0, acc_Z=0;
extern uint8_t dummy_fsm, num_bytes_read_fsm, is_last_read_fsm;
extern uint8_t volatile dummy_isr, num_bytes_read_isr, is_last_read_isr;
extern uint8_t volatile data_isr[6];
extern int volatile isr_finished, isr_state;
extern int volatile txrx;

//initializes mma8451 sensor
//i2c has to already be enabled
int init_mma()
{
	uint8_t data[1];

	//set active mode, 14 bit samples, 2g full scale and 800 Hz ODR 
	data[0] = 0x01;
	i2c_write_bytes(MMA_ADDR, REG_CTRL1, data, 1);
	return 1;
}

void read_full_xyz()
{
	int i;
	uint8_t data[6];
	int16_t temp[3];
	
	SET_BIT(DEBUG1_POS);
	i2c_read_bytes(MMA_ADDR, REG_XHI, data, 6);
	CLEAR_BIT(DEBUG1_POS);
	
	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	// Align for 14 bits
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;
}

void read_full_xyz_fsm() {
	int i;
	uint8_t data[6];
	int16_t temp[3];
	
	while(i2c_read_bytes_fsm(MMA_ADDR, REG_XHI, data, 6)) {
		ShortDelay(5);
	}
	// Reset globals
	dummy_fsm = 0;
	num_bytes_read_fsm = 0;
	is_last_read_fsm = 0;

	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	// Align for 14 bits
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;
}

void read_full_xyz_isr() {
	int i;
	int16_t temp[3];
	
	I2C0->C1 |= I2C_C1_IICIE_MASK; 		// 	enable interrrupt
	SET_BIT(DEBUG2_POS);
	I2C_TRAN;													//	set to transmit mode							
	I2C_M_START;											//	send start										
	I2C0->D = MMA_ADDR;								//	send dev address (write)
	
	isr_state = REGISTER;
	while(!isr_finished) {
		;
	}
	isr_finished = 0;									//  reset isr finished flag
	I2C0->C1 &= ~I2C_C1_IICIE_MASK;   //  disables interrrupt
	
	
	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((data_isr[2*i]<<8) | data_isr[2*i+1]);
	}

	// Align for 14 bits
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;
}















