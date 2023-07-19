#include <ArduinoJson.h> //Library for Generating Json Objects
#include <RTClib.h> //Library for DS3231 RTC Module
#include <Wire.h>

//RTC Module
RTC_DS3231 rtc;
StaticJsonDocument<128> dateTimeDoc;

void setup() {

  //For Serial Communications
  Serial.begin(9600);

  //RTC Module
  Wire.begin();
  rtc.begin();

  if (rtc.lostPower())
  {
    rtc.adjust(DateTime(2023, 7, 13, 12, 0, 0));
    
  }

  //Adjusting Current Date Time of RTC
  rtc.adjust(DateTime(2023, 7, 13, 12, 0, 0));

}


void loop() {

  String dateTimeString=getDateTime();

  deserializeJson(dateTimeDoc,dateTimeString);
  
  String date=dateTimeDoc["date"];
  String time=dateTimeDoc["time"];
  String dateTime=date+" "+time;
  
  //Sending Current Environment Data to Wifi Sheild
  makeSerialPayLoad(dateTime,gasLeakageDetected,flameDetected,temperatureValue,window1State,gasWeight);
  
}


/*==========================Getting Date & Time using RTC Module=======================*/

String getDateTime(){

  String date,time,dateTimeString;
  StaticJsonDocument<128> dateTimeDoc;

  DateTime now = rtc.now();

  date=String(now.year())+"-"+String(now.month())+"-"+String(now.day());
  time=String(now.hour())+":"+String(now.minute())+":"+String(now.second());

  dateTimeDoc["date"]=date;
  dateTimeDoc["time"]=time;

  serializeJson(dateTimeDoc,dateTimeString);

  return dateTimeString;
  delay(1000);

}

