#include "condition.h"

void setup() {    //инициализация устройств
  _initSerials();
  _initEnc();
  _initMotors();
  _initServo();
  //_initMPU();
  _initMag();

  /*stopm(1000);
  conditionBegin();*/
  turnServo();
}

void loop() {   //конечный автомат выполнения действий по сценарию
 /* conditionUpdate();
  switch (condition) {
    case 0:
      defaultCond();
      break;
    case 1:
      faceCond();
      break;
    case 2:
      orangeCond();
      break;
    case 3:
      greenCond();
      break;
    case 4:
      greetCond();
      break;
    default: break;
  }//*/
}
