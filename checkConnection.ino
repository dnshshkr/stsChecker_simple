bool checkConnection() {
  Wire.beginTransmission(TCS34725_ADDRESS);
  bool res = (Wire.endTransmission() == 0);
  if (res)
    return true;
  else {
    Serial.println(F("Sensor disconnected."));
    return false;
  }
}