#include <Servo.h>
#include "mag.h"
#define dTime 5

int pinServo[7] = {2, 3, 4, 5, 6, 7, 45};
Servo handLeft;
Servo handRight;
Servo shoulLeft;
Servo shoulRight;
Servo earRight;
Servo earLeft;
Servo turn;

//shoulright 110 40

int shL = 90, shR = 90, hR = 50, hL = 120, eR = 110, eL = 80, tS = 90;
void handRightWrite(int pos) {
  if (pos > hR) {
    for (int i = hR; i < pos; i++) {
      handRight.write(pos);
      delay(dTime);
    }
  }
  else {
    for (int i = hR; i > pos; i--) {
      handRight.write(pos);
      delay(dTime);
    }
  }
  hR = pos;
}

void handLeftWrite(int pos) {
  if (pos > hL) {
    for (int i = hL; i < pos; i++) {
      handLeft.write(pos);
      delay(dTime);
    }
  }
  else {
    for (int i = hL; i > pos; i--) {
      handLeft.write(pos);
      delay(dTime);
    }
  }
  hL = pos;
}

void shoulRightWrite(int pos) {
  if (pos > shR) {
    for (int i = shR; i < pos; i++) {
      shoulRight.write(pos);
      delay(dTime);
    }
  }
  else {
    for (int i = shR; i > pos; i--) {
      shoulRight.write(pos);
      delay(dTime);
    }
  }
  shR = pos;
}

void shoulLeftWrite(int pos) {
  if (pos > shL) {
    for (int i = shL; i < pos; i++) {
      shoulLeft.write(pos);
      delay(dTime);
    }
  }
  else {
    for (int i = shL; i > pos; i--) {
      shoulLeft.write(pos);
      delay(dTime);
    }
  }
  shL = pos;
}

void earRightWrite(int pos) {
  if (pos > eR) {
    for (int i = eR; i < pos; i++) {
      earRight.write(pos);
      delay(dTime);
    }
  }
  else {
    for (int i = eR; i > pos; i--) {
      earRight.write(pos);
      delay(dTime);
    }
  }
  eR = pos;
}

void earLeftWrite(int pos) {
  if (pos > eL) {
    for (int i = eL; i < pos; i++) {
      earLeft.write(pos);
      delay(dTime);
    }
  }
  else {
    for (int i = eL; i > pos; i--) {
      earLeft.write(pos);
      delay(dTime);
    }
  }
  eL = pos;
}
void beginServo() {
  shoulLeftWrite(90);
  shoulRightWrite(90);
  handRightWrite(50);
  handLeftWrite(120);
  earRightWrite(110);
  earLeftWrite(80);
  turn.write(90);
}

void _initServo() {
  handLeft.attach(2, 500, 2080); shoulLeft.attach(4);
  handRight.attach(5, 700, 2540); shoulRight.attach(3);
  earLeft.attach(6); earRight.attach(7);
  turn.attach(45);

  beginServo();
}

void hi() {
  shoulRightWrite(110);
  for (int j = 0; j < 2; j++) {
    for (int i = 80; i < 140; i++) {
      handRightWrite(i);
      delay(12);
    }
    for (int i = 140; i > 80; i--) {
      handRightWrite(i);
      delay(12);
    }
  }
  for (int i = 80; i > 50; i--) {
    handRightWrite(i);
    delay(12);
  }
  shoulRightWrite(90);
}

void handOrange() {
  for (int i = 50; i < 140; i++) {
    handRightWrite(i);
    handLeftWrite(map(i, 50, 130, 120, 30));
    delay(20);
  }

  for (int i = 90; i > 76; i--) {
    shoulRightWrite(i);
    shoulLeftWrite(map(i, 90, 76, 90, 104));
    delay(20);
  }//*/

  /* onMag(8000);
    offMag();*/
}

static uint32_t timerTurn = millis();
void turnServo() {
  for (int i = 90; i < 120; i++) {
    turn.write(i);
    delay(30);
  }
  for (int i = 120; i > 90; i--) {
    turn.write(i);
    delay(30);
  }
  for (int i = 90; i > 60; i--) {
    turn.write(i);
    delay(30);
  }
  for (int i = 60; i < 90; i++) {
    turn.write(i);
    delay(30);
  }
  timerTurn = millis();
}

void handClap() {
  
  for (int i = 50; i < 140; i++) {
    handRightWrite(i);
    handLeftWrite(map(i, 50, 130, 120, 30));
    delay(12);
  }
  stopm(200);
  for (int i = 0; i < 3; i++) {
    for (int i = 90; i > 65; i--) {
      shoulRightWrite(i);
      shoulLeftWrite(map(i, 90, 65, 90, 115));
      delay(8);
    }
    for (int i = 65; i < 90; i++) {
      shoulRightWrite(i);
      shoulLeftWrite(map(i, 65, 90, 115, 90));
      delay(8);
    }
    stopm(30);
  }
  for (int i = 140; i > 50; i--) {
    handRightWrite(i);
    handLeftWrite(map(i, 130, 50, 30, 120));
    delay(20);
  }
}

void handScream() {
  handRightWrite(50);
  handLeftWrite(120);
  for (int i = 50; i < 160; i++) {
    handRightWrite(i);
    handLeftWrite(map(i, 50, 160, 120, 0));
    delay(10);
  }
  shoulRightWrite(60);
  shoulLeftWrite(110);
}

void _testServo() {
  for (int _ = 0; _ < 2; _++) {
    for (int i = 60; i < 140; i++) {
      handRight.write(i);
      delay(60);
    }
    for (int i = 90; i < 140; i++) {
      shoulRight.write(i);
      delay(60);
    }
  }
}

void _testServoOne() {
  for (int _ = 0; _ < 2; _++) {
    for (int i = 40; i < 110; i++) {
      shoulRight.write(i);
      delay(60);
    }
    for (int i = 110; i > 40; i--) {
      shoulRight.write(i);
      delay(60);

    }
  }
}

void handOne() {
  if (Serial3.available()) {
    delay(100);
    int countSerial = Serial3.read();
    while (countSerial == 2) {
      countSerial = Serial3.read();
      for (int i = 90; i < 150; i++) {
        handLeft.write(i);
        earRight.write(i);
        delay(25);
      }
      for (int i = 150; i > 90; i--) {
        earRight.write(i);
        handLeft.write(i);
        delay(25);
      }
    }
    while (countSerial == 1) {
      countSerial = Serial3.read();
      delay(100);
      for (int i = 90; i < 150; i++) {
        handLeft.write(i + 20);
        handRight.write(270 - i);
        delay(25);
      }
      for (int i = 150; i > 90; i--) {
        handLeft.write(i + 20);
        handRight.write(270 - i);
        delay(25);
      }
    }
    while (countSerial == 0) {
      countSerial = Serial3.read();
      Serial.println(countSerial);
      continue;
    }
  }
}
