#include <LiquidCrystal_I2C.h> // Library for LCD

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

// Pin connected to the MQ-6 Gas Sensor input
const int gasSensorPin = A0;

//Flame Sensor
const int flameSensorPin = 3;


//Boolean values about the Flame Sensor and Gas Sensor
bool gasLeakageDetected = false;
bool flameDetected = false;

//Flame Sensor
int flameSensor;

//Temperature Sensor
float temperatureValue;

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

  // Set the sensor pin as an input
  pinMode(gasSensorPin, INPUT);

  //Flame sensor
  pinMode(flameSensorPin,INPUT);

  
}


void loop() {

  lpGasReadingVal = analogRead(gasSensorPin);
  //Serial.println(lpGasReadingVal);

  //Read the Temperature Value
  temperatureValue=sensors.getTempCByIndex(0);

  if(lpGasReadingVal>=lpGasThreshVal){
  
  //Display Gas Leakage on LCD
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Gas Leakage!"); 

  
  //Display Temperature on LCD Display
    sensors.requestTemperatures(); 
    lcd.setCursor(0, 1);
    lcd.print("Temp ");
    temperatureValue=sensors.getTempCByIndex(0);
    lcd.print(temperatureValue);
    lcd.print("\xDF""C");
 
 
  }
  
  else{

    //Display Gas Leakage Status on LCD Display
    lcd.setCursor(0, 0); 
    lcd.print("No Gas Leakage!");
   
    gasLeakageDetected = false;  //boolean value of gas leakage
    
    //Display Temperature Value on LCD Display
    sensors.requestTemperatures(); 
    lcd.setCursor(0, 1);
    lcd.print("Temp ");
    temperatureValue=sensors.getTempCByIndex(0);
    lcd.print(temperatureValue);
    lcd.print("\xDF""C");
    delay(1000);

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

   
  }

  else
  {
      flameDetected = false;  
    
  }

}
