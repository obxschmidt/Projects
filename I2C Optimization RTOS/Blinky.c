#include <cmsis_os.h>
#include <MKL25Z4.H>
#include <stdlib.h>
#include "gpio_defs.h"
#include "LEDs.h"
#include "delay.h"
#include "mma8451.h"
#include "i2c.h"

#define NUM_Q_ENTRIES 8
#define FLASH_DELAY 10
#define ACC_SENSITIVITY 40

int16_t prev_acc_X, prev_acc_Y, prev_acc_Z;

typedef struct {
	uint8_t read_write; // 0 = read, 1 = write
	uint8_t dev_adx; 
	uint8_t reg_adx; 
	uint8_t * data;
	uint8_t data_count;
	struct os_semaphore_cb * sem;
} I2C_Queue;

// These are declared as globals because the code breaks otherwise
// Also was unviewable in watch window 
I2C_Queue    *rptr;
osEvent I2C_event;

// Thread IDs for the threads
osThreadId t_F;  
osThreadId t_I2C;  

void Thread_Flash(void const * arg);
void Thread_I2C(void const * arg);

osThreadDef(Thread_Flash, osPriorityNormal, 1, 0);
osThreadDef(Thread_I2C, osPriorityNormal, 1, 0);

osPoolId mpool;	// define memory pool
osPoolDef(mpool, 16, I2C_Queue);

osMessageQId RGB_msgq;	// define message queue
osMessageQDef(RGB_msgq, NUM_Q_ENTRIES, &I2C_Queue);

osSemaphoreId RGB_sem;
osSemaphoreDef(RGB_sem);

volatile static int32_t t=0;
uint32_t g_RGB_delay=700; 	// delay for RGB sequence

void Create_OS_Objects(void) {
	t_I2C= osThreadCreate(osThread(Thread_I2C), NULL);
	t_F = osThreadCreate(osThread(Thread_Flash), NULL);
}

void Thread_Flash(void const * arg) {
	int n;
	
	I2C_Queue    I2C;
	uint8_t data[6];
	RGB_msgq = osMessageCreate(osMessageQ(RGB_msgq), NULL);
	RGB_sem = osSemaphoreCreate(osSemaphore(RGB_sem), 0);
	
	I2C.read_write = 1;
  I2C.dev_adx = MMA_ADDR;
  I2C.reg_adx = REG_XHI;
	I2C.data = data;
	I2C.data_count = 6;
	I2C.sem = RGB_sem;

	while (1) {
		osMessagePut(RGB_msgq, (uint32_t)&I2C, osWaitForever); // Send message queue
		t = osSemaphoreWait(RGB_sem, osWaitForever); // t equals number of available tokens
		
		osDelay(10);  // wait 10 msec
		if ((abs(prev_acc_X - acc_X) > ACC_SENSITIVITY) || 
			(abs(prev_acc_Y - acc_Y) > ACC_SENSITIVITY) || 
			(abs(prev_acc_Z - acc_Z) > ACC_SENSITIVITY)) {
			// Flash LEDs
			for (n=0; n<2; n++) {
				Control_RGB_LEDs(1, 1, 1);
				Delay(FLASH_DELAY);
				Control_RGB_LEDs(0, 0, 0);							
				Delay(FLASH_DELAY*2);		
			}
		}
		osSemaphoreRelease(RGB_sem);
	}
}

void Thread_I2C(void const * arg) {
	int test1 = 0;
	int test2 = 0;
	//volatile I2C_Queue    *rptr;
	//volatile osEvent I2C_event;
	
	int16_t temp[3];
	int i;
	
	while (1) {
		t = osSemaphoreWait(RGB_sem, osWaitForever);
		I2C_event = osMessageGet(RGB_msgq, osWaitForever);
		test1++;
		if (I2C_event.status == osEventMessage) {
			rptr = I2C_event.value.p;
			test2++;
			
			prev_acc_X = acc_X;
			prev_acc_Y = acc_Y;
			prev_acc_Z = acc_Z;
			
			if (i2c_mode == 1) {
				i2c_read_bytes(rptr->dev_adx, rptr->reg_adx, rptr->data, rptr->data_count);
			}
			else if (i2c_mode == 2) {
				i2c_read_bytes_isr_bytes(rptr->dev_adx, rptr->reg_adx, rptr->data, rptr->data_count);
			}
			else if (i2c_mode == 3) {
				// uint8_t volatile data_isr[6], dev_adx_isr, reg_adx_isr, data_count_isr;
				isr_state = 0;
				dummy_isr = 0;
				num_bytes_read_isr = 0;
				is_last_read_isr = 0;
				
				dev_adx_isr = rptr->dev_adx;
				reg_adx_isr = rptr->reg_adx;
				data_count_isr = rptr->data_count;
				
				i2c_read_bytes_isr_i2c(rptr->dev_adx, rptr->reg_adx, rptr->data, rptr->data_count);
				osSignalWait(I2C_SIGNAL, osWaitForever);
			}
			
			if (i2c_mode == 1 || i2c_mode == 2) {
				for ( i=0; i<3; i++ ) {
					temp[i] = (int16_t) ((rptr->data[2*i]<<8) | rptr->data[2*i+1]);
				}
			}
			else if (i2c_mode == 3) {
				for ( i=0; i<3; i++ ) {
					temp[i] = (int16_t) ((data_isr[2*i]<<8) | data_isr[2*i+1]);
				}
			}
			// Align for 14 bits
			acc_X = temp[0]/4;
			acc_Y = temp[1]/4;
			acc_Z = temp[2]/4;
		}
		osSemaphoreRelease(RGB_sem);
	}
}

/*
void Thread_Read_Switches(void const * arg) {
	int count=0;
	while (1) {
		osDelay(250);
		if (SWITCH_PRESSED(SW1_POS)) {
			osSignalSet(t_F_Busy_Wait, 1);
		}	
		
		if (SWITCH_PRESSED(SW2_POS)) { 
				count++;
				Control_RGB_LEDs(0, 1, 0);
				osDelay(g_RGB_delay/30);
				Control_RGB_LEDs(0, 0, 0);
		} else { // send message on release
				if (count > 0) {
					osMessagePut(switch_msgq, count, osWaitForever);
					count = 0;
				}
		}
	}
}

void Thread_RGB(void const * arg) {
	osEvent result; 
	int i;
	
	while (1) {
		result = osMessageGet(switch_msgq, osWaitForever);
		if (result.status == osEventMessage) {
			for (i=0; i<result.value.v; i++) { // Do RGB v times
				Control_RGB_LEDs(1, 0, 0);
				osDelay(g_RGB_delay);
				Control_RGB_LEDs(0, 1, 0);
				osDelay(g_RGB_delay);
				Control_RGB_LEDs(0, 0, 1);
				osDelay(g_RGB_delay);
			}
			Control_RGB_LEDs(0, 0, 0);
		}
	}
}

void Thread_Flash_Busy_Wait(void const * arg) {
	osEvent result; 
	
	while (1) {
		result = osSignalWait(1, osWaitForever);
		if (result.status == osEventSignal) {
			Control_RGB_LEDs(1,1,0);
			osDelay(g_RGB_delay);
			Control_RGB_LEDs(0,0,1);
			osDelay(g_RGB_delay);
			Control_RGB_LEDs(0, 0, 0);
		}
	}
}
*/







