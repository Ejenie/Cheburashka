#include <Servo.h>
#include <ServoSmooth.h>

#define pinhandLeft 2
#define pinhandRight 5
#define pinshoulLeft 4
#define pinshoulRight 3
#define pinearLeft 7
#define pinearRight 6
#define pinTurn 45

#define defSpeedServo 250
#define defAccelServo 0.2

ServoSmooth handLeft;
ServoSmooth handRight;
ServoSmooth shoulLeft;
ServoSmooth shoulRight;
ServoSmooth earRight;
ServoSmooth earLeft;
ServoSmooth turn;

void _initServo() {
  handLeft.attach(pinhandLeft, 170);
  handLeft.setSpeed(defSpeedServo);
  handLeft.setAccel(defAccelServo);
  //
  shoulLeft.attach(pinshoulLeft, 90);
  shoulLeft.setSpeed(defSpeedServo);
  shoulLeft.setAccel(defAccelServo);
  //
  handRight.attach(pinhandRight, 10);
  handRight.setSpeed(defSpeedServo);
  handRight.setAccel(defAccelServo);
  //
  shoulRight.attach(pinshoulRight, 90);
  shoulRight.setSpeed(defSpeedServo);
  shoulRight.setAccel(defAccelServo);
  //
  earLeft.attach(pinearLeft, 80);
  earLeft.setSpeed(defSpeedServo);
  earLeft.setAccel(defAccelServo);
  //
  earRight.attach(pinearRight, 110);
  earRight.setSpeed(defSpeedServo);
  earRight.setAccel(defAccelServo);
  //
  turn.attach(pinTurn, 90);
  turn.setSpeed(defSpeedServo);
  turn.setAccel(defAccelServo);
}

void servoTick() {
  handLeft.tick();
  handRight.tick();
  shoulLeft.tick();
  shoulRight.tick();
  earRight.tick();
  earLeft.tick();
  turn.tick();
}

uint32_t tmr = millis();
bool flag = true;
void hi() {
  for (int j = 0; j < 2; j++) {
    handRight.tick();
    if (millis() - tmr >= 500) {
      tmr = millis();
      flag = !flag;
      handRight.setTargetDeg(flag ? 140 : 80);
    }
  }
  handRight.setTargetDeg(50);
}
