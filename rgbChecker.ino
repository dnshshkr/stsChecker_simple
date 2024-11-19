#define VERSION "5.0"
#include <EEPROM.h>
#include "Adafruit_TCS34725.h"
#define COLOR "Yellow"
#define detPin 5
#define runPin 6
#define chPin 7
#define rPin A0
#define gPin A1
#define bPin A2
#define chSize 6
#define relayTypeAddr 0
#define repClrAddr 1
#define rgbComAddr 2
#define lrCh1Addr 3
#define hrCh1Addr 4
#define lgCh1Addr 5
#define hgCh1Addr 6
#define lbCh1Addr 7
#define hbCh1Addr 8
#define lrCh2Addr 9
#define hrCh2Addr 10
#define lgCh2Addr 11
#define hgCh2Addr 12
#define lbCh2Addr 13
#define hbCh2Addr 14
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
// const uint8_t rPin = A0, gPin = A1, bPin = A2;
bool relayType, repClr, rgbCom;
uint8_t r, g, b, gammatable[256];
uint8_t rgbDict[2][2][chSize] = {
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
  Serial.begin(115200);
  pinMode(detPin, OUTPUT);
  pinMode(runPin, OUTPUT);
  pinMode(chPin, INPUT_PULLUP);
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  getVals();
  relayType = EEPROM.read(relayTypeAddr);  //  0: LOW, 1: HIGH
  repClr = EEPROM.read(repClrAddr);        //  0: NO, 1: YES
  rgbCom = EEPROM.read(rgbComAddr);        //  0: anode, 1: cathode
  printInfo();
  generateGammatable();
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
    // digitalWrite(runPin, !relayType);
    turnOffOutputs();
    initSensor();
  }
  digitalWrite(runPin, relayType);
  digitalWrite(LED_BUILTIN, HIGH);
  //  reads RGB data from sensor
  tcs.getRGB(&fr, &fg, &fb);
  r = round(fr), g = round(fg), b = round(fb);
  //  reproduce the color on RGB LED if exists
  uint8_t repR, repG, repB;
  repClr ? repR = r, repG = g, repB = b : repR = 0, repG = 0, repB = 0;
  analogWrite(rPin, gammatable[repR]);
  analogWrite(gPin, gammatable[repG]);
  analogWrite(bPin, gammatable[repB]);
  //  reads channel (HIGH for channel 1, LOW for channel 2)
  bool ch = !digitalRead(chPin);
  Serial.print("Channel ");
  ch ? Serial.println(1) : Serial.println(2);
  Serial.print("R: "), Serial.print(r), Serial.print("\tG: "), Serial.print(g), Serial.print("\tB: "), Serial.print(b), Serial.print("\tRes: ");
  //  matches read RGB data with stored RGB data on either channels
  if (r >= rgbDict[ch][1][0] && r <= rgbDict[ch][1][1] && g >= rgbDict[ch][1][2] && g <= rgbDict[ch][1][3] && b >= rgbDict[ch][1][4] && b <= rgbDict[ch][1][5]) {
    Serial.println(COLOR);
    digitalWrite(detPin, relayType);
  } else {
    Serial.println("None");
    digitalWrite(detPin, !relayType);
  }
  Serial.println();
}
void printInfo() {
  Serial.print(F("RGB Checker Ver. ")), Serial.println(VERSION);
  Serial.println(F("Copyright(C) Delloyd R&D (M) Sdn Bhd"));
}
void reboot() {
  Serial.println(F("Rebooting..."));
  delay(100);
  resMcu();
}
