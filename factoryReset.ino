bool factoryReset() {
  Serial.print("[Settings/Factory Reset]\nProceed? (Y): ");
  int8_t cd = 11;
  char choice;
  unsigned long prevmillis;
  do {
    if (millis() - prevmillis >= 1000)
    {
      cd--;
      if (cd >= 0)
        Serial.print(cd), Serial.print(" ");
      prevmillis = millis();
    }
  } while (!Serial.available() && cd >= 0);
  if (Serial.available())
    choice = toupper(Serial.readStringUntil('\n').charAt(0));
  else
    choice = 'N';
  Serial.println();
  if (choice != 'Y') {
    Serial.println("Aborted");
    return false;
  }
  Serial.print("Clearing "), Serial.print(EEPROM.length()), Serial.println(" bytes...");
  for (uint32_t i = 0; i < EEPROM.length(); i++)
    EEPROM.update(i, 255);
  Serial.println("Done");
  return true;
}
