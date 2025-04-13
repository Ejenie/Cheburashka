#include "platform_motors.h"
#include "serials.h"
#include "servos.h"
//#include "mpu.h"

void setup() {
  _initSerials();
  _initEnc();
  _initMotors();
  _initServo();
  //_initMPU();

  /*delay(1000);
  forwardEnc(2.4);
  stopm(1000);//--1*/
}

bool one = true;

void loop() {
  static uint32_t timer = 0;
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    Serial.println(data);
    /*if (data == "hello" && one) {
      handScream();
      timer = millis();
      one = false;
    }
    if (data == "orange") {
      
    }*/
  }//*/
 /* if ((millis() - timer > 25000) && (!one)) {
    shoulRight.write(90);
    handRight.write(50);
    handLeft.write(80);
  }*/
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
