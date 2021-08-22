#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <U8g2lib.h>
#include <Wire.h>


//// config section ////
static const double DESTLAT = 51.889364 , DESTLON = 4.334446;
static const unsigned long SERIALBAUD = 9600;
static const int TOTALCHARS = 6;

//// function headers ////
static void smartDelay(unsigned long ms);
static void ToDisplay(char * str);
static void IntToCharArray(char * out, int len, unsigned long val, bool valid);
static void LoadingAnimation(char * res, int len);

//// GPS ////

static const int RXPin = 4, TXPin = 3;
static const unsigned long GPSBaud = 9600;
SoftwareSerial ss(RXPin, TXPin);

TinyGPSPlus gps;

//// LCD ////
U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R2,U8X8_PIN_NONE);


void setup()
{
  Serial.begin(SERIALBAUD);
  ss.begin(GPSBaud);
  
  u8g2.begin();
}


void loop()
{
  //cast to unsigned int since distance is positive and we are not interested in decimals
  unsigned long distance= (unsigned long) TinyGPSPlus::distanceBetween(gps.location.lat(),
                                                            gps.location.lng(),
                                                            DESTLAT,
                                                            DESTLON );
  char str[TOTALCHARS +1];
  IntToCharArray(str,TOTALCHARS, distance, gps.location.isValid());

  ToDisplay(str);

  // debugging purposes
  Serial.println(str);
  Serial.print("number of satellites: ");
  Serial.println(gps.satellites.value());
  Serial.print("time: ");
  Serial.println(gps.time.hour());
  Serial.print("current lat and lon: ");
  Serial.print(gps.location.lat(), 5);
  Serial.print(",");
  Serial.println(gps.location.lng(), 5);

  smartDelay(1000);
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

// print to the display
// assuming a 64*32 display and displaying 6 chars
static void ToDisplay( char * str)
{
  u8g2.clearBuffer();					
  u8g2.setFont(u8g2_font_10x20_tn );
  u8g2.drawStr(2,26,str);
  u8g2.sendBuffer();
}


// create char array to be displayed
static void IntToCharArray(char * out, int len, unsigned long val, bool valid)
{
  
  if (valid)
  {
    sprintf(out, "%.6ld", val);
  }
  else
  {
    LoadingAnimation(out,len );
  }

  out[len] = 0;
}


// create a chare array with a moving circle to indicate system is doing something
int counter = 0;
static void LoadingAnimation(char * res, int len)
{
  if (counter > len -1)
  {
    counter = 0;
  }
  else 
  {
    counter++;
  }

  for (int i=0; i<len; ++i)
  {
    if(i == counter)
    {
      res[i] = '0';
    }
    else
    {
      res[i] = '-';
    }
  }

}
