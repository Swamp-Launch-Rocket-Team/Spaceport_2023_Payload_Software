// Test code for Ultimate GPS Using Hardware Serial (e.g. GPS Flora or FeatherWing)
//
// This code shows how to listen to the GPS module via polling. Best used with
// Feathers or Flora where you have hardware Serial and no interrupt
//
// Tested and works great with the Adafruit GPS FeatherWing
// ------> https://www.adafruit.com/products/3133
// or Flora GPS
// ------> https://www.adafruit.com/products/1059
// but also works with the shield, breakout
// ------> https://www.adafruit.com/products/1272
// ------> https://www.adafruit.com/products/746
//
// Pick one up today at the Adafruit electronics shop
// and help support open source hardware & software! -ada

#include <Adafruit_GPS.h>

// what's the name of the hardware serial port?
#define XBEESerial Serial
#define GPSSerial Serial1
#define UART5Serial Serial2

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint32_t timer = millis();


void setup()
{
  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  // pinMode(38, OUTPUT);  // Set pin 38 as an output
  // digitalWrite(38, HIGH);  // Set pin 38 high
  XBEESerial.begin(9600);

  UART5Serial.begin(115200);
  UART5Serial.println("Adafruit GPS library basic parsing test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
}

void loop() // run over and over again
{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) UART5Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    UART5Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    // timer = millis(); // reset the timer
    // UART5Serial.print("\nTime: ");
    // if (GPS.hour < 10) { UART5Serial.print('0'); }
    // UART5Serial.print(GPS.hour, DEC); UART5Serial.print(':');
    // if (GPS.minute < 10) { UART5Serial.print('0'); }
    // UART5Serial.print(GPS.minute, DEC); UART5Serial.print(':');
    // if (GPS.seconds < 10) { UART5Serial.print('0'); }
    // UART5Serial.print(GPS.seconds, DEC); UART5Serial.print('.');
    // if (GPS.milliseconds < 10) {
    //   UART5Serial.print("00");
    // } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
    //   UART5Serial.print("0");
    // }
    // UART5Serial.println(GPS.milliseconds);
    // UART5Serial.print("Date: ");
    // UART5Serial.print(GPS.day, DEC); UART5Serial.print('/');
    // UART5Serial.print(GPS.month, DEC); UART5Serial.print("/20");
    // UART5Serial.println(GPS.year, DEC);
    // UART5Serial.print("Fix: "); UART5Serial.print((int)GPS.fix);
    // UART5Serial.print(" quality: "); UART5Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      String dataBuffer = "Lat: " + String(GPS.latitudeDegrees, 4) + ", Lon: " + String(GPS.longitudeDegrees, 4);
      UART5Serial.print(dataBuffer);
      XBEESerial.print(dataBuffer);

      // UART5Serial.print("Location: ");
      // UART5Serial.print(GPS.latitude, 4); UART5Serial.print(GPS.lat);
      // UART5Serial.print(",");
      // UART5Serial.print(GPS.longitude, 4); UART5Serial.println(GPS.lon);
      // UART5Serial.print("Speed (knots): "); UART5Serial.println(GPS.speed);
      // UART5Serial.print("Angle: "); UART5Serial.println(GPS.angle);
      // UART5Serial.print("Altitude: "); UART5Serial.println(GPS.altitude);
      // UART5Serial.print("Satellites: "); UART5Serial.println((int)GPS.satellites);
      // UART5Serial.print("Antenna status: "); UART5Serial.println((int)GPS.antenna);
    }
  }
}