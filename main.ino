#include "defines.h"
#include "settings.h"
#include "work.h"
#include "Zone.h"
#include "set_help_functions.h"
#include "reload.h"

Motor motor1(motor1_pin1,motor1_pin3,motor1_pin2,motor1_pin4);
Motor motor2(motor2_pin1,motor2_pin3,motor2_pin2,motor2_pin4);
Motor motor3(motor3_pin1,motor3_pin3,motor3_pin2,motor3_pin4);


Zone zone1(zone1PauseOfDaysMemory, zone1DurationHoursMemory, zone1DurationMinutesMemory, zone1StartHoursMemory, zone1StartMinutesMemory, zone1IsTurnedOnMemory,motor1);
Zone zone2(zone2PauseOfDaysMemory, zone2DurationHoursMemory, zone2DurationMinutesMemory, zone2StartHoursMemory, zone2StartMinutesMemory, zone2IsTurnedOnMemory,motor2);
Zone zone3(zone3PauseOfDaysMemory, zone3DurationHoursMemory, zone3DurationMinutesMemory, zone3StartHoursMemory, zone3StartMinutesMemory, zone3IsTurnedOnMemory,motor3);

unsigned long last_time_of_check = 0;
unsigned long last_time_of_check2 = 0;
unsigned long pause_battery_check = 4*60*60*1000;

boolean temp1 = 1;
void setup(){
     //  lcd.begin ();
      // lcd.clear ();
        SETUP = !digitalRead(SETUP_BUTTON); 
  rtc.halt(false);
  rtc.writeProtect(false);
  Serial.begin(9600);
  setButton.setTickMode(AUTO);
  setButton.setDebounce(100);
  setButton.setTimeout(1500);

  backButton.setTickMode(AUTO);
  backButton.setDebounce(100);
  backButton.setTimeout(1500);
  
  pinMode(SETUP_BUTTON,INPUT_PULLUP);
  pinMode (woter_pin_1,INPUT_PULLUP);
  pinMode (woter_pin_2,INPUT_PULLUP);
  pinMode (rele_pin,OUTPUT); 
  pinMode (battery_rele,OUTPUT); 
  pinMode (reset_rele,OUTPUT); 
  pinMode (rele_transistor_pin,OUTPUT); 
  pinMode (transistor_pin,OUTPUT);
  digitalWrite(rele_pin,!Open_Rele_signal);
  power.setSleepMode(POWERDOWN_SLEEP);

    digitalWrite(rele_transistor_pin,HIGH);
    delay(100); 
    digitalWrite(battery_rele,HIGH);
    delay(100);
    Serial.println(analogRead(battery_signal));
    delay(50);
    Serial.println(analogRead(battery_signal));
    delay(50);
    Serial.println(analogRead(battery_signal));
    digitalWrite(rele_transistor_pin,LOW);  

    // rtc.setDOW(MONDAY);        // Set Day-of-Week to FRIDAY
    // rtc.setTime(14, 14, 20);     // Set the time to 12:00:00 (24hr format)
    // rtc.setDate(26, 7, 2021);   // Set the date to August 6th, 2010
  t = rtc.getTime();
  if(EEPROM.read(reload_marker_cell) == 1){reload_recovery();}
  if(EEPROM.read(sleep_marker_cell) == 1){recovery_after_sleep();}
}
void loop(){
  timeTempCheck();  //t = rtc.getTime();
  
    if(millis() - last_time_of_check >= 1000*60*40){
        last_time_of_check = millis();                 // проверка принудительной перезагрузки 
        if(is_time_to_reload())reload();
    }
    main_battery_check();
    
    if(SetupButtonCheck()== 1) {
        if(isInFillingTankProcess) {
            delay(100); 
            lcd.begin();
            lcd.clear(); 
            lcd.print("  please, wait  ");     
            stopFillingTank();
        }
        settings();
    }
    workCircle();
}
