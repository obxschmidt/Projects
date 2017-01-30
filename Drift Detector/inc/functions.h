#include "queue.h"
// later rename this to GPS.h

typedef struct {
  float Speed_over_ground;
	float Track_angle_true;
	float Mag_variation;
	int new_rmc;
} RMCstruct;

typedef struct {
  float Speed_through_water;
  float Heading_angle_magnetic;
	int new_vhw;
} VHWstruct;

void Task_NMEA_Decode(void);
void Task_Report_Drift(void);
void Get_Buffer(Q_T * q, char *buffer);
void Task_Report_Drift(void);

// FSM Functions
void FSM_RMC(void);
void FSM_VHW(void);
int FSM_Talker_ID(void);
int FSM_Sentence_ID(void);
// RMC functions
void FSM_Fix_Time(void);
void FSM_Status(void);
void FSM_Latitude_Value(void);
void FSM_Latitude_Direction(void);
void FSM_Longitude_Value(void);
void FSM_Longitude_Direction(void);
void FSM_Ground_Speed(void);
void FSM_Track_Angle(void);
void FSM_Date(void);
void FSM_Magnetic_Variation(void);
void FSM_Variation_Direction(void);
// VHW functions
void FSM_True_HDG_Value(void);
void FSM_True_HDG_Type(void);
void FSM_Mag_HDG_Value(void);
void FSM_Mag_HDG_Type(void);
void FSM_Vessel_Speed_Knots(void);
void FSM_Vessel_Units_Knots(void);
void FSM_Vessel_Speed_Kilo(void);
void FSM_Vessel_Units_Kilo(void);
// Checksum functions
void FSM_Checksum_Indicator(void);
void FSM_Checksum_Calculation(void);
int Checksum(const char *s);
void Clear_Remaining_Buffer(void);

#define DONE 									(0xFF)
#define CLEAR									(0)
#define RMC										(1)
#define VHW										(2)
// RMC Macros
#define FIX_TIME							(0)		
#define STATUS								(1)	
#define LATITUDE_VALUE				(2)	
#define LATITUDE_DIRECTION		(3)	
#define LONGITUDE_VALUE				(4)	
#define LONGITUDE_DIRECTION		(5)	
#define GROUND_SPEED					(6)	
#define TRACK_ANGLE						(7)	
#define DATE									(8)	
#define MAGNETIC_VARIATION		(9)	
#define VARITION_DIRECTION		(10)		
// VHW Macros
#define TRUE_HDG_VALUE				(0)
#define TRUE_HDB_TYPE					(1)
#define MAG_HDG_VALUE					(2)
#define MAG_HDG_TYPE					(3)
#define VESSEL_SPEED_KNOTS		(4)
#define VESSEL_UNITS_KNOTS		(5)
#define VESSEL_SPEED_KILO			(6)
#define VESSEL_UNITS_KILO			(7)
// Checksum Macros
#define CHECKSUM_INDICATOR		(11)	
#define CHECKSUM_CALCULATION	(12)


