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
  regRat();
  Serial.println("condRatRegBreak");
}

void conditionBegin() {
  forwardEnc(1);
  hi();
  Serial.println("begin programm");
  stopm(2000);
  handClap();
  uint32_t timer = millis();
  while (millis() - timer < 2000);
  turnServo();
  Serial.println("start serial");
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
  uint32_t timer = millis();
  while (millis() - timer < 5000);
  handScream();
  earsClose();
  flagEmotion = false;
  condition = 0;
}

void orangeCond() {
  flagEmotion = false;
  beginServo(flagEmotion);
  flagDefault = false;
  conditionFlag[3] = true;
  earsOpen();
  handOrange();
  condition = 0;
}
void ratCond() {
  NhandOrange();
  //beginServo(flagEmotion);
  flagDefault = false;
  conditionFlag[2] = true;
  stopm(3000);
  spinRat(22000);
  spinToGreen();
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
  flagDefault = false;
  conditionFlag[4] = true;
  stopm(200);
  earsFly(5); 
  uint32_t timer = millis();
  while(millis() - timer < 6000);
  condition = 0; 
  /*conditionFlag[4] = true;
    handClap();
    uint32_t timer = millis();
    while (millis() - timer < 2000);
    condition = 0;*/
}

bool flagArUco = true;
void arUcoCond() {
  /*flagDefault = true;
  conditionFlag[4] = true;
  stopm(200);
  earsFly();
  condition = 0; //*/
}
