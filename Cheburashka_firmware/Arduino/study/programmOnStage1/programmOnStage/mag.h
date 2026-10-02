#define LEFTMAG 48
#define RIGHTMAG 50

void _initMag() {
  pinMode(LEFTMAG, OUTPUT);
  pinMode(RIGHTMAG, OUTPUT);
}

void onMag(uint32_t t = 8000) {
  uint32_t timer = millis();
  while (millis() - timer < t) {
    pinMode(LEFTMAG, HIGH);
    pinMode(RIGHTMAG, HIGH);
  }
  pinMode(LEFTMAG, LOW);
  pinMode(RIGHTMAG, LOW);
}

void offMag() {
  pinMode(LEFTMAG, LOW);
  pinMode(RIGHTMAG, LOW);
}
