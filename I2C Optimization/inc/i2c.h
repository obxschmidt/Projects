#include <stdint.h>

#define I2C_M_START 	I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP  	I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART 	I2C0->C1 |= I2C_C1_RSTA_MASK

#define I2C_TRAN			I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC				I2C0->C1 &= ~I2C_C1_TX_MASK

#define BUSY_ACK 	    while(I2C0->S & 0x01)
#define TRANS_COMP		while(!(I2C0->S & 0x80))

#define NACK 	        I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK           I2C0->C1 &= ~I2C_C1_TXAK_MASK

// Config
#define BLOCKING (0x03000018u)
#define FSM 		 (0x03000028u)
#define ISR			 (0x03000030u)
//FSM Macros
#define TRANSMIT (1)
#define WAIT_TRANSMIT (2)
#define REGISTER (3)
#define WAIT_REGISTER (4)
#define DEV (5)
#define WAIT_DEV (6)
#define RECEIVE (7)
#define RECEIVE_MODE (12)
#define WAIT_RECEIVE (8)
#define SEND_STOP (9)
#define READ_DATA (10)
#define CHECK_BYTES_READ (11)


void i2c_init(void);
int i2c_read_bytes(uint8_t dev_adx, uint8_t reg_adx, uint8_t * data, uint8_t data_count);
int i2c_write_bytes(uint8_t dev_adx, uint8_t reg_adx, uint8_t * data, uint8_t data_count);
int i2c_read_bytes_fsm(uint8_t dev_adx, uint8_t reg_adx, uint8_t * data, uint8_t data_count);
void i2c_wait( void );
int i2c_check_completion(void);
void ShortDelay (uint32_t dly); 
