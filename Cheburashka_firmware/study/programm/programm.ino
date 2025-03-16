#include "platform_motors.h"
#include "serials.h"
#include "servos.h"

void setup() {
  _initSerials();
  _initEnc();
  _initMotors();
  //_initServo();
}

void loop() {
  // _testServo();
  //handOne();
  //_checkEnc();
  //_checkSerial3In();
  turnEnc(-200);
  stopm(10000);
  /*static uint32_t timer = millis();
    while (millis() - timer < 5000 ) {
    rotateRight(20);
    }
    _stopmLib();*/
}
