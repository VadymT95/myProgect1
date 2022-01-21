

boolean is_time_to_reload(){
   timeTempCheck(); // t = rtc.getTime();
    if(is_day_changed(t.dow)){  days_to_reload--;  }
    if(days_to_reload == 0 && t.hour == hour_of_reload){
        return true; 
    }
    if(days_to_reload > 4){days_to_reload = 0;}    
    return false;
}
void reload(){
    EEPROM.update(reload_marker_cell,1);
    EEPROM.update(tank_reload_cell,isInFillingTankProcess);
    EEPROM.update(zone1_reload_cell,zone1.isInWateringProcess*100+zone1.daysToWatering);
    EEPROM.update(zone2_reload_cell,zone2.isInWateringProcess*100+zone2.daysToWatering);
    EEPROM.update(zone3_reload_cell,zone3.isInWateringProcess*100+zone3.daysToWatering);
    digitalWrite(rele_transistor_pin,HIGH);
    delay(400);  
     digitalWrite(reset_rele,HIGH);
   
    // digitalWrite(,HIGH);

}
boolean is_day_changed(byte clock_dow){
    if(current_day == 8) current_day = clock_dow;
    if(current_day != clock_dow) {
        current_day = clock_dow;
        return true;
    }
    return false;
}
void reload_recovery(){
    timeTempCheck(); //t = rtc.getTime();
    EEPROM.update(reload_marker_cell,0); 
    isInFillingTankProcess = EEPROM.read(tank_reload_cell);
    zone1.isInWateringProcess = (floor(EEPROM.read(zone1_reload_cell)/100));
    zone2.isInWateringProcess = (floor(EEPROM.read(zone2_reload_cell)/100));
    zone3.isInWateringProcess = (floor(EEPROM.read(zone3_reload_cell)/100));

    zone1.daysToWatering = (EEPROM.read(zone1_reload_cell)%100);
    zone2.daysToWatering = (EEPROM.read(zone2_reload_cell)%100);
    zone3.daysToWatering = (EEPROM.read(zone3_reload_cell)%100);

    zone1.currentWeekDay = t.dow;
    zone2.currentWeekDay = t.dow;
    zone3.currentWeekDay = t.dow;

}
void deep_sleep(){
    timeTempCheck(); // t = rtc.getTime();
    if(zone1.isInWateringProcess) zone1.stopWatering();
    if(zone2.isInWateringProcess) zone2.stopWatering();
    if(zone3.isInWateringProcess) zone3.stopWatering();
    if(isInFillingTankProcess) stopFillingTank();

    EEPROM.update(sleep_marker_cell,1);
    EEPROM.update(sleep_day_cell,t.date);

    for(int i = 0; i >= 0; i++){
    if(i == sleep_battery_check*60*6) {
        if(voltage_check() >= min_battery_voltage + 50) {recovery_after_sleep(); break;}
        i = 0;
    }
    delay(100); power.sleepDelay(10000); delay(100);
    }
}
void recovery_after_sleep(){
    byte sleep_duration = 0;
    byte day_of_start_sleep = (EEPROM.read(sleep_day_cell));
    t = rtc.getTime();
    if(t.date < day_of_start_sleep){
        sleep_duration = (31 - day_of_start_sleep + t.date);
    }else{
        sleep_duration = t.date - day_of_start_sleep;
    }

    if(zone1.daysToWatering <= sleep_duration){
        zone1.daysToWatering = 0;
        zone1.currentWeekDay = t.dow;
    }else{
        zone1.daysToWatering = zone1.daysToWatering - sleep_duration;
    }
    if(zone2.daysToWatering <= sleep_duration){
        zone2.daysToWatering = 0;
        zone2.currentWeekDay = t.dow;
    }else{
        zone2.daysToWatering = zone2.daysToWatering - sleep_duration;
    }
    if(zone3.daysToWatering <= sleep_duration){
        zone3.daysToWatering = 0;
        zone3.currentWeekDay = t.dow;
    }else{
        zone3.daysToWatering = zone3.daysToWatering - sleep_duration;
    }
     EEPROM.update(sleep_marker_cell,0);
 
}
int main_battery_check(){
    if(millis() - last_time_of_check2 >= pause_battery_check){
        last_time_of_check2 = millis();                 // проверка напряжения батареи и возможный принудительный сон.
        if(voltage_check() >= 741) {
            pause_battery_check = 4*60*60*1000;
        }else{
            pause_battery_check = 2*60*60*1000;
        }
        if(voltage_check() <= min_battery_voltage) {deep_sleep();}
    }
}      
int voltage_check(){
    digitalWrite(rele_transistor_pin,HIGH);
    delay(100); 
    digitalWrite(battery_rele,HIGH);
    delay(100);
    int voltage_sum = (analogRead(battery_signal) + analogRead(battery_signal)) /2;
    for(byte i = 1; i <=9; i++){
        if(analogRead(battery_signal) - 50 > (voltage_sum/i) || 
           analogRead(battery_signal) + 50 < (voltage_sum/i)  ){
            voltage_sum += analogRead(battery_signal);
            delay(100);
        }
    }
    digitalWrite(battery_rele,LOW);
    delay(100);
    digitalWrite(rele_transistor_pin,LOW);
    delay(100); 
    return (voltage_sum / 10);
}
