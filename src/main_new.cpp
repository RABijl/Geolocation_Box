#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif


//// set these for the destination
static const double destLat = 51.889364 , destLon = 4.334446;

//// GPS ////
static void smartDelay(unsigned long ms);

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
SoftwareSerial ss(RXPin, TXPin);

TinyGPSPlus gps;

//// LCD ////
U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R2, /* reset=*/ U8X8_PIN_NONE);


void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  
  u8g2.begin();
}


void loop()
{

}


// make sure that gps serial is not blocked
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}