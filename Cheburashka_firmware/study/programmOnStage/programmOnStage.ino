#include "condition.h"

void setup() {    //инициализация устройств
  _initSerials();
  _initEnc();
  _initMotors();
  _initServo();
  //_initMPU();
  _initMag();

  stopm(5000);
  conditionBegin();//*/
  uint32_t timer = millis();

  //ratCond();
}

void loop() {   //конечный автомат выполнения действий по сценарию
  /* handClap();
    stopm(3000);//*/
  conditionUpdate();
  switch (condition) {
    case 0:
      defaultCond();
      break;
    case 1:
      genaCond();
      break;
    case 2:
      ratCond();
      break;
    case 3:
      orangeCond();
      break;
    case 4:
      greenCond();
      break;
    default: break;
  }//*/
}
