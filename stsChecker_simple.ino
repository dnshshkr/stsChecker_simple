#define VERSION "4.3"
#include <EEPROM.h>
#include "Adafruit_TCS34725.h"
#define ylwPin 5
#define runPin 6
#define chPin 7
#define chSize 6
#define relayTypeAddr 0
#define lrCh1Addr 1
#define hrCh1Addr 2
#define lgCh1Addr 3
#define hgCh1Addr 4
#define lbCh1Addr 5
#define hbCh1Addr 6
#define lrCh2Addr 7
#define hrCh2Addr 8
#define lgCh2Addr 9
#define hgCh2Addr 10
#define lbCh2Addr 11
#define hbCh2Addr 12
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
const uint8_t rPin = A0, gPin = A1, bPin = A2;
bool relayType;
uint8_t r, g, b;
uint8_t ylwDict[2][2][chSize] = {
  //  channel 1
  {
    { lrCh1Addr, hrCh1Addr, lgCh1Addr, hgCh1Addr, lbCh1Addr, hbCh1Addr },  //  addresses of channel 1
    {}                                                                     //  values of channel 1
  },
  //  channel 2
  {
    { lrCh2Addr, hrCh2Addr, lgCh2Addr, hgCh2Addr, lbCh2Addr, hbCh2Addr },  //  addresses of channel 2
    {}                                                                     //  values of channel 2
  }
};
float fr, fg, fb;
void (*resMcu)() = 0;
void setup() {
  pinMode(chPin, INPUT_PULLUP);
  pinMode(runPin, OUTPUT);
  pinMode(ylwPin, OUTPUT);
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  getVals();
  relayType = EEPROM.read(relayTypeAddr);
  Serial.begin(115200);
  printInfo();
  initSensor();
}
void loop() {
  //  checks for user interruption
  if (Serial.available()) {
    char cmd = toupper(Serial.readStringUntil('\n').charAt(0));
    if (cmd == 'S') {
      turnOffOutputs();
      settings();
    }
  }
  //  checks if connection to sensor exists
  if (!checkConnection()) {
    relayType ? digitalWrite(runPin, LOW) : digitalWrite(runPin, HIGH);
    initSensor();
  }
  relayType ? digitalWrite(runPin, HIGH) : digitalWrite(runPin, LOW);
  //  reads RGB data from sensor
  tcs.getRGB(&fr, &fg, &fb);
  r = round(fr), g = round(fg), b = round(fb);
  //  simulates the color on RGB light if exists
  analogWrite(rPin, r);
  analogWrite(gPin, g);
  analogWrite(bPin, b);
  //  reads channel (HIGH for channel 1, LOW for channel 2)
  bool ch = !digitalRead(chPin);
  Serial.print("Channel ");
  ch ? Serial.println(1) : Serial.println(2);
  Serial.print("R: "), Serial.print(r), Serial.print("\tG: "), Serial.print(g), Serial.print("\tB: "), Serial.print(b), Serial.println();
  //  matches read RGB data with stored RGB data on either channels
  if (r >= ylwDict[ch][1][0] && r <= ylwDict[ch][1][1] && g >= ylwDict[ch][1][2] && g <= ylwDict[ch][1][3] && b >= ylwDict[ch][1][4] && b <= ylwDict[ch][1][5]) {
    Serial.println("Yellow");
    relayType ? digitalWrite(ylwPin, HIGH) : digitalWrite(ylwPin, LOW);
  } else {
    Serial.println("None");
    relayType ? digitalWrite(ylwPin, LOW) : digitalWrite(ylwPin, HIGH);
  }
  Serial.println();
}
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
void getVals() {
  for (uint8_t i = 0; i < chSize; i++) {
    ylwDict[0][1][i] = EEPROM.read(ylwDict[0][0][i]);
    ylwDict[1][1][i] = EEPROM.read(ylwDict[1][0][i]);
  }
}
void printInfo() {
  Serial.print(F("STS Checker Ver. ")), Serial.println(VERSION);
  Serial.println(F("Copyright(C) Delloyd R&D (M) Sdn Bhd"));
}
void reboot() {
  Serial.println(F("Rebooting..."));
  delay(100);
  resMcu();
}
void turnOffOutputs() {
  digitalWrite(LED_BUILTIN, LOW);
  if (relayType) {
    digitalWrite(runPin, LOW);
    digitalWrite(ylwPin, LOW);
  } else {
    digitalWrite(runPin, HIGH);
    digitalWrite(ylwPin, HIGH);
  }
  analogWrite(rPin, 0);
  analogWrite(gPin, 0);
  analogWrite(bPin, 0);
}
