#include <Servo.h>

int pinServo[7] = {2, 3, 4, 5, 6, 7, 45};
Servo handLeft;
Servo handRight;
Servo shoulLeft;
Servo shoulRight;
Servo earRight;

//shoulright 110 40

void _initServo() {
  handLeft.attach(3); shoulLeft.attach(6);
  
  handRight.attach(7); shoulRight.attach(45);
  // earRight.attach(4);

  // shoulLeft.write(90);
  shoulRight.write(90);
  handRight.write(50);
  handLeft.write(80);
  //  earRight.write(90);
}

void handScream() {
  for (int i = 50; i < 165; i++) {
    handRight.write(i);
    handLeft.write(160 - i);
    delay(10);
  }
  for (int i = 90; i > 60; i--) {
    shoulRight.write(i);
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
