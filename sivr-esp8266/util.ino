void ledBlink(const int pin){
  analogWrite(pin, 0);
  delay(100);
  analogWrite(pin, 255);
  delay(100);
}

void ledOn(const int pin){
  analogWrite(pin, 0);
}

void ledOff(const int pin){
  analogWrite(pin, 255);
}

String networkSearch(){
  String strNetworksFound = "";
  int8_t networksCount = min((int)WiFi.scanNetworks(false, true), 16);

  char networkFound[128];
  for(int i = 0; i < networksCount; i++){
    snprintf(networkFound, 128, "%d: %s, Ch:%d (%ddBm) [%s]\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "Open" : "Secured");
    strNetworksFound.concat(networkFound);
  }

  Serial.println(strNetworksFound);
  return strNetworksFound;
}

void log(char* str){
  Serial.println(str);
}