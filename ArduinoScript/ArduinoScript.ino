#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BUTTON_PIN 3

LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address if needed

const int resetPin = 2;

int buttonState = 0;
int lastButtonState = 0;
int messageIndex = 0;

void setup() {
  lcd.init(); // Initialize the LCD
  lcd.backlight();
  lcd.print("Please Wait...");
  pinMode(resetPin, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(resetPin), resetArduino, FALLING);
  Serial.begin(115200);
}

void loop() {
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH) {
    messageIndex = (messageIndex + 1) % 4;
    delay(200); // Debounce delay
  }

  lastButtonState = buttonState;

  switch (messageIndex) {
    case 0:
      displayMessage1();
      break;
    case 1:
      displayMessage2();
      break;
    case 2:
      displayMessage3();
      break;
    case 3:
      displayMessage4();
      break;
  }
}

void displayMessage1() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println("Received: " + data); // Print received data to Serial Monitor

    if (data.startsWith("TEMP:")) {
      data = data.substring(5); // Remove the "TEMP:" identifier
      int splitIndex = data.indexOf(':');
      String gpuT = data.substring(0, splitIndex);
      String cpuT = data.substring(splitIndex + 1);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("GPU Temp: " + gpuT);
      lcd.setCursor(0, 1);
      lcd.print("CPU Temp: " + cpuT);

      Serial.println("Displayed on LCD: " + gpuT + " " + cpuT); // Confirm data is displayed
    }
  }
}

void displayMessage2() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println("Received: " + data); // Print received data to Serial Monitor

    if (data.startsWith("UTIL:")) {
      data = data.substring(5); // Remove the "UTIL:" identifier
      int splitIndex = data.indexOf(':');
      String cpuUtil = data.substring(0, splitIndex);
      String cpuFreq = data.substring(splitIndex + 1);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CPU Util: " + cpuUtil + "%");
      lcd.setCursor(0, 1);
      lcd.print("CPU Freq: " + cpuFreq);

      Serial.println("Displayed on LCD: " + cpuUtil + "% " + cpuFreq + " GHz"); // Confirm data is displayed
    }
  }
}

void displayMessage3() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println("Received: " + data); // Print received data to Serial Monitor

    if (data.startsWith("MEM:")) {
      data = data.substring(4); // Remove the "MEM:" identifier
      String memUsage = data;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("System Memory");
      lcd.setCursor(0, 1);
      lcd.print(memUsage);

      Serial.println("Displayed on LCD: " + memUsage); // Confirm data is displayed
    }
  }
}

void displayMessage4() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');

    if (data.startsWith("GPU:")) {
      data = data.substring(4); // Remove the "GPU:" identifier
      int splitIndex = data.indexOf(':');
      if (splitIndex != -1) {
        String gpuUtil = data.substring(0, splitIndex);
        String gpuPower = data.substring(splitIndex + 1);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("GPU Util: " + gpuUtil + "%");
        lcd.setCursor(0, 1);
        lcd.print("GPU Power: " + gpuPower + "W");

        Serial.println("Displayed on LCD: " + gpuUtil + "% " + gpuPower + "W"); // Confirm data is displayed
      } else {
        Serial.println("Error: Invalid data format in Display 4");
      }
    } else {
      Serial.println("Error: Unexpected data in Display 4");
    }
  }
}

void resetArduino() {
  Serial.println(); // Send newline character
  delay(100); // Give time for the message to be sent
  Serial.println("Resetting Arduino..."); // Debugging statement
  asm volatile (" jmp 0"); // Reset the Arduino
}