void selfCalibrate(uint8_t* valAddrs) {
  static const uint8_t readCount = 128;
  uint16_t totalR = 0, totalG = 0, totalB = 0;
  int16_t vals[chSize];
  Serial.print("Turn on STS before continuing\nEnter offset (0-255): ");
  while (!Serial.available());
  uint8_t tol = Serial.readStringUntil('\n').toInt();
  Serial.println(tol);
  Serial.println("Calibrating...");
  //  collects RGB data up to readCount value
  for (uint8_t i = 0; i < readCount; i++) {
    if (!checkConnection()) {
      Serial.println("Calibration failed");
      //      return false; //  disables self-calibrate function on next calls until sensor is reinitialized
      return;
    }
    tcs.getRGB(&fr, &fg, &fb);
    r = round(fr), g = round(fg), b = round(fb);
    totalR += r, totalG += g, totalB += b;
  }
  //  gets average RGB values
  r = round((float)totalR / readCount);
  g = round((float)totalG / readCount);
  b = round((float)totalB / readCount);
  //  calculates offsets
  vals[0] = r - tol;
  vals[1] = r + tol;
  vals[2] = g - tol;
  vals[3] = g + tol;
  vals[4] = b - tol;
  vals[5] = b + tol;
  Serial.println("Average"), Serial.print("R: "), Serial.print(r), Serial.print("\tG: "), Serial.print(g), Serial.print("\tB: "), Serial.println(b);
  //  adjusts values in vals so that LR, LG, LB don't fall below 0 and HR, HG, HB don't exceed 255
  for (uint8_t i = 0; i < chSize; i += 2) {
    if (vals[i] < 0)
      vals[i] = 0;
  }
  for (uint8_t i = 1; i < chSize; i += 2) {
    if (vals[i] > 255)
      vals[i] = 255;
  }
  //  stores RGB offset values in EEPROM
  for (uint8_t i = 0; i < chSize; i++)
    EEPROM.update(valAddrs[i], vals[i]);
  Serial.println("Calibration done");
  return;
}
