String networkSearch(){
  DynamicJsonDocument doc(1024);

  String strNetworksFound = "";
  int8_t networksCount = min((int)WiFi.scanNetworks(false, true), 16);

  char networkFound[128];
  for(int i = 0; i < networksCount; i++){
    //snprintf(networkFound, 128, "%d: %s, Ch:%d (%ddBm) [%s]\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.encryptionType(i) == ENC_TYPE_NONE ? "Open" : "Secured");
    //strNetworksFound.concat(networkFound);
    doc[i]["ssid"] = WiFi.SSID(i);
    doc[i]["strength"] = WiFi.RSSI(i);
    doc[i]["secure"] = WiFi.encryptionType(i) == ENC_TYPE_NONE ? "Open" : "Secured";
  }

  serializeJson(doc, strNetworksFound);
  //Serial.println(strNetworksFound);
  return strNetworksFound;
}

void log(char* str){
  Serial.println(str);
}

int connectToWifi(){
  const String ssid = readSSID();
  const String password = readPassword();

  ledOff(LED_BUILTIN);
  WiFi.begin(ssid, password);

  Serial.print("[WiFi] Connecting");
  int attempts = 0;
  while(!(WiFi.status() == WL_CONNECTED || attempts == 15)){
    delay(1000);
    ledBlink(LED_BUILTIN);
    attempts++;
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("[WiFi] Status: ");
  Serial.println(WiFi.status());
  
  switch(WiFi.status()){
    case WL_WRONG_PASSWORD : {      
      Serial.println("[WiFi] Connection failed, incorrect password!");
      return WL_WRONG_PASSWORD;
    };
    
    case WL_NO_SSID_AVAIL : {      
      Serial.println("[WiFi] Connection failed, no WiFi with such SSID is found!");
      return WL_NO_SSID_AVAIL;
    };

    case WL_CONNECTED : {
      ledOn(LED_BUILTIN);

      Serial.print("[WiFi] Connected, IP address: ");
      Serial.println(WiFi.localIP());
      return WL_CONNECTED;
    };

    case -1 : {
      Serial.println("[WiFi] Connection failed, timeout!");
      return -1;
    }

    default : {
      Serial.println("[WiFi] Connection failed!");
      return WL_CONNECT_FAILED;      
    }
  }
}