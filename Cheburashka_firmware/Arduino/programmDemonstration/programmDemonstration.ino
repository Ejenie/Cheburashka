#include "condition.h"

void setup() {  //init
  _initSerials();
  _initEnc();
  _initMotors();
  delay(1000);
  _initServo();
  //_initMPU();
  // _initMag();
  //_initDisplay();

  conditionBegin();  //*/
}

void loop() {  //finite state machine

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
  }  //*/
}
