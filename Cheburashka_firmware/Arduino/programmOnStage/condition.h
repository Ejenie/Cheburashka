#include "platform_motors.h"
#include "servos.h"
#include "display.h"
//#include "mpu.h"

bool flagEmotion = true;
bool conditionFlag[5] = {false, false, false, false, false};
static int condition = 0;

void conditionUpdate() {    //обновление текущего состояния в зависимости от предыдущих действий и текущих данных с камеры
  dataCheck();
  if (data == "gena" && !conditionFlag[1]) {
    condition = 1;
    timerRed = millis();
  }
  else if (data == "rat"  && (!conditionFlag[2])) {
    condition = 2;
  }
  else if (data == "orange" && !conditionFlag[3]) {
    condition = 3;
  }
  else if (data == "aruco1" && !conditionFlag[4]) {
    condition = 4;
  }
}


void condRatReg(int velMx = 0) {
  while (dataDist != "break") {
    Serial.println(dataDist);
    distUpdate();
    //регулятор
    
    speedControl(-velMx, velMx);
  }
  Serial.println("condRatRegBreak");
}

void conditionBegin() {
  forwardEnc(1);
  /*for (int i = 90; i > 40; i--) {
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
    }*/
  hi();
  Serial.println("begin programm");
  stopm(2000);
  handClap();
  uint32_t timer = millis();
  while (millis() - timer < 2000);
  turnServo();
}

bool flagDefault = true;
void defaultCond() {    //стандратное состояние
  if (flagDefault) {
    if (!conditionFlag[3])
      beginServo(flagEmotion);
    flagDefault = false;
  }
}

void genaCond() {
  flagDefault = true;
  conditionFlag[1] = true;
  handScream();
  earRightWrite(240, 5);
  earLeftWrite(20, 5);
  flagEmotion = false;
  condition = 0;
}

void orangeCond() {
  flagEmotion = false;
  beginServo(flagEmotion);
  flagDefault = false;
  conditionFlag[3] = true;
  earRightWrite(120, 15);
  earLeftWrite(100, 15);
  handOrange();
  condition = 0;
}
void ratCond() {
  //beginServo(flagEmotion);
  flagDefault = false;
  conditionFlag[2] = true;
  stopm(3000);
  spinRat(22000);
  spinRed();
  //condRatReg();
  stopm(1000);
  condition = 0;
}

void arUco1Cond() {
  conditionFlag[4] = true;
  handClap();
  condition = 0;
}

void greenCond() {
  /*conditionFlag[4] = true;
    handClap();
    uint32_t timer = millis();
    while (millis() - timer < 2000);
    condition = 0;*/
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
