#include "set_help_functions.h"
void settings(){
    delay(100); // для устранения дребезга контактов..
    lcd.begin();
    lcd.clear();  
    while(true){
    knotSet(0);
    Serial.println("in circle");
    if(SetupButtonCheck() == 0)break;
    }
   Serial.println("intermediate_settings");
    intermediate_settings();
}

void intermediate_settings(){
    zone_intermediate_settings(zone1,1);
    zone_intermediate_settings(zone2,2);
    zone_intermediate_settings(zone3,3);
}
void zone_intermediate_settings(Zone &zone,byte zone_number){
    boolean is_changed = 0;
    if(zone.pauseOfDays != EEPROM.read(6 + (zone_number-1)*6))is_changed = 1;
    if(zone.duration !=  ( EEPROM.read(7 + (zone_number-1)*6))*60 + EEPROM.read(8 + (zone_number-1)*6)) is_changed = 1;
    if(zone.startTime !=  ( EEPROM.read(9 + (zone_number-1)*6))*60 + EEPROM.read(10 + (zone_number-1)*6))  is_changed = 1;
    if(zone.isTurnedOn != EEPROM.read(11 + (zone_number-1)*6))is_changed = 1;
    

    if(is_changed == 1){             
        if(zone.isInWateringProcess){ zone.stopWatering();}
        zone.pauseOfDays = EEPROM.read(6 + (zone_number-1)*6);
        zone.duration = ( EEPROM.read(7 + (zone_number-1)*6)*60 + EEPROM.read(8 + (zone_number-1)*6));
        zone.startTime = ( EEPROM.read(9 + (zone_number-1)*6)*60 + EEPROM.read(10 + (zone_number-1)*6));
        zone.endTime = zone.startTime+zone.duration;
        if(zone.endTime>=MINUTES_IN_DAY) zone.endTime-=MINUTES_IN_DAY;
        zone.isTurnedOn = EEPROM.read(11 + (zone_number-1)*6);
    }else{
        if(zone.isInWateringProcess && !(is_time_in_interval(zone.startTime, zone.endTime)) ) zone.stopWatering();
        if(!(zone.isInWateringProcess) && is_time_in_interval(zone.startTime, zone.endTime) && zone.daysToWatering ==0) zone.startWatering();
    }

}
boolean is_time_in_interval(int startTime, int stopTime){
    timeTempCheck();  // t = rtc.getTime();
    int currentTime = t.hour*60+t.min;
    if(stopTime>=startTime)return (startTime<=currentTime)&&(currentTime<=stopTime);
    if(stopTime<startTime){
        startTime-=stopTime;
        currentTime-=stopTime;
        if(currentTime<=0) currentTime+=MINUTES_IN_DAY;
        return (startTime<=currentTime)&&(currentTime<=0);
    }
}
