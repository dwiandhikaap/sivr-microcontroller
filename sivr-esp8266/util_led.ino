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