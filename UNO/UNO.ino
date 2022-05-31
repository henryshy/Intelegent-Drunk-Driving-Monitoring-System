

#include <Wire.h>
#include <string.h>
#include <stdlib.h>
#include <avr/wdt.h>



int alpin = A0;    // select the input pin for the potentiometer
int alvalue = 0;  // variable to store the value coming from the sensor
char sendbuff[12];

int pulsePin = A1;                 // Pulse Sensor purple wire connected to analog pin 0
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
volatile int newbpm;
void setup() {

  Serial.begin(9600);  // start serial for output
  interruptSetup();

}
void loop() {
  alvalue=analogRead(alpin);
  char altmp[5]={
    '\0'  };
  char btmp[5]={
    '\0'  };
  if(QS==true){
    QS=false;
  }
  
  itoa(BPM,btmp,10);
  itoa(alvalue,altmp,10);

  for(int i=0;i<5;i++){
    sendbuff[1+i]=altmp[i];
    sendbuff[6+i]=btmp[i];
  }
  for(int i=0;i<11;i++){
    if(sendbuff[i]<'0'||sendbuff[i]>'9')
      sendbuff[i]='s';
  }
  sendbuff[0]='a';
  sendbuff[11]='\0';
  Serial.print(sendbuff);
  delay(20);
}


