#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

// Libraries for DS18B20 Temperature Sensor 
#include <OneWire.h>
#include <DallasTemperature.h>

//To Temperature Sensor
#define ONE_WIRE_BUS 28

OneWire oneWire(ONE_WIRE_BUS);
// Setup a oneWire instance to communicate with any OneWire devices
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

//LCD Display
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); 


//Temperature Sensor
float temperatureValue;


void setup() {

  //For Serial Communications
  Serial.begin(9600);

  //Initiating LCD
  lcd.init();
  lcd.backlight();

  //For Temperature Sensor
  sensors.begin();

}


void loop() {

  
  temperatureValue=sensors.getTempCByIndex(0);

  //Sending Current Environment Data to Wifi Sheild
  makeSerialPayLoad(dateTime,gasLeakageDetected,flameDetected,temperatureValue,window1State,gasWeight);
  
  if(lpGasReadingVal>=lpGasThreshVal){
  

  //Display Temperature on LCD Display
    sensors.requestTemperatures(); 
    lcd.setCursor(0, 1);
    lcd.print("Temp ");
    temperatureValue=sensors.getTempCByIndex(0);
    lcd.print(temperatureValue);
    lcd.print("\xDF""C");
  //delay(1000);

         
  }
  
  else{


    //Display Temperature Value on LCD Display
    sensors.requestTemperatures(); 
    lcd.setCursor(0, 1);
    lcd.print("Temp ");
    temperatureValue=sensors.getTempCByIndex(0);
    lcd.print(temperatureValue);
    lcd.print("\xDF""C");
    delay(1000);

  }


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
      
  }

}


