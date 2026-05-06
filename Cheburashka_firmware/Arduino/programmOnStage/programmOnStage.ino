#include "condition.h"

void setup() {  //init
  _initSerials();
  _initEnc();
  _initMotors();
  //_initServo();
  //_initMPU();
  // _initMag();
  //_initDisplay();

  spinRat(22000);
/*  stopm(8000);
  conditionBegin();  //*/
  uint32_t timer = millis();
}

void loop() {  //finite state machine
  //_checkEnc();
  /*conditionUpdate();
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
