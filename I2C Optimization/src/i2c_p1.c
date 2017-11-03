#include	 <MKL25Z4.H>
#include	 "i2c.h"
#include 	"gpio_defs.h"
#include  "mma8451.h"

int fsm_state = TRANSMIT;
int error = 0;
uint8_t dummy_fsm, num_bytes_read_fsm=0, is_last_read_fsm=0;
// ISR
int volatile isr_state = REGISTER;
int volatile isr_finished = 0;
int volatile txrx;
uint8_t dummy_isr, num_bytes_read_isr=0, is_last_read_isr=0;
uint8_t data_isr[6];

//init i2c0
void i2c_init( void ) {
 //clock i2c peripheral and port E
	SIM->SCGC4		 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5		 |= SIM_SCGC5_PORTE_MASK;

	//set pins to I2C function
	PORTE->PCR[ 24 ] |= PORT_PCR_MUX( 5 );
	PORTE->PCR[ 25 ] |= PORT_PCR_MUX( 5 );

	//set baud rate
	//baud = bus freq/(scl_div+mul)
	I2C0->F				= ( I2C_F_ICR( 0x11 ) | I2C_F_MULT( 0 ) );

	//enable i2c and set to master mode
	I2C0->C1		 |= ( I2C_C1_IICEN_MASK );

	// Select high drive mode
	I2C0->C2		 |= ( I2C_C2_HDRS_MASK );
}

void i2c_wait( void )
{
	SET_BIT(DEBUG3_POS);
	while( ( I2C0->S & I2C_S_IICIF_MASK ) == 0 ) {
		;
	}
	I2C0->S |= I2C_S_IICIF_MASK;
	CLEAR_BIT(DEBUG3_POS);
}

int i2c_check_completion(void) {
	if (I2C0->S & I2C_S_IICIF_MASK) { 		// if completion
		I2C0->S |= I2C_S_IICIF_MASK;
		return 1;
	}
	else
		return 0;
}

int i2c_read_bytes_fsm(uint8_t dev_adx, uint8_t reg_adx, uint8_t * data, uint8_t data_count) {
	SET_BIT(DEBUG1_POS);
	switch(fsm_state) {
		case TRANSMIT:
		  I2C_TRAN;													//	set to transmit mode		
			SET_BIT(DEBUG2_POS);
			I2C_M_START;											//	send start										
			I2C0->D = dev_adx;								//	send dev address (write)
			fsm_state = WAIT_TRANSMIT;				//  advance to next state 
		break;
		case WAIT_TRANSMIT:
			if (i2c_check_completion()) { 		// if completion
				fsm_state = REGISTER;
			}
		break;
		case REGISTER:
			I2C0->D = reg_adx;								//	send register address
			fsm_state = WAIT_REGISTER;
		break;
		case WAIT_REGISTER:
			if (i2c_check_completion()) { 		// if completion
				fsm_state = DEV;
			}
		break;
		case DEV:
			I2C_M_RSTART;											//	repeated start									
			I2C0->D = dev_adx | 0x01 ;				//	send dev address (read)
			fsm_state = WAIT_DEV;
		break;
		case WAIT_DEV:
			if (i2c_check_completion()) { 		// if completion
				fsm_state = RECEIVE_MODE;
			}
		break;
		case RECEIVE_MODE:
			I2C_REC;													//	set to receive mode		
			fsm_state = CHECK_BYTES_READ;
		break;
		case CHECK_BYTES_READ:
			if (num_bytes_read_fsm < data_count) {  //  check if all data has been read
				fsm_state = RECEIVE;						//  continue reading
			}
			else {
				fsm_state = TRANSMIT;						//  restart fsm
				return 0;
			}
		break;
		case RECEIVE:
			is_last_read_fsm = (num_bytes_read_fsm == data_count-1)? 1: 0;
			if (is_last_read_fsm){
				NACK;													// tell HW to send NACK after read							
			} else {
				ACK;													// tell HW to send ACK after read								
			}
			dummy_fsm = I2C0->D;						//	dummy read
			fsm_state = WAIT_RECEIVE;
		break;
		case WAIT_RECEIVE:
			if (i2c_check_completion()) { 		// if completion
				fsm_state = SEND_STOP;
			}
		break;
		case SEND_STOP:
			if (is_last_read_fsm){
				I2C_M_STOP;											//	send stop		
				CLEAR_BIT(DEBUG2_POS);
			}
			data[num_bytes_read_fsm++] = I2C0->D; //	read data
			fsm_state = CHECK_BYTES_READ;	
		break;
		default:
			error = 1; 												// an error has occured
		break;
	}
	CLEAR_BIT(DEBUG1_POS);
	return 1;
}

void I2C0_IRQHandler(void) {
	int data_count_isr = 6;
	SET_BIT(DEBUG1_POS);
	
	switch(isr_state) {
		case (REGISTER):
			I2C0->D = REG_XHI;								//	send register address
			isr_state = DEV;
		break;
		case (DEV):
			I2C_M_RSTART;											//	repeated start									
			I2C0->D = MMA_ADDR | 0x01 ;				//	send dev address (read)
			isr_state = RECEIVE_MODE;
		break;
		case (RECEIVE_MODE):
			I2C_REC;													//	set to receive mode
			isr_state = RECEIVE;							//  set isr state
			dummy_isr = I2C0->D;
		break;
		case (RECEIVE):
			if (num_bytes_read_isr < data_count_isr) {  //  check if all data has been read
				is_last_read_isr = (num_bytes_read_isr == data_count_isr-1)? 1: 0;
				if (is_last_read_isr){
					NACK;													// tell HW to send NACK after read							
				} else {
					ACK;													// tell HW to send ACK after read								
				}						
				dummy_isr = I2C0->D;
				isr_state = SEND_STOP;
			}
		break;
		case (SEND_STOP):
			if (is_last_read_isr){
				I2C_M_STOP;										//	send stop		
				CLEAR_BIT(DEBUG2_POS);
			}
			data_isr[num_bytes_read_isr++] = I2C0->D; //	read data		
			isr_state = RECEIVE;
			if (!(num_bytes_read_isr < data_count_isr)) {
				isr_state = REGISTER;						//  restart fsm
				isr_finished = 1;
				// clear globals
				dummy_isr = 0;
				num_bytes_read_isr = 0;
				is_last_read_isr = 0;
			}
		break;	
		default:
			error = 1;
		break;
	}
	CLEAR_BIT(DEBUG1_POS);
	I2C0->S |= I2C_S_IICIF_MASK;
}

int i2c_read_bytes(uint8_t dev_adx, uint8_t reg_adx, uint8_t * data, uint8_t data_count) {
	uint8_t dummy, num_bytes_read=0, is_last_read=0;
	
	I2C_TRAN;													//	set to transmit mode		
	SET_BIT(DEBUG2_POS);
	I2C_M_START;											//	send start		
	
	I2C0->D = dev_adx;								//	send dev address (write)	
	i2c_wait();												//	wait for completion	
  	
	I2C0->D = reg_adx;								//	send register address		
	i2c_wait();												//	wait for completion	
  	
	I2C_M_RSTART;											//	repeated start									
	I2C0->D = dev_adx | 0x01 ;				//	send dev address (read)		
	i2c_wait();												//	wait for completion
  	
	I2C_REC;													//	set to receive mode								
	while (num_bytes_read < data_count) {
		is_last_read = (num_bytes_read == data_count-1)? 1: 0;
		if (is_last_read){
			NACK;													// tell HW to send NACK after read							
		} else {
			ACK;													// tell HW to send ACK after read								
		}

		dummy = I2C0->D;								//	dummy read
		i2c_wait();											//	wait for completion	
    		
		if (is_last_read){
			I2C_M_STOP;										//	send stop		
			CLEAR_BIT(DEBUG2_POS);			
		}
		data[num_bytes_read++] = I2C0->D; //	read data										
	}
	return 1;
}

int i2c_write_bytes(uint8_t dev_adx, uint8_t reg_adx, uint8_t * data, uint8_t data_count) {
	uint8_t num_bytes_written=0;
	
	I2C_TRAN;													//	set to transmit mode							
	I2C_M_START;											//	send start										
	I2C0->D = dev_adx;								//	send dev address (write)							
	i2c_wait();												//	wait for completion								

	I2C0->D = reg_adx;								//	send register address								
	i2c_wait();												//	wait for completion								

	while (num_bytes_written < data_count) {
		I2C0->D = data[num_bytes_written++]; //	write data										
		i2c_wait();											//	wait for completion								
	}
	I2C_M_STOP;												//		send stop										
	
	return 1;
}

