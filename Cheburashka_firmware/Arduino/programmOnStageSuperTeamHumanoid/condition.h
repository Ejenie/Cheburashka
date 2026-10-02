#include "platform_motors.h"
#include "servos.h"
#include "display.h"
//#include "mpu.h"

bool flagDefault = true;
bool flagEmotion = true;
bool conditionFlag[6] = { false, false, false, false, false, false };
static int condition = 0;

void conditionUpdate() {  //обновление текущего состояния в зависимости от предыдущих действий и текущих данных с камеры
  dataCheck();
  if (data == "start" && !conditionFlag[1]) {
    condition = 1;
  } else if (data == "buster" && (!conditionFlag[2])) {
    condition = 2;
  } else if (data == "talk" && !conditionFlag[3]) {
    condition = 3;
  } else if (data == "rat" && !conditionFlag[4]) {
    condition = 4;
  } else if (data == "party" && !conditionFlag[5]) {
    condition = 5;
  }
}

void condRatReg(int velMx = 0) {
  regRat();
  Serial.println("condRatRegBreak");
}

void conditionBegin() {
  forwardEncN(1);
  hi();
  delay(3000);
  Serial.println("begin programm");
  handClap();
  uint32_t timer = millis();
  while (millis() - timer < 2000)
    ;
  Serial.println("start serial");
}

void startCond() {
  Serial.println("start condition");
  flagDefault = true;
  conditionFlag[1] = true;
  talk();
  uint32_t timer = millis();
  while (millis() - timer < 5000)
    ;
  condition = 0;
}

void busterCond() {
  Serial.println("buster condition");
  flagDefault = true;
  conditionFlag[2] = true;
  Serial.println("scream");
  handScream();
  earsClose();
  flagEmotion = false;
  condition = 0;
}


void talkCond() {
  Serial.println("talk condition");
  flagDefault = true;
  conditionFlag[3] = true;
  earsOpen();
  stopm(1000);
  beginServo();
  turnEncRight();
  Serial.println("talk_comm");
  hi();
  stopm(5000);
  turnEncLeft();
  flagEmotion = true;
  condition = 0;
}

void partyCond() {
  uint32_t timer = millis();
  while (millis() - timer < 60000) {
    stopm(200);
    earsFly(2);
    circle();
    earsFly(20, 600);
  }
}

void defaultCond() {  //стандратное состояние
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
  while (millis() - timer < 5000)
    ;
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
  beginServo(flagEmotion);
  flagDefault = false;
  conditionFlag[2] = true;
  stopm(5000);
  //_deinitServo();
  spinRat(13000);
  //_initServo();
  //handRightWrite(70, 8);
  //handLeftWrite(95, 8);
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
  earsFly(2);
  uint32_t timer = millis();
  while (millis() - timer < 8000)
    ;
  circle();
  earsFly(20, 600);
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
