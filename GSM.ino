#include <SoftwareSerial.h> //Library for setup required Digital Pins for Serial Communications

//Variables for Gas Sensor Reading
float lpGasThreshVal=500.0;
float lpGasReadingVal;

// Pin connected to the MQ-6 Gas Sensor input
const int gasSensorPin = A0;

//GSM MODULE____________
SoftwareSerial mySerial (10,9);
int alertSentMessage=0;

//Flame Sensor
const int flameSensorPin = 3;

//Boolean values about the Flame Sensor and Gas Sensor
bool gasLeakageDetected = false;
bool flameDetected = false;

//Flame Sensor
int flameSensor;

void setup() {

  //For Serial Communications
  Serial.begin(9600);

  //Setup GAS Sensor Analog Input Pin
  pinMode(A0,INPUT);
  
  //FOR GSM======
  mySerial.begin(9600);
  
  // Set the sensor pin as an input
  pinMode(gasSensorPin, INPUT);

  //Flame sensor
  pinMode(flameSensorPin,INPUT);


}


void loop() {

  
  lpGasReadingVal = analogRead(gasSensorPin);
  //Serial.println(lpGasReadingVal);

 
  if(lpGasReadingVal>=lpGasThreshVal){
  
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

    gasLeakageDetected = false;  //boolean value of gas leakage
    alertSentMessage = 0;    //boolean value indicating whether a message has been already sent
  }

  //Flame Sensor
  flameSensor = digitalRead(flameSensorPin);

  if(flameSensor==0)//check if flame is detected
  {
      
   
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
  }


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
