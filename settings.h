void settings();
//-----------------------------

String mainMenuArray[][9] = {
  /*0*/ {"4",  "zones","6","tank","4","date and time","5","periphery","7"},
  /*1*/ {"2",  "is turned on","52","schedule","2"},
  /*2*/ {"3",  "duration","3","begin time","3","pause of days","55"},
  /*3*/ {"2",  "minutes","50","hours","51"},//---
  /*4*/ {"2",  "after wotering","53","custom time","57"},
  /*5*/ {"2",  "date","54","time","56"},
  /*6*/ {"3",  "zone1","1","zone2","1","zone3","1"},
  /*7*/ {"1",  "rain sensor","52"},
// 50 -- minutes
// 51 -- hours
// 52 -- is turned on
// 53 -- after wotering
// 54 -- dateSaveSet
// 55 -- pause of days
// 56 -- timeSaveSet
// 57 -- tankCustomTime
  };
byte typeOfBackButtonClick = 0;
long userPath = 0;
long userPathMemory = 0;
