#include<Servo.h>  //Library for Servo Motor

//Variables for capturing GAS Values
float lpGasThreshVal=500.0;
float lpGasReadingVal;


//servo motor
Servo servo;
int servoPosition = 0;
int buttonState = 0;
int prevButtonState = 0;
const int servoPin = 7 ; //The pin connected to the servo motor

//push button
const int buttonPin = 4 ; //The pin connected to the push button

//Boolean values about the Flame Sensor and Gas Sensor
bool gasLeakageDetected = false;

void setup() {

  //Setup GAS Sensor Analog Input Pin
  pinMode(A0,INPUT);

  //setup servo motor
  pinMode(buttonPin, INPUT_PULLUP);   // Configure the button pin as INPUT_PULLUP for push button
  servo.attach(servoPin);              // Attach the servo to its pin
  servo.write(servoPosition);          // Initialize the servo position to 0 degrees

}

void loop() {

  //Serial.println(lpGasReadingVal);
  lpGasReadingVal = analogRead(A0);

  buttonState = digitalRead(buttonPin);      // Read the state of the button

  if(lpGasReadingVal>=lpGasThreshVal){
    //Rotating Servo Motor   

    if (!gasLeakageDetected) 
    {
      if (servoPosition == 0) {
        servoPosition = 90;     // Set servo position to 90 degrees
      }
        gasLeakageDetected = true;
    }      
    delay(1000);

  }

  else{
        gasLeakageDetected = false;  //boolean value of gas leakage
  }

  ///////////////////////////////////////////////////////////////////////////
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
