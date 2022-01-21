#include "work_help_functions.h"
boolean SetupButtonCheck (){
  return !digitalRead(SETUP_BUTTON);
};
void moveTap(Motor motor, long steps){//надо ли аж long для steps?
  Stepper stepper(STEPS_PER_REV, motor.pin1, motor.pin2, motor.pin3, motor.pin4);
  digitalWrite(rele_transistor_pin,HIGH);
  delay(100);
  digitalWrite(rele_pin,Open_Rele_signal);
  stepper.setSpeed(500);
  stepper.step(steps);
  digitalWrite(motor.pin1,LOW);
  digitalWrite(motor.pin2,LOW);
  digitalWrite(motor.pin3,LOW);
  digitalWrite(motor.pin4,LOW);
  digitalWrite(rele_pin, !Open_Rele_signal);
  delay(100);
  digitalWrite(rele_transistor_pin,LOW);
}
void openTap(Motor motor){
  moveTap(motor, MOTOR_STEPS_TO_OPEN);
}
void closeTap(Motor motor){
  moveTap(motor, MOTOR_STEPS_TO_CLOSE);
}
//Tank:
Motor tankMotor(motor0_pin1, motor0_pin3, motor0_pin2, motor0_pin4);
void startFillingTank(){
  openTap(tankMotor);
  isInFillingTankProcess = true;
}
void stopFillingTank(){
  closeTap(tankMotor);
  isInFillingTankProcess = false;
}
boolean isTankFull(){
  boolean check1,check2,check3 = 0;
  while(true){
    delay(80);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check1 = digitalRead(woter_pin_1);//в нано сигнал был инвертирован
    delay(20);   
    digitalWrite(transistor_pin, LOW);
    if(SetupButtonCheck() == 1) return check1;
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check1;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check2 = digitalRead(woter_pin_1); //в нано сигнал был инвертирован
    delay(20);   
    digitalWrite(transistor_pin, LOW);
    if(SetupButtonCheck() == 1) return check2;   
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check2;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check3 = digitalRead(woter_pin_1);//в нано сигнал был инвертирован
    delay(20);   
    digitalWrite(transistor_pin, LOW);   
    if(check1 == check2 && check2== check3) return check1;
  }
}
boolean isTankEmpty(){
  boolean check1,check2,check3 = 0;
  while(true){
    delay(100);
    digitalWrite(transistor_pin, HIGH);
    delay(500);  
    check1 = digitalRead(woter_pin_2);//в нано сигнал был инвертирован
    delay(20);
    digitalWrite(transistor_pin, LOW);   
    if(SetupButtonCheck() == 1) return check1; 
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check1;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check2 = digitalRead(woter_pin_2); //в нано сигнал был инвертирован
    delay(20);
    digitalWrite(transistor_pin, LOW);  
    if(SetupButtonCheck() == 1) return check2; 
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check2;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check3 = digitalRead(woter_pin_2);//в нано сигнал был инвертирован
    delay(20);
    digitalWrite(transistor_pin, LOW);    
    if(check1 == check2 && check2== check3) return !check1;
  }
}
bool isFillingTankTime(int currentTime){
//Возвращает true если был выбран режим afterWatering, или если текущее время совпадает с указаным в формате customTime
//Проверка режима:
  bool tankMode = EEPROM.read(tankModeMemory);
  if(tankMode==1) return true;//режим afterWatering
//проверка времени:
  int startTime = EEPROM.read(tankStartHoursMemory)*60 + EEPROM.read(tankStartMinutesMemory);
  int stopTime = EEPROM.read(tankStopHoursMemory)*60 + EEPROM.read(tankStopMinutesMemory);
  //Проверка на содержание текущего времени в промежутке:
  if(stopTime>=startTime)return (startTime<=currentTime)&&(currentTime<=stopTime);
  if(stopTime<startTime){
    startTime-=stopTime;
    currentTime-=stopTime;
    if(currentTime<=0) currentTime+=MINUTES_IN_DAY;
    return (startTime<=currentTime)&&(currentTime<=0);
  }
}
void timeTempCheck(){
      t = rtc.getTime();
      if(t.hour == 0 && t.min == 0 && t.year < 2021){
          int difference = millis() - last_millis_check;
          unsigned long new_millis_sec = ((current_hour*60 + current_min)*60 + current_sec + (difference/1000));
          if( new_millis_sec <= 86400){
              byte hour1 = new_millis_sec/3600;
              byte min1 = (new_millis_sec - hour1*3600)/60;
              byte second1 = (new_millis_sec - hour1*3600 - min1*60);            
              delay(100);
              rtc.setTime(hour1, min1, second1); 
              rtc.setDate(current_date, current_month, current_year);
              delay(100);
          }else{
              delay(100);
              rtc.setTime(23, 59, 58); 
              rtc.setDate(current_date, current_month, current_year);   
              delay(100);           
          } 
      }else{
          current_hour = t.hour;
          current_min = t.min;
          current_sec = t.sec;
          current_date = t.date;
          current_month = t.mon;
          current_year = t.year;
          last_millis_check = millis();
          current_dow = t.dow;    
          if(current_hour == 23 && current_min == 55){rtc_reboot();}
          if(current_hour == 18 && current_min == 55){rtc_reboot();}
          if(current_hour == 12 && current_min == 55){rtc_reboot();}
          if(current_hour == 6 && current_min == 55){rtc_reboot();}
      }
      delay(500);
    t = rtc.getTime();
}
void rtc_reboot(){ 
    byte hour1 = t.hour;
    byte min1 = t.min;
    byte second1 = t.sec;
    byte date1 = t.date;
    byte month1 = t.mon;
    byte year1 = t.year;
    delay(100);
    rtc.setTime(hour1, min1, second1); 
    rtc.setDate(date1, month1, year1);
    delay(100);
}
