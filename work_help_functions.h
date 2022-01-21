boolean SetupButtonCheck(byte);
int getCurrentTime();

void moveTap(Motor, long);
void openTap(Motor);
void closeTap(Motor);

boolean isTankFull();
boolean isTankEmpty();
bool isFillingTankTime(int);
void startFillingTank();
void stopFillingTank();
void timeTempCheck();
void rtc_reboot();
