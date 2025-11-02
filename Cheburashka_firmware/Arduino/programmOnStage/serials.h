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

String mySubstr(const String& str, int start, int length = -1) {
    if (str.length() == 0)
        return String();
    int strLen = str.length();
    if (start < 0 || start >= strLen)
        return String(); 
    if (length == -1 || start + length > strLen)
        length = strLen - start;
    if (length <= 0)
        return String();
    String result;
    result.reserve(length);
    
    for (int i = 0; i < length; i++)
        result += str[start + i];
    return result;
}

int myStoi(const String& str) {
  int result = 0;
  int sign = 1;
  int i = 0;
  while (i < str.length() && str[i] == ' ')
    i++;
  if (i < str.length() && str[i] == '-') {
    sign = -1;
    i++;
  } else if (i < str.length() && str[i] == '+') {
    i++;
  }
  while (i < str.length() && str[i] >= '0' && str[i] <= '9') {
    result = result * 10 + (str[i] - '0');
    i++;
  }
  return result * sign;
}

int16_t dist = 0;
String dataDist = "";
void distUpdate(int index = 6) {
  if (Serial.available()) {
    String actualDataDist = Serial.readStringUntil('\n');
    String distStr = mySubstr(actualDataDist, index);
    String dataDist = mySubstr(actualDataDist, 0, (actualDataDist.length() - distStr.length()));
    dist = myStoi(distStr);
    Serial.println(dataDist);
  }
}

void _checkDistance() {
  distUpdate();
  Serial.println(dist);
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
