
#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>
#include <stdbool.h>
#include <stdint.h>
#include "Melody_LMT84.h"


#define red RED_LED
#define yellow YELLOW_LED
char ssid[] = "421LAB";
char password[] = "1234567890";
char recv;
WiFiClient client;
long previousMillis = 0;  
long interval = 100; 
IPAddress server(192,168,1,115); 
//char server[] = "www.henryshy.com";
char buff[2];
void setup() {
  MelodyLMT84.begin();
  //Initialize serial and wait for port to open:
  Serial.begin(115200);

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");

  while (WiFi.localIP() == INADDR_NONE) {
    Serial.print(".");
    delay(300);
  }

  

  pinMode(red, OUTPUT);    
  pinMode(yellow, OUTPUT);    
  Serial.println("\nIP Address obtained");
  client.connect(server, 7777);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
}
float temperature = 0.0;
void loop() {
  int i=0;

   if (!client.connected()) {
    client.connect(server, 7777);
  }
  else {
    while (client.available()) {
      buff[i++] = client.read();
    }
    Serial.println(buff);
    if(buff[0]=='0'){
      digitalWrite(yellow, HIGH);
    }
    else if(buff[0]=='1'){
      digitalWrite(yellow, LOW);
    }
    
    if(buff[1]=='9'){
      digitalWrite(red, HIGH);
    }
    else if(buff[1]=='8'){
      digitalWrite(red, LOW);
    }
     temperature =100* MelodyLMT84.getTemperature();
     Serial.println(temperature);
    client.print(temperature);
  }
  delay(100);
}
