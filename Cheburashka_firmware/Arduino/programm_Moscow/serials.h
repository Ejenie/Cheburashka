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
    if (actualData != oldData) {
      data = actualData;
      oldData = actualData;
    }
    else data = "";
    Serial.println("you send " + data);
  }
}

int id = 0;
uint32_t idOld = 0, idNow = 0;
void checkID() {
  if (Serial.available()) {
    idNow = Serial.read();
    if (idNow != idOld) {
      id = idNow;
      idOld = idNow;
    }
  }
}
