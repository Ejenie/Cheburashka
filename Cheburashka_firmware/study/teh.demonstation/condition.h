#include "platform_motors.h"
#include "servos.h"
#include "display.h"
//#include "mpu.h"

bool flagEmotion = true;
bool conditionFlag[5] = {false, false, false, false, false};
static int condition = 0;

void conditionUpdate() {    //обновление текущего состояния в зависимости от предыдущих действий и текущих данных с камеры
  dataCheck();
  if (data == "orange" && !conditionFlag[1]) {
    condition = 1;
  }
  else if (data == "fright"  && (!conditionFlag[2])) {
    condition = 2;
  }
  else if (data == "angry" && !conditionFlag[3]) {
    condition = 3;
  }
  else if (data == "sad" && !conditionFlag[4]) {
    condition = 4;
  }
}

void conditionBegin() {
  forwardEnc(1);
  Serial.println("begin programm");
  hi();
}

bool flagDefault = true;
void defaultCond() {    //стандратное состояние
  if (flagDefault) {
    beginServo(flagEmotion);
    flagDefault = false;
  }
}

void genaCond() {
  flagDefault = true;
  conditionFlag[1] = true;
  handScream();
  earClose();
  flagEmotion = false;
  condition = 0;
}

void frightCond() {
  flagDefault = true;
  conditionFlag[2] = true;
  handScream();
  earClose();
  flagEmotion = false;
  uint32_t timer = millis();
  while(millis() - timer < 5000);
  Serial.println("frightEnd");
  condition = 0;
}

void orangeCond() {
  flagEmotion = true;
  beginServo(flagEmotion);
  flagDefault = true;
  conditionFlag[1] = true;
  handOrange();
  uint32_t timer = millis();
  while(millis() - timer < 6000);
  condition = 0;
}

void angryCond() {
  flagDefault = true;
  conditionFlag[3] = true;
  earOpen();
  handAngry();
  turnServo();
  flagEmotion = false;
  uint32_t timer = millis();
  while(millis() - timer < 7000);
  beginServo(true);
  Serial.println("angryEnd");
}

void sadCond() {
  flagDefault = true;
  conditionFlag[4] = true;
  flagEmotion = true;
  beginServo(flagEmotion);
  uint32_t timer = millis();
  while(millis() - timer < 5000);
  Serial.println("sadEnd");
}

void ratCond() {
  //beginServo(flagEmotion);
  flagDefault = true;
  conditionFlag[2] = true;
  spinRat(22000);
  spinRed();
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
