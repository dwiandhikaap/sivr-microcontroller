#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Ganti ke digital input kalo udah beli switch sama resistor 10k ohm
const int setupInputPin = A0;
bool isSetupModeEnabled = false;
  
ESP8266WebServer server(80);

void setup(){
  Serial.begin(115200); 
  Serial.println("\n\n");
  
  isSetupModeEnabled = analogRead(A0) > 20;
  if(setupMode){
    setupMode();
    return;
  }

  Serial.println("[sivr-device] - NORMAL MODE");
}

void loop(){
  if(isSetupModeEnabled != analogRead(A0) > 20){
    isSetupModeEnabled = analogRead(A0) > 20;
    Serial.println("[sivr-device] - SETUP MODE is toggled");
    ledBlink(LED_BUILTIN);
    ledBlink(LED_BUILTIN);
    ledBlink(LED_BUILTIN);
    if(isSetupModeEnabled){
      ledOn(LED_BUILTIN);
    }
  }

  if(isSetupModeEnabled){
    server.handleClient();  
  }

  delay(50);
}




























