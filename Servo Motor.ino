#include<Servo.h>  //Library for Servo Motor

//Variables for Gas Sensor Reading
float lpGasThreshVal=500.0;
float lpGasReadingVal;

// Pin connected to the MQ-6 Gas Sensor input
const int gasSensorPin = A0;

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

//Flame Sensor
int flameSensor;

void setup() {

  //For Serial Communications
  Serial.begin(9600);

  //Setup GAS Sensor Analog Input Pin
  pinMode(A0,INPUT);
  
  // Set the sensor pin as an input
  pinMode(gasSensorPin, INPUT);

  //Flame sensor
  pinMode(flameSensorPin,INPUT);

  //servo motor
  pinMode(buttonPin,INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);   // Configure the button pin as INPUT_PULLUP
  servo.attach(servoPin);              // Attach the servo to its pin
  servo.write(servoPosition);          // Initialize the servo position to 0 degrees

 
}


void loop() {

  buttonState = digitalRead(buttonPin);      // Read the state of the button

  lpGasReadingVal = analogRead(gasSensorPin);
  //Serial.println(lpGasReadingVal);

  if(lpGasReadingVal>=lpGasThreshVal){

  
  //Rotating Servo Motor   

    if (!gasLeakageDetected) 
    {
      if (servoPosition == 0) {
        servoPosition = 90;     // Set servo position to 90 degrees
      }
        gasLeakageDetected = true;
    }      
         
  }
  
  else{
    
    gasLeakageDetected = false;  //boolean value of gas leakage
   
  }

  //Flame Sensor
  flameSensor = digitalRead(flameSensorPin);

  if(flameSensor==0)//check if flame is detected
  {
      
    //Servo Motor Rotate if fire exists  
    if (!flameDetected) 
    {
      if (servoPosition == 0) {
        servoPosition = 90;     // Set servo position to 90 degrees
      }
        flameDetected = true;
    }      
    
  }

  else
  {
      flameDetected = false;  
     
  }

  
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

}

