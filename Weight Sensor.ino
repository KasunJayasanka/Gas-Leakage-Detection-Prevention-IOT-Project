#include <HX711.h> //Libaray for HX711 Load Cell Amplifier Module

//Count For Gas Weight Calculation
int count = 0;

//Load Cell (Load Cell)
HX711 cell;
int weightDTpin = 25;
int weightSCKpin = 23; 
float gasWeight=0.0;

void setup() {

  //For Serial Communications
  Serial.begin(9600);

  //Load Cell
  cell.begin(weightDTpin,weightSCKpin);

}


void loop() {

  
  //Getting Weight of the Gas Cylinder
  gasWeight = getGasCylinderWeight();


  //Sending Current Environment Data to Wifi Sheild
  makeSerialPayLoad(dateTime,gasLeakageDetected,flameDetected,temperatureValue,window1State,gasWeight);
  

}


/*==========================Getting LP Gas Weight using Load Cell=======================*/

float getGasCylinderWeight()
{
  float tareWeight=6.0;
  long val=0;
  float count = 0;
  count++;
  val=((count-1)/count)*val+(1/count)*cell.read(); //get average of fluctuations
  val=0.4*val+0.6*cell.read();
  float out = (((val-168032)/9968.0f*110)/1000.0f); //In Kilograms
  out=out-tareWeight;
  if(out<0) out = 0;
  
  return out;
}

