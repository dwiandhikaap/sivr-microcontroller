// Atur SSID & Password device ESP8266
const char* ssid = "sivr-device";
const char* password = "13371337";

void enableSetupMode(){
  pinMode(LED_BUILTIN, OUTPUT);
  analogWrite(LED_BUILTIN, 255);

  WiFi.softAP(ssid, password);
  delay(100);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.println("[SETUP MODE] - Setup Mode is now enabled!");
  Serial.print("[SETUP MODE] - Setup Page IP Address: ");
  Serial.println(IP);

  serverInit();
  ledOn(LED_BUILTIN);
  return;
}

void disableSetupMode(){
  Serial.println("[SETUP MODE] - Setup Mode is now disabled!");

  WiFi.softAPdisconnect();
  serverStop();
  ledOff(LED_BUILTIN);
}