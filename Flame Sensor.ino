#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

#include <SoftwareSerial.h> //Library for setup required Digital Pins for Serial Communications

//LCD Display
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); 

//Buzzer_____________
// Pin connected to the positive (long leg) of the buzzer
const int buzzerPin = 8;

//GSM MODULE____________
SoftwareSerial mySerial (10,9);
int alertSentMessage=0;

//Flame Sensor
const int flameSensorPin = 3;

//Boolean values about the Flame Sensor 
bool flameDetected = false;

//Flame Sensor
int flameSensor;

void setup() {

  //For Serial Communications
  Serial.begin(9600);

  //Initiating LCD
  lcd.init();
  lcd.backlight();

  //FOR GSM======
  mySerial.begin(9600);
  
  //FOR BUZZER======
  // Initialize the buzzer pin as an output
  pinMode(buzzerPin, OUTPUT);

  //Flame sensor
  pinMode(flameSensorPin,INPUT);

}


void loop() {

  
  //Sending Current Environment Data to Wifi Sheild
  makeSerialPayLoad(dateTime,gasLeakageDetected,flameDetected,temperatureValue,window1State,gasWeight);
  
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

    delay(6000);  

  }

  else
  {
      flameDetected = false;  
      alertSentMessage = 0;
      if(gasLeakageDetected==false) noTone(buzzerPin);
  }

  

  /**********************************END of The Loop***************************************/
  

}

