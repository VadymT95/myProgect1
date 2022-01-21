#include "work_help_functions.h"

byte tank_status = 0;

void workCircle (){
  
  timeTempCheck(); // t = rtc.getTime();
  //  Serial.print("time -> ");
   // Serial.print(t.hour);
  //  Serial.print(":");
  //  Serial.print(t.min);   
 //   Serial.print(":");
  //  Serial.println(t.sec);


//Получение текущего времени:
  int currentTime = t.hour*60 + t.min;//в минутах

    Serial.print("---current time--- ");
    Serial.print(t.hour);
    Serial.print(".");
    Serial.print(t.min);
    Serial.print(".");
    Serial.print(t.sec);
    Serial.println(";");
    Serial.println("---zone1---");
    Serial.print("timeToWateringStart > ");
    Serial.println(zone1.timeToWateringStart(currentTime));
    Serial.print("timeToWateringStart > ");
    Serial.println(zone1.timeToWateringStart(currentTime));
    Serial.print("isEndWateringTime > ");
    Serial.println(zone1.isEndWateringTime(currentTime));
    Serial.print("EndWateringTime > ");
    Serial.println(zone1.endTime);
    Serial.print("daysToWatering > ");
    Serial.println(zone1.daysToWatering);
    Serial.print("startTime > ");
    Serial.println(zone1.startTime);
    Serial.print("duration > ");
    Serial.println(zone1.duration);

    Serial.println("---zone2---");
    Serial.print("timeToWateringStart > ");
    Serial.println(zone2.timeToWateringStart(currentTime));
    Serial.print("isInWateringProcess > ");
    Serial.println(zone2.isInWateringProcess);
    Serial.print("isEndWateringTime > ");
    Serial.println(zone2.isEndWateringTime(currentTime));
    Serial.print("EndWateringTime > ");
    Serial.println(zone2.endTime);
    Serial.print("daysToWatering > ");
    Serial.println(zone2.daysToWatering);
    Serial.print("startTime > ");
    Serial.println(zone2.startTime);
    Serial.print("duration > ");
    Serial.println(zone2.duration);

    Serial.println("---zone3---");
    Serial.print("timeToWateringStart > ");
    Serial.println(zone3.timeToWateringStart(currentTime));
    Serial.print("isInWateringProcess > ");
    Serial.println(zone3.isInWateringProcess);
    Serial.print("isEndWateringTime > ");
    Serial.println(zone3.isEndWateringTime(currentTime));
    Serial.print("EndWateringTime > ");
    Serial.println(zone3.endTime);
    Serial.print("daysToWatering > ");
    Serial.println(zone3.daysToWatering);
    Serial.print("startTime > ");
    Serial.println(zone3.startTime);
    Serial.print("duration > ");
    Serial.println(zone3.duration);
    
//Обновление статуса бака:
if(isTankFull()){
    tank_status = 2;
    if(SetupButtonCheck() == 1) return;
    Serial.println("tank = 2");
}else{
    if(isTankEmpty()){
        tank_status = 0;
    }else{
        tank_status = 1;
    }
    if(SetupButtonCheck() == 1) return;
}

//Проверка зон:
  if(tank_status != 0 && isInFillingTankProcess == false){
    Serial.println("check zone");
    if(zone1.isStartWateringTime(currentTime)){zone1.startWatering();}
    if(zone2.isStartWateringTime(currentTime)){zone2.startWatering();}
    if(zone3.isStartWateringTime(currentTime)){zone3.startWatering();}
  };
  if((zone1.isEndWateringTime(currentTime))&&(zone1.isInWateringProcess)) {zone1.stopWatering();     Serial.println("z st water");}
  if((zone2.isEndWateringTime(currentTime))&&(zone2.isInWateringProcess)) {zone2.stopWatering();     Serial.println("z st water");}
  if((zone3.isEndWateringTime(currentTime))&&(zone3.isInWateringProcess)) {zone3.stopWatering();     Serial.println("z st water");}

//Проверка бака:
  if((isFillingTankTime(currentTime))&&(tank_status != 2) && isInFillingTankProcess == false){
    Serial.println("poputka nabratb back 1");
    if((!zone1.isInWateringProcess)&&(!(zone1.timeToWateringStart(currentTime)<30))
    && (!zone2.isInWateringProcess)&&(!(zone2.timeToWateringStart(currentTime)<30))
    && (!zone3.isInWateringProcess)&&(!(zone3.timeToWateringStart(currentTime)<30))){
      startFillingTank();
      }
  }
  if(isInFillingTankProcess == true){
    Serial.println("poputka zackrutb back 1");
    if((zone1.timeToWateringStart(currentTime)<5)
    || (zone2.timeToWateringStart(currentTime)<5)
    || (zone3.timeToWateringStart(currentTime)<5)
    || (tank_status == 2)){stopFillingTank();}
  }
  if(tank_status == 0 && isInFillingTankProcess == false){
    Serial.println("mr Back is empty. So we need a 1,5 of beer!");
    if(zone1.isInWateringProcess)zone1.stopWatering();
    if(zone2.isInWateringProcess)zone2.stopWatering();
    if(zone3.isInWateringProcess)zone3.stopWatering();
    // тут можно сделать принудительный длинный сон. сделать небольшой цикл, который будет опрашивать только кнопку и датчик дождя и ждать время набора бака.
    if(isFillingTankTime(currentTime)){ startFillingTank(); }
  }
  
  //delay(50); power.sleepDelay(60000); delay(50);//можно sleepDelay заменить на супер-дупер сон
}
