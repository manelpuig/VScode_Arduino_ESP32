#include <Arduino.h>
#line 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Receiver\\ESP32_Receiver.ino"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

// Wi-Fi credentials
const char *ssid = "Robotics_UB";
const char *password = "rUBot_xx";

// UDP settings
const int udpPort = 12345;
WiFiUDP udp;

// Variables to store received data
float roll1 = 0.0, pitch1 = 0.0, yaw1 = 0.0;
float roll2 = 0.0, pitch2 = 0.0, yaw2 = 0.0;

#line 17 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Receiver\\ESP32_Receiver.ino"
void connectToWiFi();
#line 30 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Receiver\\ESP32_Receiver.ino"
void receiveOrientationUDP();
#line 68 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Receiver\\ESP32_Receiver.ino"
void setup();
#line 75 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Receiver\\ESP32_Receiver.ino"
void loop();
#line 17 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Receiver\\ESP32_Receiver.ino"
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
      if (strcmp(device, "ESP32_1") == 0) {
        roll1 = round(doc["roll"].as<float>());
        pitch1 = round(doc["pitch"].as<float>());
        yaw1 = round(doc["yaw"].as<float>());
        Serial.print("Roll_1: "); Serial.print(roll1); Serial.print(" Pitch_1: "); Serial.print(pitch1); Serial.print(" Yaw_1: "); Serial.println(yaw1);
      } else if (strcmp(device, "ESP32_2") == 0) {
        roll2 = round(doc["roll"].as<float>());
        pitch2 = round(doc["pitch"].as<float>());
        yaw2 = round(doc["yaw"].as<float>());
        Serial.print("Roll_2: "); Serial.print(roll2); Serial.print(" Pitch_2: "); Serial.print(pitch2); Serial.print(" Yaw_2: "); Serial.println(yaw2);
      } else {
        Serial.println("Unknown device.");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  udp.begin(udpPort);
  Serial.println("UDP receiver started");
}

void loop() {
  receiveOrientationUDP();
  delay(10);
}
