void _initSerials() {
  Serial.begin(9600);
  Serial3.begin(9600);
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
