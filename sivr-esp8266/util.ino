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
  Serial.println(strNetworksFound);
  return strNetworksFound;
}

void log(char* str){
  Serial.println(str);
}

void resetEEPROM(){
  for (int i = 0; i < 512; i++) {
      EEPROM.write(i, 0);
    }
  EEPROM.commit();
  delay(500);
}

void writeConfig(String ssid, String password){
  if(RESET_EEPROM){
    resetEEPROM();
  }

  int ssidLength = ssid.length()+1;
  int passwordLength = password.length();

  int addr = 0;
  for(int i = 0; i < ssidLength; i++){
    EEPROM.write(addr, ssid[i]);
    addr++;
  }
  EEPROM.write(addr+1, '\0');
  Serial.println("");

  addr = 48;
  for(int i = 0; i < passwordLength; i++){
    EEPROM.write(addr, password[i]);
    addr++;
  }
  EEPROM.write(addr+1, '\0');
  Serial.println("");

  if (EEPROM.commit()) {
    Serial.println("Data successfully committed");
  } else {
    Serial.println("ERROR! Data commit failed");
  }
}

String readSSID(){
  String ssid = "";
  int addr = 0;

  for(int i = 0; i < 48; i++){
    if(char(EEPROM.read(addr)) == '\0'){
      break;
    }        
    ssid += char(EEPROM.read(addr));
    addr++;
  }
  
  Serial.println("");
  Serial.print("SSID : ");
  Serial.println(ssid);
  return ssid;
}

/* DynamicJsonDocument readConfig(){
  DynamicJsonDocument config[128];

  int addr = 0;
  for(int i = 0; i < 48; i++){
    config["ssid"][i] = EEPROM.read(addr);
  }

  for(int i = 0; i < 48; i++){
    config["password"][i] = EEPROM.read(addr);
  }

  return config;
} */