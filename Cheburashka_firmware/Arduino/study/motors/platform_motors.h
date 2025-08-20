/*----------------------*/
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

volatile int16_t platform_encoders[2] = {0, 0};
volatile int16_t encL = 0, encR = 0;
/*----------------------*/
//клаасс моторов платформы создавать н-
void _encoderL() {
  if (digitalRead(ML_ENC_DIG)) encL++; else encL--;//platform_encoders[0]++; else platform_encoders[0]--;
}

void _encoderR() {
  if (digitalRead(MR_ENC_DIG)) encR++; else encR--;//platform_encoders[1]++; else platform_encoders[1]--;
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
