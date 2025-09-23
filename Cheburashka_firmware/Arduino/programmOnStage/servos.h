#include <Servo.h>
#include "mag.h"

int pinServo[7] = {2, 3, 4, 5, 6, 7, 45};
Servo handLeft;
Servo handRight;
Servo shoulLeft;
Servo shoulRight;
Servo earRight;
Servo earLeft;
Servo turn;

//shoulright 110 40

void beginServo(bool fEmotion = true) {
  if (fEmotion) {
    shoulLeft.write(90);
    shoulRight.write(90);
    handRight.write(50);
    handLeft.write(120);
    earRight.write(110);
    earLeft.write(80);
    turn.write(90);
  }
}

void _initServo() {
  handLeft.attach(2); shoulLeft.attach(4);
  handRight.attach(5); shoulRight.attach(3);
  earLeft.attach(7); earRight.attach(6);
  turn.attach(45);

  beginServo();
}

void hi() {
  shoulRight.write(110);
  for (int j = 0; j < 2; j++) {
    for (int i = 80; i < 140; i++) {
      handRight.write(i);
      delay(12);
    }
    for (int i = 140; i > 80; i--) {
      handRight.write(i);
      delay(12);
    }
  }
  for (int i = 80; i > 50; i--) {
    handRight.write(i);
    delay(12);
  }
  shoulRight.write(90);
}

void handOrange() {
  for (int i = 50; i < 140; i++) {
    handRight.write(i);
    handLeft.write(map(i, 50, 130, 135, 45));
    delay(20);
  };

  for (int i = 90; i > 76; i--) {
    shoulRight.write(i);
    shoulLeft.write(map(i, 90, 76, 90, 104));
    delay(20);
  }//*/

  /*onMag(8000);
  offMag();*/
}

void handClap() {
  for (int i = 50; i < 140; i++) {
    handRight.write(i);
    handLeft.write(map(i, 50, 130, 120, 30));
    delay(12);
  }
  stopm(200);
  for (int i = 0; i < 3; i++) {
    for (int i = 90; i > 65; i--) {
      shoulRight.write(i);  Q1
      shoulLeft.write(map(i, 90, 65, 90, 115));
      delay(15);
    }
    for (int i = 65; i < 90; i++) {
      shoulRight.write(i);
      shoulLeft.write(map(i, 65, 90, 115, 90));
      delay(15);
    }
    stopm(30);
  }
  for (int i = 140; i > 50; i--) {
    handRight.write(i);
    handLeft.write(map(i, 130, 50, 30, 120));
    delay(20);
  }
}

void handForw() {
  for (int i = 50; i < 90; i++) {
    handRight.write(i);
    handLeft.write(180 - i);
    delay(20);
  }

  for (int i = 90; i > 80; i--) {
    shoulRight.write(i);
    shoulLeft.write(90 + (90 - i));
    delay(20);
  }//*/
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

void handScream() {
  for (int i = 50; i < 160; i++) {
    handRight.write(i);
    handLeft.write(map(i, 50, 160, 120, 0));
    delay(10);
  }
  shoulRight.write(60);
  shoulLeft.write(110);
  /*for (int i = 90; i > 60; i--) {
    shoulRight.write(i);
    shoulLeft.write(map(i, 90, 60, 120, 90));
    delay(20);
    }//*/
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
