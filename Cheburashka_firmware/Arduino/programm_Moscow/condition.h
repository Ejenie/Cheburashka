#include "platform_motors.h"
#include "servos.h"
#include "mpu.h"

bool conditionFlag[4] = {false, false, false, false};
static int condition = 0;

void conditionUpdate() {    //обновление текущего состояния в зависимости от предыдущих действий и текущих данных с камеры
  dataCheck();
  if (data == "face" && !conditionFlag[0]) {
    condition = 1;
  }
  else if ( data == "orange"  && !conditionFlag[1]) {
    condition = 2;
  }
  else if (data == "green" && !conditionFlag[2]) {
    condition = 3;
  }
  else if (data == "greeting" && !conditionFlag[3]) {
    condition = 4;
  }
  else if (data == "orangeAdd") {
    condition = 2;
  }
}

void conditionBegin() {
  forwardEnc(1);
  turnServo();
 // turnEncRight(-100);
  hi();
}

bool flagDefault = true;
void defaultCond() {    //стандратное состояние
  if (flagDefault) {
    beginServo();
    flagDefault = false;
  }
}

void faceCond() {
  flagDefault = true;
  conditionFlag[0] = true;
  uint32_t timer = millis();
  while (millis() - timer < 2000);
  condition = 0;
}

void orangeCond() {
  flagDefault = false;
  conditionFlag[1] = true;
  handOrange();
  uint32_t timer = millis();
  while (millis() - timer < 10000);
  condition = 0;
}

void greenCond() {
  if (conditionFlag[1]) NhandOrange();
  flagDefault = true;
  conditionFlag[2] = true;
  Serial.println("clap");
  handClap();
  uint32_t timer = millis();
  while (millis() - timer < 5000);
  Serial.println("scream");
  handScream();
  earRight.write(180);
  earLeft.write(0);
  timer = millis();
  while (millis() - timer < 5000);
  Serial.println("break");
  earRight.write(0);
  earLeft.write(180);
  talk();
  condition = 0;
}

void greetCond() {
  flagDefault = true;
  conditionFlag[3] = true;
  turnServo();
  Serial.println("greeting");
  hi();
  uint32_t timer = millis();
  while (millis() - timer < 3000);
  condition = 0;
}
