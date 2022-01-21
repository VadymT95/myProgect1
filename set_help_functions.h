int read_potent(int);
void LCDprintResult(byte,byte,byte,byte);
void menuInitialize(byte, String[]);
void menuUpdate(byte&, String[], byte);
byte knotSet(byte);
//
#define rainSensorIsTurnedOnMemory 0

#define tankStartHoursMemory 1
#define tankStartMinutesMemory 2
#define tankStopHoursMemory 3
#define tankStopMinutesMemory 4
#define tankModeMemory 5

#define zone1PauseOfDaysMemory 6
#define zone1DurationHoursMemory 7
#define zone1DurationMinutesMemory 8
#define zone1StartHoursMemory 9
#define zone1StartMinutesMemory 10
#define zone1IsTurnedOnMemory 11

#define zone2PauseOfDaysMemory 12
#define zone2DurationHoursMemory 13
#define zone2DurationMinutesMemory 14
#define zone2StartHoursMemory 15
#define zone2StartMinutesMemory 16
#define zone2IsTurnedOnMemory 17

#define zone3PauseOfDaysMemory 18
#define zone3DurationHoursMemory 19
#define zone3DurationMinutesMemory 20
#define zone3StartHoursMemory 21
#define zone3StartMinutesMemory 22
#define zone3IsTurnedOnMemory 23
