#include <Arduino.h>
#include "TinyGPS++.h"
#include "min.h"

#define GPS_BAUD 9600
#define S6C_BAUD 9600

#define MESSAGE_SEND 0
#define MESSAGE_SET_MODE 1
#define MESSAGE_SET_FREQUENCY 2
#define MESSAGE_SET_DATARATE 3
#define MESSAGE_SET_INTERVAL 4
#define MESSAGE_SET_LENGTH 5
#define MESSAGE_SET_CONTINUOUS 6
#define MESSAGE_QUICKSAVE 0xF5
#define MESSAGE_QUICKLOAD 0xF9
#define MESSAGE_QUICKACK 0xFA
#define MESSAGE_SEND_CONFIG 7
#define MESSAGE_ARM 8
#define MESSAGE_DISARM 9

#define GPS_DATA_INTERVAL 3600

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
Uart SerialS6C( &sercom0, PIN_SERIAL_S6C_RX, PIN_SERIAL_S6C_TX, PAD_SERIAL_S6C_RX, PAD_SERIAL_S6C_TX ) ;
Uart SerialGPS( &sercom5, PIN_SERIAL_GPS_RX, PIN_SERIAL_GPS_TX, PAD_SERIAL_GPS_RX, PAD_SERIAL_GPS_TX ) ;

struct min_context min_ctx_s6c;

unsigned long lastSend = 0;
bool firstAcquired = false;

void displayInfo();
void sendCoords();
