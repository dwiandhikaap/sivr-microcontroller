void resetEEPROM(){
  for (int i = 0; i < 256; i++) {
      EEPROM.write(i, 0);
    }
  EEPROM.commit();
  delay(500);
}

void writeConfig(String ssid, String password){
  if(RESET_EEPROM){
    resetEEPROM();
  }

  int ssidLength = ssid.length();
  int passwordLength = password.length();

  int addr = 0;
  for(int i = 0; i < ssidLength; i++){
    EEPROM.write(addr, ssid[i]);
    addr++;
  }
  EEPROM.write(addr, '\0');

  addr = 48;
  for(int i = 0; i < passwordLength; i++){
    EEPROM.write(addr, password[i]);
    addr++;
  }
  
  EEPROM.write(addr, '\0');
  EEPROM.commit();
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

  return ssid;
}

String readPassword(){
  String password = "";
  int addr = 48;

  for(int i = 0; i < 64; i++){
    if(char(EEPROM.read(addr)) == '\0'){
      break;
    }        
    password += char(EEPROM.read(addr));
    addr++;
  }
  
  return password;
}