#include "platform_motors.h"
#include "serials.h"
#include "servos.h"
#include "mpu.h"

bool conditionFlag[4] = {false, false, false};
static int condition = 0;

void conditionUpdate() {    //обновление текущего состояния в зависимости от предыдущих действий и текущих данных с камеры
  dataCheck();
  if (data == "face" && !conditionFlag[1]) {
    condition = 1;
  }
  else if ( data == "orange"  && !conditionFlag[2]) {
    condition = 2;
  }
  else if (data == "aruco" && !conditionFlag[3]) {
    condition = 3;
  }
}

void conditionBegin() {
  forwardEnc(1);
  Serial.println("begin programm");
 /* for (int i = 90; i > 40; i--) {
    turn.write(i);
    delay(20);
  }
  for (int i = 40; i < 90; i++) {
    turn.write(i);
    delay(20);
  }
  for (int i = 90; i < 140; i++) {
    turn.write(i);
    delay(20);
  }
  for (int i = 140; i > 90; i--) {
    turn.write(i);
    delay(20);
  }
  turnEncRight(-100);*/
  hi();
}

bool flagDefault = true;
void defaultCond() {    //стандратное состояние
  if (flagDefault) {
    beginServo();
    flagDefault = false;
  }

  if (millis() - timerTurn > 12000) {
    turnServo();
  }
}

void faceCond() {
  flagDefault = true;
  conditionFlag[1] = true;
  handClap();
  uint32_t timer = millis();
  while (millis() - timer < 5000);
  condition = 0;
}

void orangeCond() {
  flagDefault = true;

  conditionFlag[2] = true;
  handOrange();
  uint32_t timer = millis();
  while (millis() - timer < 8000);
  condition = 0;
}

bool flagArUco = true;
void arUcoCond() {
  flagDefault = true;
  conditionFlag[3] = true;
  stopm(1000);
  /* while (flagArUco) {
     checkID();
     switch (id) {
       case 1:
         forwardEnc(1);
         break;
       case 2:
         turnEncRight();
         break;
       case 3:
         flagArUco = false;
         break;
       default: break;
     }
    }
    // _testServo();
    condition = 0;*/
}
