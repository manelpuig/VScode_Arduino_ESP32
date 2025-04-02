#include <WiFi.h>
#include <WiFiUdp.h>
#include "MPU9250.h"
#include <Wire.h> //needed for I2C to read IMU
#include <ArduinoJson.h>

// Device ID
const char *deviceId = "G6_Endo";

// Wi-Fi credentials
const char *ssid = "Robotics_UB";
const char *password = "rUBot_xx";

// UDP settings
IPAddress receiverESP32IP(192, 168, 1, 6); // IP address of the receiver ESP32 - CHANGE THIS!
IPAddress receiverComputerIP(192, 168, 1, 3); // IP address of your computer - CHANGE THIS!
const int udpPort = 12345;
WiFiUDP udp;

// MPU-9250 object
MPU9250 mpu;

// Orientation data
float Endo_roll = 0.0, Endo_pitch = 0.0, Endo_yaw = 0.0;

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.println("IP Address: " + WiFi.localIP().toString());
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void updateOrientation() {
  if (mpu.update()) {
    Endo_yaw = -mpu.getYaw();
    Endo_pitch = -mpu.getPitch();
    Endo_roll = mpu.getRoll();
  }
}

void sendOrientationUDP() {
  StaticJsonDocument<256> doc;
  doc["device"] = deviceId;
  doc["roll"] = Endo_roll;
  doc["pitch"] = Endo_pitch;
  doc["yaw"] = Endo_yaw;

  char jsonBuffer[512];
  size_t bytes = serializeJson(doc, jsonBuffer);
    if (bytes == 0){
        Serial.println(F("Serialization Failed"));
        return;
    }

  // Send to ESP32
  udp.beginPacket(receiverESP32IP, udpPort);
  udp.write((const uint8_t*)jsonBuffer, bytes); // Cast to const uint8_t*
  udp.endPacket();

  // Send to Computer
  udp.beginPacket(receiverComputerIP, udpPort);
  udp.write((const uint8_t*)jsonBuffer, bytes); // Cast to const uint8_t*
  udp.endPacket();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();//needed for I2C to read IMU
  delay(2000);

  // Inicialitza el MPU-9250
  if (!mpu.setup(0x68)) {
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
  Serial.println("MPU connected");
  delay(2000);

  // Connecta a la xarxa Wi-Fi
  connectToWiFi();

  // Comença UDP
  udp.begin(udpPort);
  Serial.println("UDP initialized.");
}

void loop() {
  updateOrientation(); // Update Endo RPY data
  sendOrientationUDP(); // Send Endo RPY data to Gripper and Computer with UDP
  delay(10); // sending rate
}
