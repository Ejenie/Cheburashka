#include <Servo.h>

int pinServo[7] = {2, 3, 4, 5, 6, 7, 45};
Servo handLeft;
Servo handRight;
Servo shoulLeft;
Servo shoulRight;
Servo earRight;

void _initServo() {
  handLeft.attach(6); shoulLeft.attach(3);
  handRight.attach(7); shoulRight.attach(45);
  earRight.attach(4);

  shoulLeft.write(90);
  shoulRight.write(90);
  handRight.write(135);
  handLeft.write(180);
  earRight.write(90);
}

void _testServo() {
  for (int i = 0; i < 180; i++) {
    handRight.write(i);
    delay(10);
  }
  for (int i = 180; i > 0; i--) {
    handRight.write(i);
    delay(10);
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
        handRight.write(270-i);
        delay(25);
      }
      for (int i = 150; i > 90; i--) {
        handLeft.write(i + 20);
        handRight.write(270-i);
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
