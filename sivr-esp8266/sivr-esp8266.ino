#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <EEPROM.h>

#define RESET_EEPROM false

// Ganti ke digital input kalo udah beli switch sama resistor 10k ohm
const int setupInputPin = A0;
bool isSetupModeEnabled = false;
  
ESP8266WebServer server(80);

void setup(){
  Serial.begin(115200); 
  EEPROM.begin(256);
  Serial.println("\n\n");

  Serial.println("[sivr-device] - Initializing...");
  connectToWifi();
}

void loop(){
  if(isSetupModeEnabled != analogRead(A0) > 20){
    isSetupModeEnabled = analogRead(A0) > 20;
    ledBlink(LED_BUILTIN);
    ledBlink(LED_BUILTIN);
    ledBlink(LED_BUILTIN);
    if(isSetupModeEnabled){
      enableSetupMode();
      return;
    }

    disableSetupMode();
  }

  if(isSetupModeEnabled){
    server.handleClient();  
  }

  delay(50);
}




























