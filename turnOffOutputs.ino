void turnOffOutputs() {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(runPin, !relayType);
  digitalWrite(detPin, !relayType);
  analogWrite(rPin, 0);
  analogWrite(gPin, 0);
  analogWrite(bPin, 0);
}