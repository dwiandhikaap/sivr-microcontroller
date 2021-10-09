#include <ESP8266WiFi.h>

// Atur SSID & Password device ESP8266
const char* ssid = "sivr-device";
const char* password = "13371337";

// Ganti ke digital input kalo udah beli switch sama resistor 10k ohm
const int setupInputPin = A0;
bool setupMode = false;

// TODO: tambahin web server untuk setup mode nya

void setup(){
  Serial.begin(115200); 
  Serial.println("\n\n");
  
  /* 
    Karena pake potentiometer sebagai switchnya
  */
  setupMode = analogRead(A0) > 20;
  if(setupMode){
    pinMode(LED_BUILTIN, OUTPUT);
    analogWrite(LED_BUILTIN, 255);

    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.println("[sivr-device] - SETUP MODE");
    Serial.print("Soft-AP IP Address: ");
    Serial.println(IP);

    Serial.println(WiFi.localIP());
    return;
  }

  Serial.println("[sivr-device] - NORMAL MODE");

}

void loop(){
  if(setupMode){
    Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());  

    // Kalo ada perangkat yang terhubung, blink 2x
    if(WiFi.softAPgetStationNum() > 0){
      blink(LED_BUILTIN);
      blink(LED_BUILTIN);
    }

    delay(2000);
    return;
  }

  delay(200);
}

void blink(const int pin){
  analogWrite(pin, 0);
  delay(100);
  analogWrite(pin, 255);
  delay(100);
}