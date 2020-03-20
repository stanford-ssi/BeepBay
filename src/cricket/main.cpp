#include <Arduino.h>

#include "ff.h"
#include "SSISD.hpp"
#include "TinyGPS++.h"


void displayInfo();

#define GPS_BAUD 9600

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
Uart SerialS6C( &sercom0, PIN_SERIAL_S6C_RX, PIN_SERIAL_S6C_TX, PAD_SERIAL_S6C_RX, PAD_SERIAL_S6C_TX ) ;
Uart SerialGPS( &sercom5, PIN_SERIAL_GPS_RX, PIN_SERIAL_GPS_TX, PAD_SERIAL_GPS_RX, PAD_SERIAL_GPS_TX ) ;

void SERCOM5_Handler(void) {
  SerialGPS.IrqHandler();
}

void SERCOM0_Handler(void) {
  SerialS6C.IrqHandler();
}

void setup() {
  Serial.begin(9600);
  SerialGPS.begin(GPS_BAUD);
  SerialS6C.begin(9600);
}

void loop(){

// print out data from GPS module to serial
  if (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      displayInfo();
      delay(50);
    }
  }

// print out data from S6C to serial
  if (SerialS6C.available() > 0) {
    Serial.write(SerialS6C.read());
  }

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