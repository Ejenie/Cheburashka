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

  //turnEncLeft();
}

void loop() {  //finite state machine
  conditionUpdate();
  switch (condition) {
    case 0:
      defaultCond();
      break;
    case 1:
      startCond();
      break;
    case 2:
      busterCond();
      break;
    case 3:
      talkCond();
      break;
    case 4:
      ratCond();
      break;
    case 5:
      partyCond();
      break;
    default: break;
  }  //*/
}
