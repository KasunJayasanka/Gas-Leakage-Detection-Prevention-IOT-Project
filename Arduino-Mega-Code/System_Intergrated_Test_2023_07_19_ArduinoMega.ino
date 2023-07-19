#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

// Libraries for DS18B20 Temperature Sensor 
#include <OneWire.h>
#include <DallasTemperature.h>

#include <SoftwareSerial.h> //Library for setup required Digital Pins for Serial Communications
#include<Servo.h>  //Library for Servo Motor
#include <TinyGPS.h> //Library for NEO-6M-0-001 GPS Module

#include <RTClib.h> //Library for DS3231 RTC Module

#include <HX711.h> //Libaray for HX711 Load Cell Amplifier Module

#include <ArduinoJson.h> //Library for Generating Json Objects

//To Temperature Sensor
#define ONE_WIRE_BUS 28

OneWire oneWire(ONE_WIRE_BUS);
// Setup a oneWire instance to communicate with any OneWire devices
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

//LCD Display
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); 

//Variables for Gas Sensor Reading
float lpGasThreshVal=500.0;
float lpGasReadingVal;

//Count For Gas Weight Calculation
int count = 0;

//Buzzer_____________
// Pin connected to the positive (long leg) of the buzzer
const int buzzerPin = 8;

// Pin connected to the MQ-6 Gas Sensor input
const int gasSensorPin = A0;

//GSM MODULE____________
SoftwareSerial mySerial (10,9);
int alertSentMessage=0;

//Flame Sensor
const int flameSensorPin = 3;

//servo motor
Servo servo;
int servoPosition = 0;
int buttonState = 0;
int prevButtonState = 0;
const int servoPin = 7 ; //The pin connected to the servo motor

//Boolean values about the Flame Sensor and Gas Sensor
bool gasLeakageDetected = false;
bool flameDetected = false;

//push button
const int buttonPin = 4 ; //The pin connected to the push button

//magnetic sensor and window alert buzzer
const int magSensorBuzzer = 5; 
const int magneticSensor = 6;

//RTC Module
RTC_DS3231 rtc;
// char t[32];
// String date,time,dateTime;
StaticJsonDocument<128> dateTimeDoc;

//Magnetic Sensor
int window1State;

//Flame Sensor
int flameSensor;

//Temperature Sensor
float temperatureValue;

//Load Cell (Load Cell)
HX711 cell;
int weightDTpin = 25;
int weightSCKpin = 23; 
float gasWeight=0.0;

// Create GPS Object
TinyGPS gps; 
//For GPS Coordinates
float lat,lon;
//For Location URL
String url=""; 

void setup() {

  //For Serial Communications
  Serial.begin(9600);

  //Initiating LCD
  lcd.init();
  lcd.backlight();

  //Setup GAS Sensor Analog Input Pin
  pinMode(A0,INPUT);
  
  //For Temperature Sensor
  sensors.begin();

  //FOR GSM======
  mySerial.begin(9600);
  
  //FOR BUZZER======
  // Initialize the buzzer pin as an output
  pinMode(buzzerPin, OUTPUT);

  // Set the sensor pin as an input
  pinMode(gasSensorPin, INPUT);

  //LED Blink system
  pinMode(11, OUTPUT); 
   
  //Setting up pin for Relay Module
  pinMode(2,OUTPUT);

  //Flame sensor
  pinMode(flameSensorPin,INPUT);

  //servo motor

  pinMode(buttonPin,INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);   // Configure the button pin as INPUT_PULLUP
  servo.attach(servoPin);              // Attach the servo to its pin
  servo.write(servoPosition);          // Initialize the servo position to 0 degrees

  //magnetic sensor
  pinMode(magneticSensor,INPUT_PULLUP);
  pinMode(magSensorBuzzer,OUTPUT);

  //GPS Module
  Serial2.begin(9600);

  //RTC Module
  Wire.begin();
  rtc.begin();

   if (! rtc.begin()) 
  {
    // Serial.println(" RTC Module not Present");
    //while (1);
    lcd.setCursor(0,0);
    lcd.print("No RTC Initialized");
    lcd.clear();
  }

  if (rtc.lostPower())
  {
    lcd.setCursor(0,0);  
    lcd.print("RTC LOST Power");
    lcd.clear();
    //Serial.println("RTC power failure, reset the time!");
    rtc.adjust(DateTime(2023, 7, 19, 14, 30, 0));
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  //Adjusting Current Date Time of RTC
  rtc.adjust(DateTime(2023, 7, 19, 14, 20, 0));

  //Load Cell
  cell.begin(weightDTpin,weightSCKpin);

}


void loop() {

  buttonState = digitalRead(buttonPin);      // Read the state of the button

  String dateTimeString=getDateTime();

  deserializeJson(dateTimeDoc,dateTimeString);
  
  String date=dateTimeDoc["date"];
  String time=dateTimeDoc["time"];
  String dateTime=date+" "+time;
  // Serial.println("Date:");
  // Serial.println(date);
  // Serial.println(time);

  String url=getLocationURL();

  lpGasReadingVal = analogRead(gasSensorPin);
  //Serial.println(lpGasReadingVal);

  //magnetic sensor
  window1State=digitalRead(magneticSensor);

  // Serial.print("Window State = ");
  // Serial.println(window1State);

  //Getting Weight of the Gas Cylinder
  gasWeight = getGasCylinderWeight();


  //Check the status of Window
  if(window1State==LOW)
  {
    tone(magSensorBuzzer,700,1000);
    delay(1000);
  }
  else
  {
    noTone(magSensorBuzzer);
  }


  //Read the Temperature Value
  temperatureValue=sensors.getTempCByIndex(0);

  //Sending Current Environment Data to Wifi Sheild
  makeSerialPayLoad(dateTime,gasLeakageDetected,flameDetected,temperatureValue,window1State,gasWeight);
  
  if(lpGasReadingVal>=lpGasThreshVal){
  //display gas leak

    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Gas Leakage!"); 

  //LED Blink system ON
    digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level) 

  
  //Rotating Servo Motor   

    if (!gasLeakageDetected) 
    {
      if (servoPosition == 0) {
        servoPosition = 90;     // Set servo position to 90 degrees
      }
        gasLeakageDetected = true;
    }      
    delay(1000);


  //Sending Current Environment Data to Wifi Sheild
  makeSerialPayLoad(dateTime,gasLeakageDetected,flameDetected,temperatureValue,window1State,gasWeight);
  
  //Display Temperature on LCD Display
    sensors.requestTemperatures(); 
    lcd.setCursor(0, 1);
    lcd.print("Temp ");
    temperatureValue=sensors.getTempCByIndex(0);
    lcd.print(temperatureValue);
    lcd.print("\xDF""C");
  //delay(1000);

  //Ringing Buzzer to alert about Gas Leakage
    tone(buzzerPin, 1000, 1000);
    delay(500);
    tone(buzzerPin, 700, 2000);
    delay(500);
    tone(buzzerPin, 1000, 1000);
    delay(500);
    tone(buzzerPin, 700);

  //Send Message to User

  if (alertSentMessage<10)
  {
  //Send message by gsm module
    SendMessage("+94775840180", "ALERT!...Gas Leakage Detected!");
    alertSentMessage++;
  }
  
    //Turn off the Electricity and Turn on the Exhaust Fan
    digitalWrite(2,HIGH); 
    delay(6000);            
  }
  
  else{

    //Display Gas Leakage Status on LCD Display
    lcd.setCursor(0, 0); 
    lcd.print("No Gas Leakage!");

    //Turn off Buzzer which ring when Gas Leakage
    if(flameDetected==false) noTone(buzzerPin);

    gasLeakageDetected = false;  //boolean value of gas leakage
    alertSentMessage = 0;    //boolean value indicating whether a message has been already sent

    //Display Temperature Value on LCD Display
    sensors.requestTemperatures(); 
    lcd.setCursor(0, 1);
    lcd.print("Temp ");
    temperatureValue=sensors.getTempCByIndex(0);
    lcd.print(temperatureValue);
    lcd.print("\xDF""C");
    delay(1000);

    //Turn on the Electricity and Turn off Exhaust Fan
    digitalWrite(2,LOW);
    delay(1000) ;      

    //turn off led
    digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW 
  }

  //Flame Sensor
  flameSensor = digitalRead(flameSensorPin);

  if(flameSensor==0)//check if flame is detected
  {
      
    //Display Fire Status & Temperature Value on LCD
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Fire Detected!");
    sensors.requestTemperatures(); 
    lcd.setCursor(0, 1);
    lcd.print("Temp ");
    temperatureValue=sensors.getTempCByIndex(0);
    lcd.print(temperatureValue);
    // lcd.print("º");
    lcd.print("\xDF""C");

    //Servo Motor Rotate if fire exists  
    if (!flameDetected) 
    {
      if (servoPosition == 0) {
        servoPosition = 90;     // Set servo position to 90 degrees
      }
        flameDetected = true;
    }      
    delay(1000);

    //Sending Current Environment Data to Wifi Sheild
    makeSerialPayLoad(dateTime,gasLeakageDetected,flameDetected,temperatureValue,window1State,gasWeight);

    //ring buzzer 
    tone(buzzerPin,1200);

    //Send Message to User and Fire Authority
    if (alertSentMessage<10)
    {
      //message to owner 
      SendMessage("+94775840180",("ALERT!...Fire Detected!"));
      // if (mySerial.available()>0)
      // Serial.write(mySerial.read());
      delay(2000);
      
      //message to fire authority
      SendMessage("+94711569794",FireAlertMessage(url));
      delay(3000);
     alertSentMessage++;
    }

    //Turn off the Electricity and Turn on the Exhaust Fan
    digitalWrite(2,HIGH); 

    delay(6000);  

  }

  else
  {

      //Turn on the Electricity and Turn on the Exhaust Fan
      digitalWrite(2,LOW); 

      flameDetected = false;  
      alertSentMessage = 0;
      if(gasLeakageDetected==false) noTone(buzzerPin);
      
  }

  /************************************************************************************************************/
  
  //Manually Control the Servo Motor Using Push Button
  if (buttonState != prevButtonState) {
    if (buttonState == LOW) {    // If the button is pressed (LOW state)
      if (servoPosition == 0) {
        servoPosition = 90;     // Set servo position to 90 degrees
      }
      else {
        servoPosition = 0;      // Set servo position to 0 degrees
      }
    }
  }

  servo.write(servoPosition);   // Move the servo to the desired position
  prevButtonState = buttonState;   // Save the button state for the next iteration

  /**********************************END of The Loop***************************************/
  

}

/***************************************************Custom Functions ************************************/

/*==========================Functions to Send Message Through GSM Module=======================*/

/*==========================Sending Message to User=======================*/
void SendMessage(const char* recipient, const String message){

  mySerial.println("AT+CMGF=1");
  delay(2000);
  mySerial.print("AT+CMGS=\"");
  mySerial.print(recipient);  // Set the recipient phone number
  mySerial.println("\"");
  delay(1000);
  mySerial.print(message);
  delay(1000);

  mySerial.write(26);  // Send the Ctrl+Z character to indicate the end of the message
  delay(1000);
}

/*==========================Sending Message to Fire Authority=======================*/

//creates the message string to be sent to the fire authority using GPS location coordinates

String FireAlertMessage(String url)//creates the message string to be sent to the fire authority using GPS location coordinates
{
  String message;
  if(url!=""){
    message = "ALERT! Fire Detected."+ url;
  }
  else{
    String uRl="www.google.com/maps/search/?api=1&query=6.797174%2C79.902046";
    message = "ALERT! Fire Detected at the Location of Device 001 \n"+uRl;
  }
  // Serial.print("\n");
  //Serial.println(message);
  
  return message;
}

/*==========================Getting GPS Coordinates and generate URL=======================*/


String getLocationURL(){
  String url="";
  
  for (unsigned long start = millis(); millis() - start < 1000;){
    while(Serial2.available()){ // check for gps data
    
    if(gps.encode(Serial2.read()))// encode gps data
    { 
    gps.f_get_position(&lat,&lon); // get latitude and longitude

    url = "\nLocation: www.google.com/maps/search/?api=1&query="+String(lat, 7)+"%2C"+String(lon,7);
    }

  }

  }
  return url;
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
  
  // sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());  
  // Serial.print(F("Date/Time: "));
  // Serial.println(t);

  return dateTimeString;

  delay(1000);

}

/*==========================Getting LP Gas Weight using Load Cell=======================*/

float getGasCylinderWeight()
{
  float tareWeight=9.2;
  long val=0;
  float count = 0;
  count++;
  val=((count-1)/count)*val+(1/count)*cell.read(); //get average of fluctuations
  val=0.4*val+0.6*cell.read();
  float out = (((val-168032)/9968.0f*110)/1000.0f); //In Kilograms
  out=out-tareWeight;
  if(out<0) out = 0;
  
  return out;
}

/*==========================Serial Communicating to WiFi Sheild=======================*/

void makeSerialPayLoad(String dateTime,bool gasLeakageDetected,bool flameDetected,float temperatureValue,int window1State,float gasWeight){

    String serialPayload;

    // We build the serial payload with & separation

    serialPayload = "*"
    +dateTime  // 0th index you will have dateTime value
    +"&"
    + String(gasLeakageDetected) 
    + "&" 
    + String(flameDetected) 
    + "&"
    + String(temperatureValue)
    + "&"
    + String(window1State)
    + "&"
    + String(gasWeight)
    + "$";

    Serial.print(serialPayload);

    delay(2000);
}
