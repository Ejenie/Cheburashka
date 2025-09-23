#include "platform_motors.h"
#include "serials.h"
#include "servos.h"
#include "mpu.h"

bool conditionFlag[4] = {false, false, false, false};
static int condition = 0;

void conditionUpdate() {    //обновление текущего состояния в зависимости от предыдущих действий и текущих данных с камеры
  dataCheck();
  if (data == "face" && !conditionFlag[1]) {
    condition = 1;
  }
  else if ( data == "orange"  && !conditionFlag[2]) {
    condition = 2;
  }
  else if (data == "green" && !conditionFlag[3]) {
    condition = 3;
  }
  else if (data == "greeting" && !conditionFlag[4]){
    condition = 4;
  }
}

void conditionBegin() {
 /* forwardEnc(1);
  Serial.println("begin programm");*/
  turnServo();
  turnEncRight(-100);
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
  conditionFlag[1] = true;
  uint32_t timer = millis();
  while (millis() - timer < 2000);
  condition = 0;
}

void orangeCond() {
  flagDefault = false;
  conditionFlag[2] = true;
  handOrange();
  uint32_t timer = millis();
  while (millis() - timer < 2000);
  condition = 0;
}

void greenCond() {
  flagDefault = false;
  conditionFlag[3] = true;
  handClap();
  Serial.println("clap");  
  uint32_t timer = millis();
  while (millis() - timer < 1000);
  handScream();
  Serial.println("scream");  
  timer = millis();
  while (millis() - timer < 3000);
  condition = 0;
}

void greetCond() {
  flagDefault = true;
  conditionFlag[4] = true;
  turnServo();  
  Serial.println("greeting");
  hi();
  uint32_t timer = millis();
  while (millis() - timer < 3000);
  condition = 0;
}
