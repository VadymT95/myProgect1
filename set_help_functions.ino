#include "set_help_functions.h"
byte knotSet(byte stringAddress){
    Serial.println("knotSet");
    byte mode = 0;//пункт меню, находящийся в данный момент на экране
    byte deepOfRecursion = numberOfDigits(userPath);
    byte quantity = mainMenuArray[stringAddress][0].toInt();
    boolean push = 0;
    lcd.clear();
    lcd.print(mainMenuArray[stringAddress][1]);
    lcd.setCursor(0,1);
    lcd.print(t.hour);
    lcd.setCursor(4,1);
    lcd.print(t.min);
    lcd.setCursor(0,0);
    if(typeOfBackButtonClick = 1 && deepOfRecursion == 0) typeOfBackButtonClick = 0;
    while(true){
      Serial.println("recursion");
        if(SetupButtonCheck() == 0)break;    
        menuUpdate(mode, mainMenuArray[stringAddress], quantity);
        //////////
        if(userPathMemory > 9){       
            mode = (userPathMemory%10)-1;
            push = 1;
            userPathMemory = userPathMemory/10;
            }else{userPathMemory = 0;}
        /////////
        if(backButtonCheck())break;
        if(setButton.isSingle()|| push == 1){   //если нажат "enter"
              push = 0;
              if(mainMenuArray[stringAddress][(mode*2+2)].toInt() >= 40){
                  userPath += (mode+1) * int(ceil(pow(10,(deepOfRecursion))));         
                  chooseSet(userPath, mainMenuArray[stringAddress][(mode*2+2)].toInt());
                    Serial.println("a step f");               

                  }
            if(mainMenuArray[stringAddress][(mode*2+2)].toInt() < 40) {    
                  userPath += (mode+1) * int(ceil (pow(10,(deepOfRecursion))));
                  knotSet(mainMenuArray[stringAddress][(mode*2+2)].toInt());
                    Serial.println("b step f");               

                  }
                    Serial.println("c step f");               

              if(typeOfBackButtonClick == 1)break;         
             }
        }
       Serial.println("return 0");
       return 0;
    } 
void chooseSet(long userPath, byte outValue){
  switch(outValue){
     case 50: 
       switch (userPath%10){
           case 1: saveSetZone(userPath,0,59);break;
           //case 2: saveSet(0,59,tankMinutesMemory);break;
        }
       break;
     case 51:
       switch (userPath%10){
           case 1: saveSetZone(userPath,0,23);break;
           //case 2: saveSet(0,23,tankHoursMemory);break;
        }
       break;
     case 52: 
        switch (userPath%10){
           case 1: saveBoolSetZone(userPath);break;
           case 4: saveSet(rainSensorIsTurnedOnMemory);break;
           default: break;
        }
        break;
     case 53: saveSet(tankModeMemory);break; //булевый saveSet
     case 54: dateSaveSet(); break;
     case 55: saveSetZone(userPath,1,14); break;//14 - максимальное кол-во дней для паузы
     case 56: timeSaveSet();break;
     case 57: tankCustomTimeSet(); break;//
  }
    Serial.println("choose set f");               
}
//------------------------------
//Help functions
int read_potent (int interval){
     int value  =  analogRead(Poten_pin);
     return constrain(map(value, 0, 1024, 0, interval + 1),0,interval);
  } 
int read_potent (int lowerLimit,int upperLimit){
     int value  =  analogRead(Poten_pin);
     return constrain(map(value, 0, 1024, lowerLimit, upperLimit+1),lowerLimit,upperLimit);
  }
byte numberOfDigits(long num){//потестить
    byte digits = 0;
    for(;num>0; digits++){
      num/=10;
    }
    return digits;
}
void LCDprintResult (byte firstValue, byte secondValue, byte setCursorColumn,byte setCursorInLine){
  // выводит результат - время
    lcd.clear ();
    lcd.print("save ");
    if(firstValue >=10){
        lcd.setCursor(setCursorInLine,setCursorColumn);
        lcd.print(firstValue);
    }
    if(firstValue <=9){
        lcd.setCursor(setCursorInLine + 1,setCursorColumn);
        lcd.print(firstValue);
        lcd.setCursor(setCursorInLine,setCursorColumn);
        lcd.print("0");
    }
    lcd.setCursor(setCursorInLine + 2,setCursorColumn); 
    lcd.print(".");
    if(secondValue <=9){
        lcd.setCursor(setCursorInLine + 3,setCursorColumn); 
        lcd.print("0");
        lcd.setCursor(setCursorInLine + 4,setCursorColumn);
        lcd.print(secondValue);
    } 
    if(secondValue >=10){ 
        lcd.setCursor(setCursorInLine + 3,setCursorColumn);
        lcd.print(secondValue);
    }
}
//MenuPrinters-----------------
void menuInitialize(byte mode, String items[]){
    lcd.clear();
    lcd.print(items[mode]);
}
void menuUpdate(byte &mode, String items[], byte quantity){
   if(quantity > 1){;
   if(mode!=read_potent(quantity-1)){//Если пользователь сместил потенциометр - обновляем экран
       mode = read_potent(quantity-1);
       lcd.clear();
       lcd.print(items[mode*2+1]);   
   }}
}
//----------------------------
//value set menu printers-----
void printValue(byte value, byte line, byte column){
    if(value >=10){
        lcd.setCursor(line,column);
        lcd.print(value);
    }
    if(value <=9){
        lcd.setCursor(line + 1,column);
        lcd.print(value);
        lcd.setCursor(line,column);
        lcd.print("0");
    }
  }
void printFormattedValue(byte &value,byte lowerLimit,byte upperLimit){
  if(value != read_potent(lowerLimit,upperLimit)){
    value = read_potent(lowerLimit,upperLimit);
    printValue(value,5,1);
    }
  }
//перегрузка printValue для булевого значения
void printValue(bool value, byte line, byte column){
  if(backButtonCheck()){return;}
  lcd.setCursor(line,column);
  if(value==0) lcd.print("ON ");
  if(value==1) lcd.print("OFF");
}
//перегрузка printFormattedValue для булевого значения
void printFormattedValue(bool &value){
  if(backButtonCheck()){return;}
  if(value != read_potent(1)){
    value = read_potent(1);
    printValue(value,5,1);
  }
}
//----------------------------
// Setters
void saveSet(byte lowerLimit,byte upperLimit, byte memoryCell){
  Serial.println("ss start");
  byte value = read_potent(lowerLimit,upperLimit);//value = -1;???
  bool savePermit = 1;
  printValue(value,5,1);//инициализация
  while(true){
    printFormattedValue(value,lowerLimit,upperLimit);
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;break;}
  }
  if(savePermit == 1){ 
    lcd.clear();
    lcd.print("saved: ");
    printValue(value,7,0);
    EEPROM.update(memoryCell,value);
    //if(memoryCell == tankMinutesMemory || memoryCell == tankHoursMemory)EEPROM.update(tankModeMemory,1); // даже для Вадима это костыль. Нужно переделать меню
    while(true){
       if(backButtonCheck()==1)break;
       }
   }  
}
//перегрузка saveSet для булевого значения
void saveSet(byte memoryCell){
    bool value = read_potent(1);//value = -1;???
    bool savePermit = 1;
    printValue(value,5,1);
    while(true){
        Serial.println("save Set bool");
        if(backButtonCheck()==1){savePermit =0;break;}
        printFormattedValue(value);
        if(setButton.isSingle()) break;
    }
    if(savePermit == 1){  
        lcd.clear();
        lcd.print("saved: ");
        printValue(value,7,0);
    //  Serial.println("save");  
        EEPROM.update(memoryCell,!value);
        while(true){
           if(backButtonCheck()==1)break;
        }
     }
   Serial.println("save Set bool final");
}
//версия saveSet с возвратом значения вместо записи в память
byte saveSetReturn(byte lowerLimit,byte upperLimit){
    byte value = 61;//value = -1;???
    bool savePermit = 1;
    
    while(true){
        printFormattedValue(value,lowerLimit,upperLimit);
        if(setButton.isSingle())break;
        if(backButtonCheck()==1){savePermit =0;break;}
    }
    if(savePermit == 1){  
        lcd.clear();
        lcd.print("saved: ");
        printValue(value,7,0);
    //  Serial.println("save");
            return value;       
    }
    return 100;
}
void dateSaveSet(){
  bool savePermit = 1;
  byte DaysValue, MonthValue, YearsValue = 0;
  lcd.clear();
  lcd.print("date: ??.??.20  ");
  lcd.setCursor(13,1); lcd.print("^^");
  while(savePermit){
    if(YearsValue != read_potent(0,50)){
        YearsValue = read_potent(0,50);
        printValue(YearsValue,14,0);}
    if(setButton.isSingle())break; 
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  lcd.setCursor(9,1); lcd.print("^^     ");
  while(savePermit){
    if(MonthValue != read_potent(1,12)){
        MonthValue = read_potent(1,12);
        printValue(MonthValue,9,0);}
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  byte maxDay = daysInMonth(MonthValue, YearsValue);//определяем кол-во дней в данном месяце
  lcd.setCursor(6,1); lcd.print("^^    ");
  while(savePermit){
    if(DaysValue != read_potent(1,maxDay)){
        DaysValue = read_potent(1,maxDay);
        printValue(DaysValue,6,0);}
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;return;}
    }
  if(savePermit == 1){   
    lcd.setCursor(6,1);lcd.print("  ");
    lcd.setCursor(0,0);lcd.print("saved");
    rtc.setDate(DaysValue, MonthValue, 2000+YearsValue);
      while(true){
    if(backButtonCheck()==1){break;}
      }
  }
  //Сохранение в память
}
byte daysInMonth(byte currentMonth, byte currentYear){
  switch(currentMonth){
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      return 31; break;
    case 4: case 6: case 9: case 11:
      return 30; break;
    case 2:
      if((currentYear%100==0)&&(currentYear%400!=0)) return 28;
      if(currentYear%4==0) return 29;
      return 28;
      break;
  }
}
void timeSaveSet(){
  bool savePermit = 1;
  byte minutesValue, hoursValue = 0;
  lcd.clear();
  lcd.print("time:   .??");
  lcd.setCursor(6,1); lcd.print("^^");
  while(savePermit){
    if(hoursValue != read_potent(0,23)){
        hoursValue = read_potent(0,23);
        printValue(hoursValue,6,0);}
    if(setButton.isSingle())break; 
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  lcd.setCursor(6,1); lcd.print("   ^^");
  while(savePermit){
    if(minutesValue != read_potent(0,59)){
        minutesValue = read_potent(0,59);
        printValue(minutesValue,9,0);}
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  if(savePermit == 1){   
    lcd.setCursor(9,1);lcd.print("  ");
    lcd.setCursor(0,0);lcd.print("saved");
    rtc.setTime(hoursValue,minutesValue,0);
      while(true){
    if(backButtonCheck()==1){break;}
      }
  }
}
void tankCustomTimeSet(){
  bool savePermit = 1;
  byte startMinutesValue, startHoursValue = 0;
  byte stopMinutesValue, stopHoursValue = 0;
  lcd.clear();
  lcd.print("   :??--??:??");
  lcd.setCursor(1,1); lcd.print("^^");
  while(savePermit){//стартовые часы
    if(startHoursValue != read_potent(0,23)){
      startHoursValue = read_potent(0,23);
      printValue(startHoursValue,1,0);
    }
    if(setButton.isSingle())break; 
    if(backButtonCheck()==1){savePermit=0;return;}
  }
  lcd.setCursor(1,1); lcd.print("   ^^");
  while(savePermit){//стартовые минуты
    if(startMinutesValue != read_potent(0,59)){
        startMinutesValue = read_potent(0,59);
        printValue(startMinutesValue,4,0);}
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  lcd.setCursor(4,1); lcd.print("    ^^");
  while(savePermit){//финишные часы
    if(stopHoursValue != read_potent(0,23)){
      stopHoursValue = read_potent(0,23);
      printValue(stopHoursValue,8,0);
    }
    if(setButton.isSingle())break; 
    if(backButtonCheck()==1){savePermit=0;return;}
  }
  lcd.setCursor(8,1); lcd.print("   ^^");
  while(savePermit){//финишные минуты
    if(stopMinutesValue != read_potent(0,59)){
        stopMinutesValue = read_potent(0,59);
        printValue(stopMinutesValue,11,0);}
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  //сохранение
  if(savePermit == 1){   
    lcd.setCursor(9,1);lcd.print("  ");
    lcd.setCursor(0,0);lcd.print("saved");
    EEPROM.update(tankStartHoursMemory, startHoursValue);
    EEPROM.update(tankStartMinutesMemory, startMinutesValue);
    EEPROM.update(tankStopHoursMemory, stopHoursValue);
    EEPROM.update(tankStopMinutesMemory, stopMinutesValue);
      while(true){
    if(backButtonCheck()==1){break;}
      }
  }
  Serial.println("1 step f");               
}
//saveSet для zone
void saveSetZone(long userPath,byte lowerLimit,byte upperLimit){
  byte value = 0;
  value = saveSetReturn(lowerLimit,upperLimit);
  if(value != 100){
  switch ((int((userPath)/1000))%10){
    case 1:  //duration
      switch(((userPath)/10)%10){
                //в каждом кейсе записываем в память значение saveSetReturn(interval, memoryCell); memoryCell уникальная для каждого кейса
        case 1: if(upperLimit==59 ){
                      EEPROM.update(zone1DurationMinutesMemory,value);
                }else{EEPROM.update(zone1DurationHoursMemory,value);} break; //сохранение в энергонезависимую память
        case 2: if(upperLimit==59){
                      EEPROM.update(zone2DurationMinutesMemory,value);
                }else{EEPROM.update(zone2DurationHoursMemory,value);} break; //сохранение в энергонезависимую память
        case 3: if(upperLimit==59){
                      EEPROM.update(zone3DurationMinutesMemory,value);
                }else{EEPROM.update(zone3DurationHoursMemory,value);} break;//сохранение в энергонезависимую память
      }
      break;
    case 2://start time
      switch(((userPath)/10)%10){
        case 1: if(upperLimit==59){
                      EEPROM.update(zone1StartMinutesMemory,value);
                }else{EEPROM.update(zone1StartHoursMemory,value);} break; //сохранение в энергонезависимую память
        case 2: if(upperLimit==59){
                      EEPROM.update(zone2StartMinutesMemory,value);
                }else{EEPROM.update(zone2StartHoursMemory,value);} break; //сохранение в энергонезависимую память
        case 3: if(upperLimit==59){
                      EEPROM.update(zone3StartMinutesMemory,value);
                }else{EEPROM.update(zone3StartHoursMemory,value);} break;//сохранение в энергонезависимую память
      }
      break;
    case 3://pause of days
      switch(((userPath)/10)%10){
        case 1: EEPROM.update(zone1PauseOfDaysMemory,value); break; //сохранение в энергонезависимую память
        case 2: EEPROM.update(zone2PauseOfDaysMemory,value); break; //сохранение в энергонезависимую память
        case 3: EEPROM.update(zone3PauseOfDaysMemory,value); break; //сохранение в энергонезависимую память
      }
  }
  while(true){
     if(backButtonCheck()==1)break; // если нажать тумблер, то будет https://demotivation.ru/wp-content/uploads/2020/03/1066111.gif
  
  }
  }
}
//булевый saveSet для zone
void saveBoolSetZone(long userPath){
  switch(((userPath)/10)%10){
    case 1: saveSet(zone1IsTurnedOnMemory); break;//сохранение в энергонезависимую память
    case 2: saveSet(zone2IsTurnedOnMemory); break;//сохранение в энергонезависимую память
    case 3: saveSet(zone3IsTurnedOnMemory); break;//сохранение в энергонезависимую память
      }
}
boolean backButtonCheck(){
   if(backButton.isSingle()){
       userPathMemory = userPath;
       userPath = 0;
       typeOfBackButtonClick = 1;
       return 1;}
   if(backButton.isHolded() || (SetupButtonCheck() == 0)){
       typeOfBackButtonClick = 1;
       userPath = 0;
       return 1;
       }else{return 0;}
  }
