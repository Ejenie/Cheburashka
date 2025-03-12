#include "platform_motors.h"
#include "serials.h"

void setup() {
  _initSerials();
  _initEnc();

}

void loop() {
  _checkSerial3in();
}
