#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <U8g2lib.h>
#include <Wire.h>


//// config section ////
static const double DESTLAT = 51.889364 , DESTLON = 4.334446;
static const int32_t SERIALBAUD = 9600;

//// function headers ////
static void smartDelay(unsigned long ms);
static void ToDisplay(const char * str);
static void IntToCharArray(char * out, uint32_t val, bool valid);
static void LoadingAnimation(char * res, int len);

//// GPS ////

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
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
  char str[6];
  IntToCharArray(str, distance, gps.location.isValid());

  ToDisplay(str);

  // debugging purposes
  Serial.println(str);
  Serial.print("number of satellites: ");
  Serial.println(gps.satellites.value());

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
static void ToDisplay(const char * str)
{
  u8g2.clearBuffer();					
  u8g2.setFont(u8g2_font_6x10_mn);
  u8g2.drawStr(0,10,str);
  u8g2.sendBuffer();
}


// create char array to be displayed
static void IntToCharArray(char * out, uint32_t val, bool valid)
{
  int len = strlen(out) -1;
  if (valid)
  {
    sprintf(out, "%lx", val);
  }
  else
  {
    LoadingAnimation(out,len);
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
      res[i] = 'O';
    }
    else
    {
      res[i] = '*';
    }
  }

}
