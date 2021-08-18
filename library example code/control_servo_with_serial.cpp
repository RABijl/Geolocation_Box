

/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include <Servo.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

Servo serv;
int pos = 0;
int newPos = 0;

bool handleInp = false;

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  serv.attach(PIN5);
  Serial.begin(9600);
}

void loop()
{
//   // turn the LED on (HIGH is the voltage level)
//   digitalWrite(LED_BUILTIN, HIGH);
//   //Serial.println("led is high");

//   // wait for a second
//   delay(1000);

//   // turn the LED off by making the voltage LOW
//   digitalWrite(LED_BUILTIN, LOW);
//   Serial.println("led is low");
  
//    // wait for a second
//   delay(1000);

    if (handleInp == true) {
        digitalWrite(LED_BUILTIN, HIGH);
        if(newPos != pos){
          serv.write(newPos);
          pos = newPos;
          delay(300);
        }
        handleInp = false;
        delay(300);
        digitalWrite(LED_BUILTIN, LOW);

    }


}

String line = "";
void serialEvent()
{
  
   while(Serial.available()) 
   {
     
     char ch = Serial.read();
     if(ch == '\n' ){
        Serial.println("received this: " +line);
        newPos = line.toInt();
        handleInp = true;
        line="";
        
     }else if(ch != 13){
       line += ch;
     }

   }
}

