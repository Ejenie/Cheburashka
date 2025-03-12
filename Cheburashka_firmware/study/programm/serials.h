void _initSerials() {
  Serial.begin(115200);
  Serial3.begin(9600);
}

void _checkSerial3In() {
 // Serial.println(Serial3.available());
  if (Serial3.available()) {
    Serial.println(Serial3.read());
  }
}

void _checkSerial3To() {
  for (int i = 0; i < 1000000; i++) {
    Serial3.write(i);
  }
}
