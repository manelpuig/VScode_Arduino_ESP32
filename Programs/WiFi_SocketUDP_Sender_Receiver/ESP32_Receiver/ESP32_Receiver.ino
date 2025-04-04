#include <WiFi.h>
#include <WiFiUdp.h>
#include "MPU9250.h"
#include <Wire.h> //needed for I2C to read IMU
#include <ArduinoJson.h>

// Device ID
const char *deviceId = "G6_Gri";

// Wi-Fi credentials
const char *ssid = "Robotics_UB";
const char *password = "rUBot_xx";

// UDP settings
IPAddress receiverComputerIP(192, 168, 1, 3); // IP address of your computer - CHANGE THIS!
const int udpPort = 12345;
WiFiUDP udp;

// MPU-9250 object
MPU9250 mpu;

// Variables to store received data
float Endo_roll = 0.0, Endo_pitch = 0.0, Endo_yaw = 0.0;
float Gri_roll = 0.0, Gri_pitch = 0.0, Gri_yaw = 0.0;

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
    Gri_yaw = -mpu.getYaw();
    Gri_pitch = -mpu.getPitch();
    Gri_roll = mpu.getRoll();
  }
}

void sendOrientationUDP() {
  StaticJsonDocument<256> doc;
  doc["device"] = deviceId;
  doc["roll"] = Gri_roll;
  doc["pitch"] = Gri_pitch;
  doc["yaw"] = Gri_yaw;

  char jsonBuffer[512];
  size_t bytes = serializeJson(doc, jsonBuffer);
    if (bytes == 0){
        Serial.println(F("Serialization Failed"));
        return;
    }
  // Send to Computer
  udp.beginPacket(receiverComputerIP, udpPort);
  udp.write((const uint8_t*)jsonBuffer, bytes); // Cast to const uint8_t*
  udp.endPacket();
}

void receiveOrientationUDP() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    byte packetBuffer[512];
    int len = udp.read(packetBuffer, 512);
    if (len > 0) {
      packetBuffer[len] = '\0';
      Serial.print("Received packet size: ");
      Serial.println(packetSize);
      Serial.print("Received: ");
      Serial.println((char*)packetBuffer);

      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, packetBuffer);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      const char* device = doc["device"];
      if (strcmp(device, "G6_Endo") == 0) {
        Endo_roll = round(doc["roll"].as<float>());
        Endo_pitch = round(doc["pitch"].as<float>());
        Endo_yaw = round(doc["yaw"].as<float>());
        Serial.print("Endo_Roll: "); Serial.print(Endo_roll); Serial.print(" Endo_Pitch: "); Serial.print(Endo_pitch); Serial.print(" Endo_Yaw: "); Serial.println(Endo_yaw);
      } else {
        Serial.println("Unknown device.");
      }
    }
  }
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

  // Connect to WiFi network
  connectToWiFi();
  // Init UDP
  udp.begin(udpPort);
  Serial.println("UDP initialized");
}

void loop() {
  receiveOrientationUDP();
  updateOrientation(); // Update IMU data from Gripper
  sendOrientationUDP(); // Send IMU data to computer with UDP
  delay(10);
}