//Реализация методов класса Zone:
Zone::Zone(byte PauseOfDaysMemory, byte DurationHoursMemory, byte DurationMinutesMemory, byte StartHoursMemory, byte StartMinutesMemory, byte IsTurnedOnMemory, Motor motor_c){
  pauseOfDays = EEPROM.read(PauseOfDaysMemory);
  duration = EEPROM.read(DurationHoursMemory)*60 + EEPROM.read(DurationMinutesMemory);
  startTime = EEPROM.read(StartHoursMemory)*60 + EEPROM.read(StartMinutesMemory);
  isTurnedOn = EEPROM.read(IsTurnedOnMemory);
  endTime = startTime+duration;
  if(endTime>=MINUTES_IN_DAY) endTime-=MINUTES_IN_DAY;
  motor = motor_c;
}
bool Zone::isDayChanged(){
 // t = rtc.getTime();//в начале work.ino и так вызывается
 timeTempCheck();
  if(currentWeekDay==t.dow) return false;
  currentWeekDay = t.dow;
  return true;
}
bool Zone::isStartWateringTime(int currentTime){
  if(!isTurnedOn){return false;}//если зона выключена, возвращаем значения, которые не влияют на работу программы
  if(isDayChanged() && daysToWatering != 0) { daysToWatering--;}
  if(isInWateringProcess){return false;}
  if((timeToWateringStart(currentTime)<TIME_BACKLASH)&&(daysToWatering==0)){ return true;}
  return false;
}
bool Zone::isEndWateringTime(int currentTime){
  if(abs(currentTime-endTime)<TIME_BACKLASH) return true;
  return false;
}
int Zone::timeToWateringStart(int currentTime){
  if(!isTurnedOn) return 1430;//если зона выключена, возвращаем значения, которые не влияют на работу программы
  int timeDifference = startTime - currentTime;
  if(timeDifference<0) timeDifference+=MINUTES_IN_DAY;
  return timeDifference;
}
void Zone::startWatering(){
  openTap(motor);
  isInWateringProcess = true;
}
void Zone::stopWatering(){
  closeTap(motor);
  isInWateringProcess = false;
  daysToWatering = pauseOfDays;
}
