#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

#include <SoftwareSerial.h> //Library for setup required Digital Pins for Serial Communications

//LCD Display
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); 

float lpGasThreshVal=500.0;
float lpGasReadingVal;

//Buzzer_____________
// Pin connected to the positive (long leg) of the buzzer
const int buzzerPin = 8;


// Pin connected to the MQ-6 Gas Sensor input
const int gasSensorPin = A0;

//GSM MODULE____________
SoftwareSerial mySerial (10,9);
int alertSentMessage=0;

//Boolean values about the Flame Sensor and Gas Sensor
bool gasLeakageDetected = false;


void setup() {

  //For Serial Communications
  Serial.begin(9600);

  //Initiating LCD
  lcd.init();
  lcd.backlight();

  //Setup GAS Sensor Analog Input Pin
  pinMode(A0,INPUT);

  //FOR BUZZER======
  // Initialize the buzzer pin as an output
  pinMode(buzzerPin, OUTPUT);

  // Set the Gas Sensor pin as an input
  pinMode(gasSensorPin, INPUT);

  //LED Blink system
  pinMode(11, OUTPUT); 
   
  //disconect current with relay module
  pinMode(2,OUTPUT);

  //FOR GSM======
  mySerial.begin(9600);
}


void loop() {

  
  lpGasReadingVal = analogRead(A0);
  
  if(lpGasReadingVal>=lpGasThreshVal){

  //display gas leakage on LCD
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Gas Leakage!"); 

  //LED Blink system ON
    digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level) 

  //Turn off the Electricity and Turn on the Exhaust Fan

    digitalWrite(2,HIGH); 
    delay(1000);

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
  
    delay(1000);             
  }
  
  else{

    //Display Gas Leakage Status on LCD Display
    lcd.setCursor(0, 0); 
    lcd.print("No Gas Leakage!");

    gasLeakageDetected = false;  //boolean value of gas leakage
    alertSentMessage = 0;    //boolean value indicating whether a message has been already sent

    //Turn on the Electricity and Turn off Exhaust Fan
    digitalWrite(2,LOW);
    delay(1000) ;      

    //turn off led
    digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW 
  }

  /**********************************END of The Loop***************************************/

}

