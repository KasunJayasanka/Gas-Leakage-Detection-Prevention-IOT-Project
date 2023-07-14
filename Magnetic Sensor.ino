//magnetic sensor and window alert buzzer
const int magSensorBuzzer = 5; 
const int magneticSensor = 6;

//Magnetic Sensor
int window1State;

void setup() {

  //For Serial Communications
  Serial.begin(9600);

  //magnetic sensor
  pinMode(magneticSensor,INPUT_PULLUP);
  pinMode(magSensorBuzzer,OUTPUT);

  
}


void loop() {

 
  //magnetic sensor
  window1State=digitalRead(magneticSensor);

  
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


  //Sending Current Environment Data to Wifi Sheild
  makeSerialPayLoad(dateTime,gasLeakageDetected,flameDetected,temperatureValue,window1State,gasWeight);
  
}

