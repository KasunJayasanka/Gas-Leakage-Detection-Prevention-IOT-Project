#include <TinyGPS.h> //Library for NEO-6M-0-001 GPS Module

//GSM MODULE____________
SoftwareSerial mySerial (10,9);
int alertSentMessage=0;

// Create GPS Object
TinyGPS gps; 
//For GPS Coordinates
float lat,lon;
//For Location URL
String url=""; 

//Flame Sensor
const int flameSensorPin = 3;

//Boolean values about the Flame Sensor 
bool flameDetected = false;

//Flame Sensor
int flameSensor;

void setup() {

  //For Serial Communications
  Serial.begin(9600);

  //FOR GSM======
  mySerial.begin(9600);
  
  //GPS Module
  Serial2.begin(9600);

  //Flame sensor
  pinMode(flameSensorPin,INPUT);

}


void loop() {

  
  String url=getLocationURL();
  
  //Flame Sensor
  flameSensor = digitalRead(flameSensorPin);

  if(flameSensor==0)//check if flame is detected
  {
      
    //Send Message to User and Fire Authority
    while (alertSentMessage<10)
    {
      //message to fire authority
      SendMessage("+94711569794",FireAlertMessage(url));
      delay(3000);
     alertSentMessage++;
    }  

  }

  else
  {
      flameDetected = false;  
      alertSentMessage = 0;
  }


}

/***************************************************Custom Functions ************************************/

/*==========================Sending Message to Fire Authority=======================*/

//creates the message string to be sent to the fire authority using GPS location coordinates

String FireAlertMessage(String url)//creates the message string to be sent to the fire authority using GPS location coordinates
{
  String message;
  if(url!=""){
    message = "ALERT! Fire Detected."+ url;
  }
  else{
    String uRl="www.google.com/maps/search/?api=1&query=6.7989812%2C79.8942950";
    message = "ALERT! Fire Detected at the Location of Device 001"+uRl;
  }

  
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



