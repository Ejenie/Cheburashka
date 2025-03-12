void _initSerials() {
  Serial.begin(115200);
  Serial3.begin(115200);
}

void _checkSerial3in() {
  if (Serial3.available()) {
    Serial.println(Serial3.read());
  }
}

void _checkSerial3to() {
  for (int i = 0; i < 1000000; i++) {
    Serial3.write(i);
  }
}
