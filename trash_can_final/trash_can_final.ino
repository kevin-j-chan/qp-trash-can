/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  WARNING!
    It's very tricky to get it working. Please read this article:
    http://help.blynk.cc/hardware-and-libraries/arduino/esp8266-with-at-firmware

  You’ll need:
   - Blynk App (download from AppStore or Google Play)
   - Arduino Uno board
   - Decide how to connect to Blynk
     (USB, Ethernet, Wi-Fi, Bluetooth, ...)

  There is a bunch of great example sketches included to show you how to get
  started. Think of them as LEGO bricks  and combine them as you wish.
  For example, take the Ethernet Shield sketch and combine it with the
  Servo example, or choose a USB sketch and add a code from SendData
  example.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include "Ultrasonic.h"

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "ae7bc78c4f3a40a6a2ea43232c795535";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "KeVIN";
char pass[] = "kevinchan";

String AP = "KeVIN";
String PASS = "kevinchan";

// Hardware Serial on Mega, Leonardo, Micro...
//#define EspSerial Serial1

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(10, 11); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);


// ULTRASONIC RANGER
Ultrasonic ultrasonic(7);

int runRanger(){
  long RangeInInches;
  long RangeInCentimeters;
  RangeInInches = ultrasonic.MeasureInInches();
  RangeInCentimeters = RangeInInches * 2.54;/*ultrasonic.MeasureInCentimeters();*/
  Serial.println("The distance to obstacles in front is: ");
  Serial.print(RangeInInches);//0~157 inches
  Serial.println(" inch");
  Serial.print(RangeInCentimeters);//0~400cm
  Serial.println(" cm");
  return RangeInInches;
}

// TIMERS
BlynkTimer timer;

void sendUpRange(){
  Blynk.virtualWrite(V4, runRanger());
}

// SEND COMMAND 
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    EspSerial.println(command);//at+cipsend
    if(EspSerial.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  //sendCommand("AT+CWMODE=1",5,"OK");
  //sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");

  Serial.println("Starting Blynk.begin()");
  Blynk.begin(auth, wifi, ssid, pass);
  Serial.println("Finished Blynk.begin()");
  Blynk.virtualWrite(V5, "Finished!");
  Blynk.virtualWrite(V4, runRanger());
  //sendUpRange();  
  delay(2000);
  Blynk.virtualWrite(V5, "-------");
  timer.setInterval(1000L, sendUpRange);
}

void loop()
{
  Blynk.run();
  timer.run();
}

