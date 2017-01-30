#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "UART.h"
#include "functions.h"

extern Q_T RxQ, TxQ;
extern int buffer_state;
extern int sentence_buffer_size;
// Structs
RMCstruct rmcS;
VHWstruct vhwS;
// Decoded NMEA global variables
char talker_ID[2];
char sentence_ID[4];
// RMC globals
char fix_time[6];
char status; // Can be either A=active or V=void
float latitude_value;
char latitude_direction; // Can be either N=north or S=south
float longitude_value;
char longitude_direction; // Can be either N=north or S=south
float ground_speed; // Given in knots
float track_angle; 
char date[6];
float magnetic_variation;
char magnetic_direction; // Can be E=east(positive) or W=west(negative)
// VHW globals
float true_hdg_value;
char true_hdg_type; // Can only be 'T'
float mag_hdg_value;
char mag_hdg_type; // Can only be 'M'
float vessel_speed_knots;
char vessel_units_knots; // Can only be 'N'
float vessel_speed_kilo;
char vessel_units_kilo; // Can only be 'K'
// Checksum globals
char checksum_indicator; // Can only be '*'
int checksum_value;
int checksum_flag;
// Buffer global
char local_buffer[BUFFER_SIZE];
int buffer_counter = 0;
int fsm_counter;

void Task_NMEA_Decode() {
	int fsm_flag_1;
	int fsm_flag_2; 
	
	fsm_counter = 0; // resets FSM on call of Task_NMEA_Decode()
	Get_Buffer(&RxQ, local_buffer); // puts received message from queue into local buffer
	buffer_state = EMPTY; // resets buffer state
	buffer_counter = 0;
	// Determines type of NMEA message
	fsm_flag_1 = FSM_Talker_ID();
	fsm_flag_2 = FSM_Sentence_ID();
	// if RMC message
	if (fsm_flag_1 == RMC && fsm_flag_2 == RMC) {
		fsm_flag_1 = CLEAR;
	  fsm_flag_2 = CLEAR;
		FSM_RMC();
	}
	// if VHW message
	else if (fsm_flag_1 == VHW && fsm_flag_2 == VHW) {
		fsm_flag_1 = CLEAR;
	  fsm_flag_2 = CLEAR;
		FSM_VHW();
	}
}

void FSM_RMC() {
	while (fsm_counter != DONE) {
	switch(fsm_counter) {
		case FIX_TIME:
			FSM_Fix_Time(); 
			break;
		case STATUS:
			FSM_Status(); 
			break;
		case LATITUDE_VALUE:
			FSM_Latitude_Value(); 
			break;
		case LATITUDE_DIRECTION:
			FSM_Latitude_Direction();
			break;
		case LONGITUDE_VALUE:
			FSM_Longitude_Value();
			break;
		case LONGITUDE_DIRECTION:
			FSM_Longitude_Direction();
			break;
		case GROUND_SPEED:
			FSM_Ground_Speed();
			break;
		case TRACK_ANGLE:
			FSM_Track_Angle();
			break;
		case DATE:
			FSM_Date();
			break;
		case MAGNETIC_VARIATION:
			FSM_Magnetic_Variation();
			break;
		case VARITION_DIRECTION:
			FSM_Variation_Direction();
			break;
		case CHECKSUM_INDICATOR:
			FSM_Checksum_Indicator();
		case CHECKSUM_CALCULATION:
			FSM_Checksum_Calculation();
		default:
			fsm_counter = DONE;
			break;
	  }
	}
}

void FSM_VHW() {
	while (fsm_counter != DONE) {
	switch(fsm_counter) {
		case TRUE_HDG_VALUE:
			FSM_True_HDG_Value();
			break;
		case TRUE_HDB_TYPE:
			FSM_True_HDG_Type();
			break;
		case MAG_HDG_VALUE:
			FSM_Mag_HDG_Value();
			break;
		case MAG_HDG_TYPE:
			FSM_Mag_HDG_Type();
			break;
		case VESSEL_SPEED_KNOTS:
			FSM_Vessel_Speed_Knots();
			break;
		case VESSEL_UNITS_KNOTS:
			FSM_Vessel_Units_Knots();
			break;
		case VESSEL_SPEED_KILO:
			FSM_Vessel_Speed_Kilo();
			break;
		case VESSEL_UNITS_KILO:
			FSM_Vessel_Units_Kilo();
			break;
		case CHECKSUM_INDICATOR:
			FSM_Checksum_Indicator();
			break;
		case CHECKSUM_CALCULATION:
			FSM_Checksum_Calculation();
			break;
		default:
			fsm_counter = DONE;
			break;
		}
	}
}

int FSM_Talker_ID() {
	talker_ID[0] = local_buffer[buffer_counter++];
	talker_ID[1] = local_buffer[buffer_counter++];
	// Error Handling
	if (talker_ID[0] == 'G' && talker_ID[1] == 'P')
		return RMC;
	else if (talker_ID[0] == 'V' && talker_ID[1] == 'W')
		return VHW;
	else {
		printf("Error. Invalid Talker ID.\n");
		return CLEAR;
	}
}

int FSM_Sentence_ID() {
	sentence_ID[0] = local_buffer[buffer_counter++];
	sentence_ID[1] = local_buffer[buffer_counter++];
	sentence_ID[2] = local_buffer[buffer_counter++];
	sentence_ID[3] = local_buffer[buffer_counter++];
	// Error Handling
	if (sentence_ID[0] == 'R' && sentence_ID[1] == 'M' && sentence_ID[2] == 'C')
		return RMC;
	else if (sentence_ID[0] == 'V' && sentence_ID[1] == 'H' && sentence_ID[2] == 'W')
		return VHW;
	else {
		printf("Error. Invalid Sentence ID.\n");
		return CLEAR;
	}
}

/* RMC Functions */

void FSM_Fix_Time() {
	int i = 0;
	while (local_buffer[buffer_counter] != ',') {
		fix_time[i++] = local_buffer[buffer_counter++];
	}
	buffer_counter++; // accounts for comma
	fsm_counter++;
}

void FSM_Status() {
	if (local_buffer[buffer_counter] != ',') {
		status = local_buffer[buffer_counter++];
		buffer_counter++; // Increment 1 additional time to account for comma
		fsm_counter++;
	}
	else {
		buffer_counter++;
		fsm_counter++;
	}
}

void FSM_Latitude_Value() {
	int i = 0;
	char temp_buffer[16];
	while (local_buffer[buffer_counter] != ',') {
		if (Is_Number(local_buffer[buffer_counter]))
		  temp_buffer[i++] = local_buffer[buffer_counter++];
		else {
			printf("Error. Non numerical Latitude Value\n");
			fsm_counter = DONE;
			buffer_counter++;
		}
	}
	latitude_value = atof(temp_buffer);
	buffer_counter++; // accounts for comma
	fsm_counter++;
}

void FSM_Latitude_Direction() {
	if (local_buffer[buffer_counter] != ',') {
		latitude_direction = local_buffer[buffer_counter++];
		buffer_counter++; // Increment 1 additional time to account for comma
		fsm_counter++;
	}
	else {
		buffer_counter++;
		fsm_counter++;
	}
}

void FSM_Longitude_Value() {
	int i = 0;
	char temp_buffer[16];
	while (local_buffer[buffer_counter] != ',') {
		if (Is_Number(local_buffer[buffer_counter]))
		  temp_buffer[i++] = local_buffer[buffer_counter++];
		else {
			printf("Error. Non numerical Longitude Value\n");
			fsm_counter = DONE;
			buffer_counter++;
		}
	}
	longitude_value = atof(temp_buffer);
	buffer_counter++; // accounts for comma
	fsm_counter++;
}

void FSM_Longitude_Direction() {
	if (local_buffer[buffer_counter] != ',') {
		longitude_direction = local_buffer[buffer_counter++];
		buffer_counter++; // Increment 1 additional time to account for comma
		fsm_counter++;
	}
	else {
		buffer_counter++;
		fsm_counter++;
	}
}

void FSM_Ground_Speed() {
	int i = 0;
	char temp_buffer[16];
	while (local_buffer[buffer_counter] != ',') {
		if (Is_Number(local_buffer[buffer_counter]))
		  temp_buffer[i++] = local_buffer[buffer_counter++];
		else {
			printf("Error. Non numerical Ground Speed Value\n");
			fsm_counter = DONE;
			buffer_counter++;
		}
	}
	ground_speed = atof(temp_buffer);
	rmcS.Speed_over_ground = ground_speed; // load value into struct
	rmcS.new_rmc = 1;
	buffer_counter++; // accounts for comma
	fsm_counter++;
}

void FSM_Track_Angle() {
	int i = 0;
	char temp_buffer[16];
	while (local_buffer[buffer_counter] != ',') {
		if (Is_Number(local_buffer[buffer_counter]))
		  temp_buffer[i++] = local_buffer[buffer_counter++];
		else {
			printf("Error. Non numerical Track Angle Value\n");
			fsm_counter = DONE;
			buffer_counter++;
		}
	}
	track_angle = atof(temp_buffer);
	rmcS.Track_angle_true = track_angle; // load value into struct
	rmcS.new_rmc = 1;
	buffer_counter++; // accounts for comma
	fsm_counter++;
}

void FSM_Date() {
	int i = 0;
	while (local_buffer[buffer_counter] != ',') {
		if (Is_Number(local_buffer[buffer_counter]))
		  date[i++] = local_buffer[buffer_counter++];
		else {
			printf("Error. Non numerical Date Value\n");
			fsm_counter = DONE;
			buffer_counter++;
		}
	}
	buffer_counter++; // accounts for comma
	fsm_counter++;
}

void FSM_Magnetic_Variation() {
	int i = 0;
	char temp_buffer[16];
	while (local_buffer[buffer_counter] != ',') {
		if (Is_Number(local_buffer[buffer_counter]))
		  temp_buffer[i++] = local_buffer[buffer_counter++];
		else {
			printf("Error. Non numerical Magnetic Variation Value\n");
			fsm_counter = DONE;
			buffer_counter++;
		}
	}
	magnetic_variation = atof(temp_buffer);
	rmcS.Mag_variation = magnetic_variation; // load value into struct
	rmcS.new_rmc = 1;
	buffer_counter++; // accounts for comma
	fsm_counter++;
}

void FSM_Variation_Direction() {
	if (local_buffer[buffer_counter] != ',') {
		magnetic_direction = local_buffer[buffer_counter++];
		fsm_counter++;
	}
	else
		fsm_counter++;
}

/* VHW Functions */

void FSM_True_HDG_Value() {
	int i = 0;
	char temp_buffer[16];
	while (local_buffer[buffer_counter] != ',') {
		if (Is_Number(local_buffer[buffer_counter]))
		  temp_buffer[i++] = local_buffer[buffer_counter++];
		else {
			printf("Error. Non numerical True Heading Value\n");
			fsm_counter = DONE;
			buffer_counter++;
		}
	}
	true_hdg_value = atof(temp_buffer);
	buffer_counter++; // accounts for comma
	fsm_counter++;
}

void FSM_True_HDG_Type() {
	if (local_buffer[buffer_counter] != ',') {
		true_hdg_type = local_buffer[buffer_counter++];
		buffer_counter++; // Increment 1 additional time to account for comma
		fsm_counter++;
	}
	else {
		buffer_counter++;
		fsm_counter++;
	}
}

void FSM_Mag_HDG_Value() {
	int i = 0;
	char temp_buffer[16];
	while (local_buffer[buffer_counter] != ',') {
		if (Is_Number(local_buffer[buffer_counter]))
		  temp_buffer[i++] = local_buffer[buffer_counter++];
		else {
			printf("Error. Non numerical Magnetic heading Value\n");
			fsm_counter = DONE;
			buffer_counter++;
		}
	}
	mag_hdg_value = atof(temp_buffer);
	vhwS.Heading_angle_magnetic = mag_hdg_value; // load data into struct
	vhwS.new_vhw = 1;
	buffer_counter++; // accounts for comma
	fsm_counter++;
}

void FSM_Mag_HDG_Type() {
	if (local_buffer[buffer_counter] != ',') {
		mag_hdg_type = local_buffer[buffer_counter++];
		buffer_counter++; // Increment 1 additional time to account for comma
		fsm_counter++;
	}
	else {
		buffer_counter++;
		fsm_counter++;
	}
}

void FSM_Vessel_Speed_Knots() {
	int i = 0;
	char temp_buffer[16];
	while (local_buffer[buffer_counter] != ',') {
		if (Is_Number(local_buffer[buffer_counter]))
		  temp_buffer[i++] = local_buffer[buffer_counter++];
		else {
			printf("Error. Non numerical Vessel Speed Value\n");
			fsm_counter = DONE;
			buffer_counter++;
		}
	}
	vessel_speed_knots = atof(temp_buffer);
	vhwS.Speed_through_water = vessel_speed_knots; // load data into struct
	vhwS.new_vhw = 1;
	buffer_counter++; // accounts for comma
	fsm_counter++;
}

void FSM_Vessel_Units_Knots() {
	if (local_buffer[buffer_counter] != ',') {
		vessel_units_knots = local_buffer[buffer_counter++];
		buffer_counter++; // Increment 1 additional time to account for comma
		fsm_counter++;
	}
	else {
		buffer_counter++;
		fsm_counter++;
	}
}

void FSM_Vessel_Speed_Kilo() {
	int i = 0;
	char temp_buffer[16];
	while (local_buffer[buffer_counter] != ',') {
		if (Is_Number(local_buffer[buffer_counter]))
		  temp_buffer[i++] = local_buffer[buffer_counter++];
		else {
			printf("Error. Non numerical Vessel Speed Value\n");
			fsm_counter = DONE;
			buffer_counter++;
		}
	}
	vessel_speed_kilo = atof(temp_buffer);
	buffer_counter++; // accounts for comma
	fsm_counter++;
}

void FSM_Vessel_Units_Kilo() {
	if (local_buffer[buffer_counter] != ',') {
		vessel_units_kilo = local_buffer[buffer_counter++];
		fsm_counter = CHECKSUM_INDICATOR;
	}
	else {
		fsm_counter = CHECKSUM_INDICATOR;
	}
}

/* Checksum Functions */

void FSM_Checksum_Indicator() {
	checksum_indicator = local_buffer[buffer_counter];
	local_buffer[buffer_counter++] = 0;
	fsm_counter++;
}

void FSM_Checksum_Calculation() {
	// Get checksum from local_buffer
	char temp_buffer[2];
	temp_buffer[0] = local_buffer[buffer_counter];
	local_buffer[buffer_counter++] = 0;
	temp_buffer[1] = local_buffer[buffer_counter];
	local_buffer[buffer_counter++] = 0;
	// Convert local_buffer to hex
	checksum_value = strtol(temp_buffer, NULL, 16);
	// Clear the rest of local_buffer
	Clear_Remaining_Buffer();
	if (Checksum(local_buffer) == checksum_value) {
		fsm_counter = DONE;
		checksum_flag = 1;
	}
	else {
		printf("Error. Invalid Checksum. Message discarded.\n");
		fsm_counter = DONE;
		checksum_flag = 0;
	}
}

void Clear_Remaining_Buffer() {
	int i;
	for (i = buffer_counter; i < BUFFER_SIZE; i++) {
		local_buffer[i] = 0;
	}
}

void Task_Report_Drift() {
	float drift;
	float difference;
	if (checksum_flag) {
		if (rmcS.new_rmc) {
			rmcS.new_rmc = 0; // reset new
			printf("RMC received. Speed over ground is %f kts with a track of %f°T. "
			"Magnetic variation is %f°.\n", ground_speed, track_angle, magnetic_variation);
			// Calculate drift values
			drift = mag_hdg_value - track_angle;
			difference = vessel_speed_knots - ground_speed;
			// Print drift message
			if ((mag_hdg_value > 0 && track_angle > 0) || (vessel_speed_knots > 0 && ground_speed > 0)) {
				printf(" Difference between heading and track is %f°, "
				"difference in speed through water and speed over ground "
				"is %f kts.\n", drift, difference);
			}
		}
		if (vhwS.new_vhw) {
			vhwS.new_vhw = 0;
			printf("VHW received. Speed through water is %f kts with a heading "
			"of %f°M.\n", vessel_speed_knots, mag_hdg_value);
			// Calculate drift values
			drift = mag_hdg_value - track_angle;
			difference = vessel_speed_knots - ground_speed;
			// Print drift message
			if ((mag_hdg_value > 0 && track_angle > 0) || (vessel_speed_knots > 0 && ground_speed > 0)) {
				printf(" Difference between heading and track is %f°, "
				"difference in speed through water and speed over ground "
				"is %f kts.\n", drift, difference);
			} 
		}
	}
}

void Get_Buffer(Q_T * q, char *buffer) {
	int buffer_index = 0;
	int i;
	for(i = q->Head; i < q->Tail; i++) {
		buffer[buffer_index++] = Q_Dequeue(q); // adds char to array
	}
	q->Head = 0;
	q->Tail = 0;
	//sentence_buffer_size = 0; // Used for testing
}

int Checksum(const char *s) {
	int c = 0;
	while(*s)
			c ^= *s++;

	return c;
}


