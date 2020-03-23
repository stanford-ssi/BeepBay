#include "main.h"
#undef min
#undef max

void SERCOM5_Handler(void) {
  SerialGPS.IrqHandler();
}

void SERCOM0_Handler(void) {
  SerialS6C.IrqHandler();
}

void setup() {
  Serial.begin(115200);
  SerialGPS.begin(GPS_BAUD);
  SerialS6C.begin(S6C_BAUD);
  
  delay(2000); // wait for serial monitor to be opened

}

/**
 * Report GPS coordinates every GPS_DATA_INTERVAL.
 * If GPS data stops sending, it is due to power loss
 * rather than not acquiring new GPS data.
 **/
void loop() {

  // If new GPS data  is acquired, read it and encode it.
  if (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
    Serial.println("NEW GPS DATA POINT ACQUIRED");
    if (!firstAcquired) {
      firstAcquired = true;
    }
  }


  // Report GPS coordinates every GPS_DATA_INTERVAL
  // if at least one valid location has been acquired.
  bool time_to_send = (millis() - lastSend) > GPS_DATA_INTERVAL;
  if (time_to_send && firstAcquired) {
      sendCoords();
      displayInfo();
      lastSend = millis();
    }

}

void sendCoords() {
  char message_id = 0x3; // 0b11
  const int msg_len = sizeof(int32_t) * 3 + sizeof(uint8_t);
  uint8_t msg[msg_len + 2];
  msg[0] = MESSAGE_SEND;
  msg[1] = msg_len;
  msg[2] = message_id;

  ((int32_t *)(msg + 3))[0] = long(gps.location.lat()*1000000);
  ((int32_t *)(msg + 3))[1] = long(gps.location.lng()*1000000);
  ((int32_t *)(msg + 3))[2] = long(gps.altitude.feet());

  min_send_frame(&min_ctx_s6c, 0, msg, msg[1] + 3);
}

void displayInfo() {
  /* Print out GPS data to the serial monitor.
   */

  // Format -- Location: lat,lng Date/Time: mo/day/yr hr:min:sec.centisec \n
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

uint16_t min_tx_space(uint8_t port)
{
  uint16_t n = 1;
  if (port == 0)
    n = SerialS6C.availableForWrite();
  return n;
}

void min_tx_byte(uint8_t port, uint8_t byte)
{
  if (port == 0)
    SerialS6C.write(&byte, 1U);
}

uint32_t min_time_ms()
{
  return millis();
}

void min_tx_start(uint8_t port) {}

void min_tx_finished(uint8_t port) { SerialS6C.flush(); }
