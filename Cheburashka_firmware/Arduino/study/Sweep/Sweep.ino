#include "servos.h"

void setup() {
  _initServo();
}

void loop() {
  //servoTick();
  /* handRight.setTargetDeg(140);
    delay(9000);*/
  for (int j = 0; j < 2; j++) {
    handRight.tick();
    if (millis() - tmr >= 300) {
      tmr = millis();
      flag = !flag;
      handRight.setTargetDeg(flag ? 140 : 80);
    }
  }
}
