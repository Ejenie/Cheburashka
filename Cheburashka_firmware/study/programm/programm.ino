#include "platform_motors.h"
#include "serials.h"
#include "servos.h"
//#include "mpu.h"

void setup() {
  _initSerials();
  _initEnc();
  _initMotors();
  //_initServo();
  //_initMPU();
}

void loop() {
  //_testServo();
  //rotateRight(100);
  //Serial.println("dvizh");
  //checkMPU();
  forwardEnc(1);
  stopm(3000);
  //_checkEnc();
  /*for ( int i = 0; i < 4; i++) {
    Serial.println("dvizh1");
    forwardEnc(300);
    _checkEnc();
    stopm(300);
    turnEnc(TURN90);
    _checkEnc();
    stopm(300);
    }
    stopm(5000);//*/
}
