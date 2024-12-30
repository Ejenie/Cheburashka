/*----------------------*/
//left motor
#define ML_EN 0
#define ML_A 0
#define ML_B 0
#define ML_DIR 0

#define ML_ENC_INT 0
#define ML_ENC_PWM 0
//right motor
#define MR_EN 0
#define MR_A 0
#define MR_B 0
#define MR_DIR 0

#define MR_ENC_INT 0
#define MR_ENC_PWM 0

volatile int16_t platform_encoders[2] = {0, 0};
/*----------------------*/
//клаасс моторов платформы создавать н-
void _encoderL() {
  if (digitalRead(ML_ENC_PWM)) platform_encoders[0]++; else platform_encoders[0]--;
}

void _encoderR() {
  if (digitalRead(MR_ENC_PWM)) platform_encoders[1]++; else platform_encoders[1]--;
}
