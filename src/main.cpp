#include <Arduino.h>
#include<BluetoothSerial.h>
#include <DFRobot_MAX30102.h>
#include <Wire.h>
#include "MAX30105.h" 
#define errorpin_pushbutton 15
BluetoothSerial SerialBT;//define Bluetooth

//define Function
void spo2mon();
void tempmon();
void  error();


//define varibale
int32_t SPO2; //SPO2
int8_t SPO2Valid; //Flag to display if SPO2 calculation is valid
int32_t heartRate; //Heart-rate
int8_t heartRateValid; //Flag to display if heart-rate calculation is valid
float temperature;//temperature in celecoc
float avgtemperature;//avarge temperature
float sumtemperature;//sum of the temperature
int sumheartrate;//sum of the heartrate
int sumSPO2;//sum of the SPO2
int avgheartrate;//avarge heartrate
int avgSPO2;//avarage Spo2
int dive;//number that i acn take the avarge from it
bool help=0;
void setup() {
  // put your setup code here, to run once:
 //Init serial
  Serial.begin(115200);
  SerialBT.begin("OLder_monitor");
  pinMode(errorpin_pushbutton,INPUT);
}

void loop() {
  
  // put your main code here, to run repeatedly:

  Serial.println(digitalRead(errorpin_pushbutton));
  delay(1000);
  while (digitalRead(errorpin_pushbutton)==LOW )
  {
    delay(1000);
    Serial.println(digitalRead(errorpin_pushbutton));
    SerialBT.print("HELP");
  SerialBT.print(";");
  SerialBT.print("HELP");
  SerialBT.print(";");
  SerialBT.print("HELP");
  SerialBT.println(";");
  Serial.print("HELP");
  }
 
  SerialBT.print(avgSPO2);
  SerialBT.print(";");
  SerialBT.print(avgheartrate);
  SerialBT.print(";");
  SerialBT.print(avgtemperature);
  SerialBT.println(";");
  spo2mon();
  tempmon();
   delay(200);
  

  delay(200);
}

// put function definitions here:
void spo2mon(){

    DFRobot_MAX30102 particleSensor;
   while (!particleSensor.begin()) {
    Serial.println("MAX30102 was not found");
    delay(1000);}
  particleSensor.sensorConfiguration(/*ledBrightness=*/50, /*sampleAverage=*/SAMPLEAVG_4, \
                        /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_100, \
                        /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);
   Serial.println();
   Serial.println("Wait about four seconds");
  //Print result
  int i=0;
  sumSPO2=0;
  sumheartrate=0;
  dive=0;
  while ( i<5)
  {
   particleSensor.heartrateAndOxygenSaturation(/**SPO2=*/&SPO2, /**SPO2Valid=*/&SPO2Valid, /**heartRate=*/&heartRate, /**heartRateValid=*/&heartRateValid);
  
  /*Serial.print("heartRate=");
  Serial.print(heartRate, DEC);
  Serial.print(", heartRateValid=");
  Serial.print(heartRateValid, DEC);
  Serial.print("; SPO2=");
  Serial.print(SPO2, DEC);
  Serial.print(", SPO2Valid=");
  Serial.println(SPO2Valid, DEC);*/
  i++;
if(SPO2!=-999&heartRate!=-999){
  sumSPO2=SPO2+sumSPO2;
    Serial.print(", sumSPO2=");
  Serial.println(sumSPO2, DEC);
  sumheartrate=heartRate+sumheartrate;
Serial.print("sumheartrate=");
  Serial.println(sumheartrate, DEC);
  dive++;
  }
    }
  avgheartrate=sumheartrate/dive;
  Serial.print("heartRate=");
  Serial.print(avgheartrate, DEC);
  avgSPO2=sumSPO2/dive;
  Serial.print(", sumSPO2=");
  Serial.println(avgSPO2, DEC);
  } 

void tempmon(){

MAX30105 particleSensor;
  if (particleSensor.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
   particleSensor.enableDIETEMPRDY(); 
   
  temperature = particleSensor.readTemperature();
int count=0;
sumtemperature=0;
  //Print result
  while (count<=50)
  {
    temperature = particleSensor.readTemperature();
    sumtemperature=temperature+sumtemperature;
     Serial.print("sumtemperature=");
  Serial.println(sumtemperature, DEC );
  count++;
 
  }
  
avgtemperature=(sumtemperature/50.0)+2;
     Serial.print("temperature=");
  Serial.println(avgtemperature, DEC );
  delay(200);


} 
 
void IRAM_ATTR error(){
  
}
