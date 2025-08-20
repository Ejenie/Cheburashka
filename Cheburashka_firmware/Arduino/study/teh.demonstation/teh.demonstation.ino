#include "condition.h"

void setup() {    //инициализация устройств
  _initSerials();
  _initEnc();
  _initMotors();
  _initServo();
  //_initMPU();
  _initMag();
  //_initDisplay();

 /* stopm(7000);
  conditionBegin();//*/
  uint32_t timer = millis();

  //ratCond();
  //frightCond();
}

void loop() {   //конечный автомат выполнения действий по сценарию
 angryCond();
 defaultCond();
 /* conditionUpdate();
  switch (condition) {
    case 0:
      defaultCond();
      break;
    case 1:
      orangeCond();
      break;
    case 2:
      frightCond();
      break;
    case 3:
      angryCond();
      break;
    case 4:
      sadCond();
      break;
    default: break;
  }//*/
}
