//Variables for Gas Sensor Reading
float lpGasThreshVal=500.0;
float lpGasReadingVal;


// Pin connected to the MQ-6 Gas Sensor input
const int gasSensorPin = A0;

//Boolean values about the Flame Sensor and Gas Sensor
bool gasLeakageDetected = false;
bool flameDetected = false;

void setup() {

  //For Serial Communications
  Serial.begin(9600);

  // Set the sensor pin as an input
  pinMode(gasSensorPin, INPUT);

  //Setting up pin for Relay Module
  pinMode(2,OUTPUT);


}

void loop() {

  
  lpGasReadingVal = analogRead(gasSensorPin);

  if(lpGasReadingVal>=lpGasThreshVal){
  
    //Turn off the Electricity and Turn on the Exhaust Fan
    digitalWrite(2,HIGH); 
    delay(6000);            
  }
  
  else{
 
    gasLeakageDetected = false;  //boolean value of gas leakage
   
    //Turn on the Electricity and Turn off Exhaust Fan
    digitalWrite(2,LOW);

    delay(6000) ;      

  }

   //Flame Sensor
  flameSensor = digitalRead(flameSensorPin);

  if(flameSensor==0)//check if flame is detected
  {
    //Turn off the Electricity and Turn on the Exhaust Fan
    digitalWrite(2,HIGH); 

    delay(6000) ;    
  }

  else
  {
      flameDetected = false;  
  }

}

