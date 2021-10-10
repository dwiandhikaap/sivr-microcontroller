// Atur SSID & Password device ESP8266
const char* ssid = "sivr-device";
const char* password = "13371337";

void setupMode(){
  pinMode(LED_BUILTIN, OUTPUT);
  analogWrite(LED_BUILTIN, 255);

  WiFi.softAP(ssid, password);
  delay(100);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.println("[sivr-device] - SETUP MODE");
  Serial.print("Soft-AP IP Address: ");
  Serial.println(IP);

  Serial.println(WiFi.localIP());

  serverInit();
  ledOn(LED_BUILTIN);
  return;
}

