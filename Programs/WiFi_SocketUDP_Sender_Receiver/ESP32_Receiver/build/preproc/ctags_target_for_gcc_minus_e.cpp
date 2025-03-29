# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Receiver\\ESP32_Receiver.ino"
# 2 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Receiver\\ESP32_Receiver.ino" 2

# 4 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Receiver\\ESP32_Receiver.ino" 2

// Wi-Fi credentials
const char *ssid = "Robotics_UB";
const char *password = "rUBot_xx";

// UDP settings
const int udpPort = 12345;
WiFiUDP udp;

// Variables to store received data
float roll1 = 0.0, pitch1 = 0.0, yaw1 = 0.0;
float roll2 = 0.0, pitch2 = 0.0, yaw2 = 0.0;

void connectToWiFi() {
  Serial0.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial0.print(".");
  }
  Serial0.println("\nWi-Fi connected!");
  Serial0.println("IP Address: " + WiFi.localIP().toString());
  Serial0.print("ESP32 MAC Address: ");
  Serial0.println(WiFi.macAddress());
}

void receiveOrientationUDP() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    byte packetBuffer[512];
    int len = udp.read(packetBuffer, 512);
    if (len > 0) {
      packetBuffer[len] = '\0';
      Serial0.print("Received packet size: ");
      Serial0.println(packetSize);
      Serial0.print("Received: ");
      Serial0.println((char*)packetBuffer);

      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, packetBuffer);
      if (error) {
        Serial0.print(((reinterpret_cast<const __FlashStringHelper *>(("deserializeJson() failed: ")))));
        Serial0.println(error.f_str());
        return;
      }

      const char* device = doc["device"];
      if (strcmp(device, "ESP32_1") == 0) {
        roll1 = round(doc["roll"].as<float>());
        pitch1 = round(doc["pitch"].as<float>());
        yaw1 = round(doc["yaw"].as<float>());
        Serial0.print("Roll_1: "); Serial0.print(roll1); Serial0.print(" Pitch_1: "); Serial0.print(pitch1); Serial0.print(" Yaw_1: "); Serial0.println(yaw1);
      } else if (strcmp(device, "ESP32_2") == 0) {
        roll2 = round(doc["roll"].as<float>());
        pitch2 = round(doc["pitch"].as<float>());
        yaw2 = round(doc["yaw"].as<float>());
        Serial0.print("Roll_2: "); Serial0.print(roll2); Serial0.print(" Pitch_2: "); Serial0.print(pitch2); Serial0.print(" Yaw_2: "); Serial0.println(yaw2);
      } else {
        Serial0.println("Unknown device.");
      }
    }
  }
}

void setup() {
  Serial0.begin(115200);
  connectToWiFi();
  udp.begin(udpPort);
  Serial0.println("UDP receiver started");
}

void loop() {
  receiveOrientationUDP();
  delay(10);
}
