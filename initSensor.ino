void initSensor() {
  static const uint16_t blinkDuration = 500;
  bool bl = true;
  unsigned long prevmillis;
  Serial.println(F("Setting up sensor..."));
  bool sensorInit = tcs.begin();
  while (!sensorInit) {
    relayType ? digitalWrite(ylwPin, LOW) : digitalWrite(ylwPin, HIGH);
    if (Serial.available()) {
      char cmd = Serial.readStringUntil('\n').charAt(0);
      if (toupper(cmd) == 'S') {
        turnOffOutputs();
        settings();
      }
    }
    //    blinks LED_BUILTIN and runPin
    if (prevmillis == 0 || millis() - prevmillis >= blinkDuration) {
      bl = !bl;
      Serial.println(F("Sensor error (Send 'S' to enter settings)."));
      relayType ? digitalWrite(runPin, bl) : digitalWrite(runPin, !bl);
      digitalWrite(LED_BUILTIN, bl);
      sensorInit = tcs.begin();
      prevmillis = millis();
    }
  }
  Serial.println(F("Sensor ready."));
  relayType ? digitalWrite(runPin, HIGH) : digitalWrite(runPin, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
}
