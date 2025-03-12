#include "platform_motors.h"
#include "serials.h"
#include "servos.h"

void setup() {
  _initSerials();
  _initEnc();
  //_initServo();
}

void loop() {
  // _testServo();
  _forwardEnc(500);
  _stopm();
  _rightEnc();
  _stopm();
  //handOne();
  //_checkInt();
  //_checkSerial3In();
}
