#include <Arduino.h>


Uart SerialStrato( &sercom0, PIN_SERIAL_STRATO_RX, PIN_SERIAL_STRATO_TX, PAD_SERIAL_STRATO_RX, PAD_SERIAL_STRATO_TX ) ;

void SERCOM0_Handler(void) {
  SerialStrato.IrqHandler();
}

void setup() {
  Serial.begin(9600);
  SerialStrato.begin(9600);
}

void loop() {
  if (SerialStrato.available() > 0) {
    Serial.write(SerialStrato.read());
  }
}