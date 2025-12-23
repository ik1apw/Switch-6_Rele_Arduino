// -----------------------------------------------------------------------------
// Project: Switch 6 Porte Remoto – IK1APW
// Author:  Emanuele Rossi - IK1APW
// Hardware: Arduino Nano + OLED SSD1306 + Encoder + EEPROM
// Version: Final Remoto
// -----------------------------------------------------------------------------

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <Encoder.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Encoder myEnc(3, 4);
const int ENCODER_BTN = 5;
bool lastButtonState = HIGH;

const int NUM_PORTS = 6;
const int ports[NUM_PORTS] = {6, 7, 8, 9, 10, 11};
bool portState[NUM_PORTS] = {0, 0, 0, 0, 0, 0};

const int EEPROM_ADDR = 0;
const int EEPROM_SIZE = NUM_PORTS;

long oldPosition = -999;
int selectedPort = 0;

String inputString = "";
bool stringComplete = false;

void saveStateToEEPROM() {
  for (int i = 0; i < NUM_PORTS; i++) EEPROM.update(EEPROM_ADDR + i, portState[i]);
}

void loadStateFromEEPROM() {
  for (int i = 0; i < NUM_PORTS; i++) {
    portState[i] = EEPROM.read(EEPROM_ADDR + i);
    digitalWrite(ports[i], portState[i]);
  }
}

void clearEEPROM() {
  for (int i = 0; i < EEPROM_SIZE; i++) EEPROM.update(EEPROM_ADDR + i, 0);
}

void drawScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Switch 6 Porte Remoto"));
  display.setCursor(0, 10);
  display.println(F("IK1APW"));
  display.drawLine(0, 20, 128, 20, SSD1306_WHITE);
  for (int i = 0; i < NUM_PORTS; i++) {
    display.setCursor(10, 25 + i * 7);
    display.print("Port ");
    display.print(i + 1);
    display.print(": ");
    display.print(portState[i] ? "ON " : "OFF");
    if (i == selectedPort) display.print(" <");
  }
  display.display();
}

void togglePort(int index) {
  portState[index] = !portState[index];
  digitalWrite(ports[index], portState[index]);
  saveStateToEEPROM();
  drawScreen();
}

void processSerialCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();
  if (cmd.startsWith("ON")) {
    int n = cmd.substring(2).toInt();
    if (n >= 1 && n <= NUM_PORTS) {
      portState[n - 1] = HIGH;
      digitalWrite(ports[n - 1], HIGH);
      saveStateToEEPROM();
      drawScreen();
    }
  } else if (cmd.startsWith("OFF")) {
    int n = cmd.substring(3).toInt();
    if (n >= 1 && n <= NUM_PORTS) {
      portState[n - 1] = LOW;
      digitalWrite(ports[n - 1], LOW);
      saveStateToEEPROM();
      drawScreen();
    }
  } else if (cmd == "STATUS") {
    Serial.print("STATE:");
    for (int i = 0; i < NUM_PORTS; i++) {
      Serial.print(portState[i]);
      if (i < NUM_PORTS - 1) Serial.print(",");
    }
    Serial.println();
  } else if (cmd == "CLEAR") {
    clearEEPROM();
    loadStateFromEEPROM();
    drawScreen();
    Serial.println("EEPROM CLEARED");
  }
}

void setup() {
  for (int i = 0; i < NUM_PORTS; i++) pinMode(ports[i], OUTPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) for (;;) ;
  Serial.begin(9600);
  Serial.setTimeout(10);
  pinMode(ENCODER_BTN, INPUT_PULLUP);
  loadStateFromEEPROM();
  drawScreen();
}

void loop() {
  long newPosition = myEnc.read() / 4;
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    if (newPosition > 0) selectedPort = (selectedPort + 1) % NUM_PORTS;
    if (newPosition < 0) selectedPort = (selectedPort - 1 + NUM_PORTS) % NUM_PORTS;
    myEnc.write(0);
    drawScreen();
  }

  bool buttonState = digitalRead(ENCODER_BTN);
  if (buttonState == LOW && lastButtonState == HIGH) {
    togglePort(selectedPort);
    delay(200);
  }
  lastButtonState = buttonState;

  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') stringComplete = true;
    else inputString += inChar;
  }
  if (stringComplete) {
    processSerialCommand(inputString);
    inputString = "";
    stringComplete = false;
  }
}
