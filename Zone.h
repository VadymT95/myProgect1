class Zone{
  public:
    bool isInWateringProcess = false;//поливается ли эта зона сейчас
    byte pauseOfDays;
    byte currentWeekDay = 8;
    byte daysToWatering = 1;
      //in minutes:
    int duration;
    int startTime;
    int endTime;
    bool isTurnedOn;
    Motor motor;
    //
    Zone(byte, byte, byte, byte, byte, byte, Motor);
    bool isDayChanged();
    bool isStartWateringTime(int currentTime);
    bool isEndWateringTime(int currentTime);
    int timeToWateringStart(int currentTime);
    void startWatering();
    void stopWatering();
};
