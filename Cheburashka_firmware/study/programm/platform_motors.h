/*----------------------*/
#include <Motors.h>
//left motor
#define ML_EN 49
#define ML_A 24
#define ML_B 25
#define ML_PWM 46

#define ML_ENC_INT 21
#define ML_ENC_DIG 9

//right motor
#define MR_EN 51
#define MR_A 22
#define MR_B 23
#define MR_PWM 44

#define MR_ENC_INT 20
#define MR_ENC_DIG 8

#define VEL_L 70
#define VEL_R 70

volatile int16_t platform_encoders[2] = {0, 0};
volatile int16_t encL = 0, encR = 0;

Motors motorB = Motors(46, 24, 25, 0, 9);
Motors motorC = Motors(44, 22, 23, 1, 8);
/*----------------------*/
//клаасс моторов платформы создавать н-
void _encoderL() {
  if (digitalRead(ML_ENC_DIG)) encL--; else encL++;
}

void _encoderR() {
  if (digitalRead(MR_ENC_DIG)) encR--; else encR++;
}

void _initEnc() {
  pinMode(ML_ENC_DIG, INPUT);
  pinMode(MR_ENC_DIG, INPUT);
  pinMode(ML_ENC_INT, INPUT);
  pinMode(MR_ENC_INT, INPUT);
  attachInterrupt(digitalPinToInterrupt(ML_ENC_INT), _encoderL, RISING);
  attachInterrupt(digitalPinToInterrupt(MR_ENC_INT), _encoderR, RISING);
}

void _checkEnc() {
  Serial.print("encL: ");
  Serial.print(encL);//platform_encoders[0]);
  Serial.print("\t");
  Serial.print("encR: ");
  Serial.println(encR);//platform_encoders[1]);
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

void _sinhrEnc() {
  static float k1 = 3, k2 = 20, k3 = 0.7, k4 = 0.05, dT = 10.0/*!!!*/, angVel = 0/*!!!*/, angle = 0 /*!!!*/;
  static int16_t encOld = 0;
  int16_t enc = (encL + encR) / 2; float encVel = (enc - encOld) / (0.001 * dT); encOld = enc;
  float u = angVel * k1 + angle * k2 + enc * k3 + encVel * k4;  
}

void _stopm(uint32_t t = 1000) {
  uint32_t timer = millis();
  while (millis() - timer < t) {
    motorB.stop();
    motorC.stop();
  }
}

void _forwardEnc(int16_t enc = 250) {
  encL = 0;
  while (encL < enc) {
    motorB.set(VEL_L);
    motorC.set(-VEL_R);
  }
  _stopm();
}

void _forwardTime(uint32_t t = 1000) {
  uint32_t timer = millis();
  while (millis() - timer < t) {
    motorB.set(VEL_L);
    motorC.set(-VEL_R);
  }
  _stopm();
}

void _rightEnc(int16_t enc = 254) {
  encL = 0;
  while (encL < enc) {
    motorB.set(VEL_L);
    motorC.set(VEL_R);
  }
  _stopm();
}
