/*----------------------*/
//#include <Motors.h>
#include "mpu.h"
#include "serials.h"

//left motor of foot
#define MFL_EN 9
#define MFL_IN1 29
#define MFL_IN2 35

//right motor of foot
#define MFR_EN 8
#define MFR_IN1 28
#define MFR_IN2 34

//left motor
#define ML_EN 49
#define ML_A 24
#define ML_B 25
#define ML_PWM 46

#define ML_ENC_INT 20
#define ML_ENC_DIG 26

//right motor
#define MR_EN 51
#define MR_A 22
#define MR_B 23
#define MR_PWM 44

#define MR_ENC_INT 21
#define MR_ENC_DIG 27

#define VEL_L 70
#define VEL_R 70

#define REVERSE -1

#define TURN90 343

volatile int16_t encL = 0, encR = 0;

//Motors motorB = Motors(46, 24, 25, 0, 9); //A, B, PWM, INTERRUPT,
//Motors motorC = Motors(44, 22, 23, 1, 8);
/*----------------------*/
//клаасс моторов платформы создавать н-
int64_t timerL = 0, timerR = 0, time_encL = 0, time_encR = 0;
void _encoderL() {
  time_encL = micros() - timerL;
  if (digitalRead(ML_ENC_DIG)) {
    time_encL = -abs(time_encL);
    encL--;
  } else {
    time_encL = abs(time_encL);
    encL++;
  }
  timerL = micros();
}

void _encoderR() {
  time_encR = micros() - timerR;
  if (digitalRead(MR_ENC_DIG)) {
    time_encR = -abs(time_encR);
    encR--;
  } else {
    time_encR = abs(time_encR);
    encR++;
  }
  timerR = micros();
}

void _initEnc() {
  pinMode(ML_ENC_DIG, INPUT);
  pinMode(MR_ENC_DIG, INPUT);
  pinMode(ML_ENC_INT, INPUT);
  pinMode(MR_ENC_INT, INPUT);
  attachInterrupt(digitalPinToInterrupt(ML_ENC_INT), _encoderL, RISING);
  attachInterrupt(digitalPinToInterrupt(MR_ENC_INT), _encoderR, RISING);
}

int16_t len_to_pulses(float n) {
  return (n / (3.14 * 0.1)) * 372;
}

void _checkEnc() {
  Serial.print("encL: ");
  Serial.print(digitalRead(26));  //platform_encoders[0]);
  Serial.print("\t");
  Serial.print("encR: ");
  Serial.println(digitalRead(27));  //platform_encoders[1]);
}

void _checkInt() {
  Serial.print("intL: ");
  Serial.print(digitalRead(ML_ENC_INT));
  Serial.print("\t");
  Serial.print("intR: ");
  Serial.println(digitalRead(MR_ENC_INT));
}

void _checkDIG() {
  Serial.print("digL: ");
  Serial.print(digitalRead(ML_ENC_DIG));
  Serial.print("\t");
  Serial.print("digR: ");
  Serial.println(digitalRead(MR_ENC_DIG));
}

void _initMotors() {
  pinMode(ML_EN, OUTPUT);
  pinMode(ML_A, OUTPUT);
  pinMode(ML_B, OUTPUT);
  pinMode(ML_PWM, OUTPUT);
  digitalWrite(ML_EN, HIGH);

  pinMode(MR_EN, OUTPUT);
  pinMode(MR_A, OUTPUT);
  pinMode(MR_B, OUTPUT);
  pinMode(MR_PWM, OUTPUT);
  digitalWrite(MR_EN, HIGH);

  //motors of foots
  pinMode(MFL_EN, OUTPUT);
  pinMode(MFL_IN1, OUTPUT);
  pinMode(MFL_IN2, OUTPUT);
  pinMode(MFR_EN, OUTPUT);
  pinMode(MFR_IN1, OUTPUT);
  pinMode(MFR_IN2, OUTPUT);
}

void motorsFoots(int mL = 170, int mR = 170, uint32_t t = 500) {
  analogWrite(MFL_EN, mL);
  if (mL > 0) {
    digitalWrite(MFL_IN1, LOW);
    digitalWrite(MFL_IN2, HIGH);
  } else {
    digitalWrite(MFL_IN1, HIGH);
    digitalWrite(MFL_IN2, LOW);
  }
  //
  analogWrite(MFR_EN, mR);
  if (mR > 0) {
    digitalWrite(MFR_IN1, LOW);
    digitalWrite(MFR_IN2, HIGH);
  } else {
    digitalWrite(MFR_IN1, HIGH);
    digitalWrite(MFR_IN2, LOW);
  }
}

void movement_of_foots(uint32_t t = 500) {
  static uint32_t timer = millis();
  if ((millis() - timer) < 550) {
    motorsFoots(-220, 140);
  } else if ((millis() - timer) < 900) {
    motorsFoots(145, -140);
  } else {
    timer = millis();
  }
}

int sgn(int n) {
  if (n > 0) return 1;
  else return -1;
}

void zeroEnc() {
  encL = 0;
  encR = 0;
}

void rotateLeft(int powerMotor) {
    Serial.print("left: ");
    Serial.println(powerMotor); //*/
  int vel = map(abs(powerMotor), 0, 100, 0, 255);
  //если powerMotor < 0 то LOW, если > 0 то HIGH
  if (powerMotor > 0) {
    digitalWrite(ML_A, HIGH);
    digitalWrite(ML_B, LOW);
  } else {
    digitalWrite(ML_A, LOW);
    digitalWrite(ML_B, HIGH);
  }
  analogWrite(ML_PWM, constrain(vel, -255, 255));
}

void rotateRight(int powerMotor) {
  /*
    Serial.print("right: ");
    Serial.println(powerMotor);//*/
  int vel = map(abs(powerMotor), 0, 100, 0, 255);
  //если powerMotor < 0 то LOW, если > 0 то HIGH
  if (powerMotor > 0) {
    digitalWrite(MR_A, LOW);
    digitalWrite(MR_B, HIGH);
  } else {
    digitalWrite(MR_A, HIGH);
    digitalWrite(MR_B, LOW);
  }
  analogWrite(MR_PWM, constrain(vel, -255, 255));
}

float speedL = 1, speedR = 1;
float errOldL = 0, errOldR = 0, iLEnc = 0, iREnc = 0, errIL = 0, errIR = 0;
void speedControl(int rpmL, int rpmR, float kPspeed = 0.1, float kIspeed = 0.1) {
  /* speedL = (3.14 * 0.1) / ((time_encL) / 1000.0 * 380);
    speedR = (3.14 * 0.1) / ((time_encR) / 1000.0 * 380);*/

  speedL = 60000000 / 100 / time_encL;  // 1/time_enc * epr
  speedR = 60000000 / 100 / time_encR;

  float errL = (rpmL - speedL), errR = (rpmR - speedR);
  float uL = errL * kPspeed, uR = errR * kPspeed;

  //movement_of_foots();
  rotateLeft(uL);
  rotateRight(uR);
  /* Serial.print("speedL: ");
    Serial.print(speedL);
    Serial.print("  speedR: ");
    Serial.print(speedR);
    Serial.print("  uL: ");
    Serial.print(uL);
    Serial.print("  uR: ");
    Serial.println(uR);//*/
}

void stopm(uint32_t t = 2000) {
  uint32_t timer = millis();
  while (millis() - timer < t) {
    //rotateRight(-30);
    delay(200);
    digitalWrite(ML_A, LOW);
    digitalWrite(ML_B, LOW);
    digitalWrite(MR_A, LOW);
    digitalWrite(MR_B, LOW);
  }
}

void forwardEnc(int16_t dist = 1200, float kM = 1, int velMx = 50, float kp = 10, float kd = 1, float ki = 0.001, float kv = 0.1) {
  zeroEnc();
  dist = (len_to_pulses(dist)) * kM;
  int16_t encLReg = encL, encRReg = encR, errOldEnc = 0, errEnc = 0, dEnc = 0, iEnc = 0, vel = 0;
  float u = 0, kpReg = 0, kdReg = 0, kiReg = 0;
  velMx = abs(velMx);
  while ((encLReg + encRReg) < abs(dist) * 2) {
    //movement_of_foots();
    encLReg = abs(encL);
    encRReg = abs(encR);
    if ((encLReg + encRReg) < abs(dist) * 1.5) vel = (encLReg + encRReg) / 2;
    else vel = abs(dist) - (encLReg + encRReg) / 2;
    vel += 20;
    if (vel > velMx) vel = velMx;
    errEnc = (encRReg - encLReg);
    iEnc += errEnc;
    u = float(errEnc) * kp + float(iEnc) * kiReg;
    errOldEnc = errEnc;
    rotateLeft(sgn(dist) * (vel + u));
    rotateRight(sgn(dist) * (vel - u));
    ;
  }
  //motorsFoots(-70, -70);
  stopm(300);
  //motorsFoots(0, 0);
  stopm(40);
}

void forwardTime(int32_t time = 2000, int16_t dist = 1200, float kM = 1, int velMx = 50, float kp = 10, float kd = 1, float ki = 0.001, float kv = 0.1) {
  dist = (len_to_pulses(dist)) * kM;
  int16_t encLReg = encL, encRReg = encR, errOldEnc = 0, errEnc = 0, dEnc = 0, iEnc = 0, vel = 0;
  float u = 0, kpReg = 0, kdReg = 0, kiReg = 0;
  velMx = abs(velMx);
  uint32_t timer = millis();
  while (millis() - timer < time) {
    encLReg = abs(encL);
    encRReg = abs(encR);
    if ((encLReg + encRReg) < abs(dist) * 1.5) vel = (encLReg + encRReg) / 2;
    else vel = abs(dist) - (encLReg + encRReg) / 2;
    vel += 20;
    if (vel > velMx) vel = velMx;
    errEnc = (encRReg - encLReg);
    iEnc += errEnc;
    u = float(errEnc) * kp + float(iEnc) * kiReg;
    errOldEnc = errEnc;
    rotateLeft(sgn(dist) * (vel + u));
    rotateRight(sgn(dist) * (vel - u));
    ;
  }
  //motorsFoots(-70, -70);
  stopm(300);
  //motorsFoots(0, 0);
  stopm(40);
}

void turnEncRight(int16_t dist = -343, int velMx = 40, float kp = 10, float kd = 1, float ki = 0.001, float kv = 0.1) {
  zeroEnc();
  int16_t encLReg = encL, encRReg = encR, errOldEnc = 0, errEnc = 0, dEnc = 0, iEnc = 0, vel = 0;
  float u = 0, kpReg = 0, kdReg = 0, kiReg = 0;
  velMx = abs(velMx);

  while ((encLReg) < abs(dist)) {
    //  Serial.println("beginRegTurn");
    encLReg = abs(encL);
    encRReg = abs(encR);
    vel = float(vel) * kv + 20;
    if (vel > velMx) vel = velMx;
    kpReg = float(kp * vel) / 100.0;
    errEnc = (encRReg - encLReg);
    iEnc += errEnc;
    if (sgn(errOldEnc) != sgn(errEnc)) iEnc = 0;
    u = float(errEnc) * kpReg + dEnc * kdReg + float(iEnc) * kiReg;
    errOldEnc = errEnc;

    rotateLeft(-1 * sgn(dist) * constrain(vel + u, -velMx, velMx));
    rotateRight(sgn(dist) * constrain((vel - u), -velMx, velMx));
    /* Serial.print("vel and u end: ");
      Serial.print(vel);
      Serial.print("  ");
      Serial.println(u);
      Serial.println();*/
  }
  stopm(1000);
}


void turnEncLeft(int16_t dist = -1200, int velMx = 220, float kp = 10, float kd = 1, float ki = 0.001, float kv = 0.1) {
  zeroEnc();
  int16_t encLReg = encL, encRReg = encR, errOldEnc = 0, errEnc = 0, dEnc = 0, iEnc = 0, vel = 0;
  float u = 0, kpReg = 0, kdReg = 0, kiReg = 0;
  velMx = abs(velMx);

  while ((encLReg) < abs(dist)) {
    speedControl(-velMx, velMx);
  }
  stopm(1000);
  _checkEnc();
}

void turnTimeLeft(uint32_t t, int velMx = 60, float kp = 10, float kd = 10, float ki = 0.001, float kv = 0.1) {
  zeroEnc();
  int16_t encLReg = encL, encRReg = encR, errOldEnc = 0, errEnc = 0, dEnc = 0, iEnc = 0, vel = 0;
  float u = 0, kpReg = 0, kdReg = 0, kiReg = 0;
  velMx = abs(velMx);
  uint32_t timer = millis();
  Serial.println("start");

  while (millis() - timer < t) {
    Serial.println(constrain((vel + u), -velMx, velMx));
    encLReg = abs(encL);
    encRReg = abs(encR);
    vel = float(vel) * kv + 20;
    if (vel > velMx) vel = velMx;
    errEnc = (encRReg - encLReg);
    u = float(errEnc) * kpReg;
    rotateLeft(-1 * constrain((vel - u), -velMx, velMx));
    rotateRight(constrain((vel + u), -velMx, velMx));  //*/
    /*Serial.print("errEnc and u: ");
      Serial.print(errEnc);
      Serial.print("  ");
      Serial.println(u);
      Serial.println();*/
  }
}

void turnTimeRight(uint32_t t, int velMx = 60, float kp = 10, float kd = 1, float ki = 0.001, float kv = 0.1) {
  zeroEnc();
  int16_t encLReg = encL, encRReg = encR, errOldEnc = 0, errEnc = 0, dEnc = 0, iEnc = 0, vel = 0;
  float u = 0, kpReg = 0, kdReg = 0, kiReg = 0;
  velMx = abs(velMx);
  uint32_t timer = millis();

  while (millis() - timer < t) {
    //movement_of_foots();
    encLReg = abs(encL);
    encRReg = abs(encR);
    vel = float(vel) * kv + 20;
    if (vel > velMx) vel = velMx;
    errEnc = (encRReg - encLReg);
    u = float(errEnc) * kpReg;
    rotateLeft(constrain((vel - u), -velMx, velMx));
    rotateRight(-1 * constrain((vel + u), -velMx, velMx));  //*/
    /*Serial.print("errEnc and u: ");
      Serial.print(errEnc);
      Serial.print("  ");
      Serial.println(u);
      Serial.println();*/
  }
}

void spinRat(uint32_t t) {
  Serial.println("spinRat");
  turnTimeLeft(t);
}

bool flagToGreen = false;
uint32_t timerRed = millis();
void spinToGreen(uint32_t t = 20000, int velMx = 30, float kp = 10, float kd = 1, float ki = 0.001, float kv = 0.1) {
  zeroEnc();
  int16_t encLReg = encL, encRReg = encR, errOldEnc = 0, errEnc = 0, dEnc = 0, iEnc = 0, vel = 0;
  float u = 0, kpReg = 0, kdReg = 0, kiReg = 0;
  velMx = abs(velMx);
  while (!flagToGreen) {
    //movement_of_foots();
    encLReg = abs(encL);
    encRReg = abs(encR);
    vel = float(vel) * kv + 20;
    if (vel > velMx) vel = velMx;
    errEnc = (encRReg - encLReg);
    u = float(errEnc) * kpReg;
    rotateLeft(-1 * constrain((vel - u), -velMx, velMx));
    rotateRight(constrain((vel + u), -velMx, velMx));
    dataCheck();
    if (data == "aruco1") {
      flagToGreen = true;
      motorsFoots(0, 0);
    }
  }
}

bool flagToRat = false;
void regRat(int velMx = 20, float kp = 10, float kd = 1, float ki = 0.001, float kv = 0.1) {
  zeroEnc();
  int16_t encLReg = encL, encRReg = encR, errOldEnc = 0, errEnc = 0, dEnc = 0, iEnc = 0, vel = 0;
  float u = 0, kpReg = 0, kdReg = 0, kiReg = 0;
  velMx = abs(velMx);
  while (!flagToRat) {
    encLReg = abs(encL);
    encRReg = abs(encR);
    vel = float(vel) * kv + 20;
    if (vel > velMx) vel = velMx;
    errEnc = (encRReg - encLReg);
    u = float(errEnc) * kpReg;
    rotateLeft(-1 * constrain((vel - u), -velMx, velMx));
    rotateRight(constrain((vel + u), -velMx, velMx));
    dataCheck();
    if (dataDist == "break") {
      flagToRat = true;
    }
  }
}

void motorsAruco() {
  forwardEnc(1);
  //  turnEncRight(343);
  forwardEnc(1);
  stopm(2000);
}

void motorsFootStop() {
  motorsFoots(-70, -70);
  stopm(300);
  motorsFoots(0, 0);
}

/*-------------LIB MOTORS-------------*/
/*void _stopmLib(uint32_t t = 1000) {
  uint32_t timer = millis();
  while (millis() - timer < t) {
    motorB.stop();
    motorC.stop();
  }
}

void _forwardEncLib(int16_t enc = 250) {
  encL = 0;
  while (encL < enc) {
    motorB.set(VEL_L);
    motorC.set(REVERSE * VEL_R);
  }
  _stopmLib();
}

void _forwardTimeLib(uint32_t t = 1000) {
  uint32_t timer = millis();
  while (millis() - timer < t) {
    motorB.set(VEL_L);
    motorC.set(REVERSE * VEL_R);
  }
  _stopmLib();
}

void _rightEncLib(int16_t enc = 254) {
  encL = 0;
  while (encL < enc) {
    motorB.set(VEL_L);
    motorC.set(REVERSE * (-VEL_R));
  }
  _stopmLib();
}
*/