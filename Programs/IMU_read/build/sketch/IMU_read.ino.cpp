#include <Arduino.h>
#line 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_read\\IMU_read.ino"
#include <Wire.h>
#include "src/RoboticsUB.h"

IMU imu;
float *rpw;
const int PIN_IMU_INT = 18;       // Interrupt pin

#line 8 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_read\\IMU_read.ino"
void setup();
#line 15 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_read\\IMU_read.ino"
void loop();
#line 8 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_read\\IMU_read.ino"
void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(PIN_IMU_INT, INPUT_PULLUP);
  imu.Install();
}

void loop() {
  if (digitalRead(PIN_IMU_INT) == HIGH) { // Check for interrupt
    imu.ReadSensor();
    rpw = imu.GetRPW();
    if (rpw) { // Check if rpw is valid
        Serial.print("Roll: ");   Serial.print(rpw[0]); Serial.print("\t");
        Serial.print("Pitch: ");  Serial.print(rpw[1]); Serial.print("\t");
        Serial.print("Yaw: ");    Serial.println(rpw[2]);
        } 
        else {
        Serial.println("Failed to get RPW values.");
        }
  }
  delay(50);
}