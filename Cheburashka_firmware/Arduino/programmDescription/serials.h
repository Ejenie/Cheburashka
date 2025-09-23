void _initSerials() {
  Serial.begin(9600);
  Serial3.begin(9600);
}

void _checkSerial3In() {
  if (Serial.available()) {
    Serial.println(Serial.read());
  }
}

void _checkSerial3To() {
  for (int i = 0; i < 1000000; i++) {
    Serial3.write(i);
  }
}

String oldData = "";
String data = "";
uint32_t timerData = millis();

void dataCheck() {
  if (Serial.available()) {
    String actualData = Serial.readStringUntil('\n');
    if (actualData != oldData || (millis() - timerData > 7000)) {
      data = actualData;
      oldData = actualData;
      timerData = millis();
    }
    else data = "";
    Serial.println("Arduino: you send " + data);
  }
}

int id = 0;
int idOld = 0;
void checkID() {
  if (Serial.available()) {
    int idNow = Serial.read();
    if (idNow != idOld) {
      id = idNow;
      idOld = idNow;
    }
  }
}
