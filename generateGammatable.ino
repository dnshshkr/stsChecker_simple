void generateGammatable() {
  Serial.println(F("Generating gamma table..."));
  for (uint16_t i = 0; i < 256; i++) {
    // float f = i;
    // f /= 255.0;
    // f = pow(f, 2.5);
    // f *= 255.0;
    uint8_t f = (int)(pow((float)i / 255.0, 2.5) * 255.0);
    rgbCom ? gammatable[i] = 255 - f : gammatable[i] = f;
    // Serial.println(String(i) + " " + String(gammatable[i]));
  }
  Serial.println(F("Done."));
}