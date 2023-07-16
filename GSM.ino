#include <SoftwareSerial.h>

//GSM MODULE____________
SoftwareSerial mySerial (10,9);

// Pin connected to the sensor input
const int gassensorPin = A0;

float lpGasThreshVal=500.0;
float lpGasReadingVal;


void setup() {
  // Set the sensor pin as an input
  pinMode(gassensorPin, INPUT);

  //FOR GSM======
  Serial.begin(9600);
  mySerial.begin(9600);

}

void loop() {
  lpGasReadingVal = analogRead(gassensorPin);

  if(lpGasReadingVal>=lpGasThreshVal){
      //Send message by gsm module
    SendMessage("+94775840180", "ALERT!...Gas Leakage Detected!");
  }
}

//==========================GSM Function=======================
void SendMessage(const char* recipient, const String message){
  mySerial.println("AT+CMGF=1");
  delay(2000);
  mySerial.print("AT+CMGS=\"");
  mySerial.print(recipient);  // Set the recipient phone number
  mySerial.println("\"");
  delay(1000);
  mySerial.println(message);
  delay(1000);

  mySerial.write(26);  // Send the Ctrl+Z character to indicate the end of the message
  delay(1000);
}
