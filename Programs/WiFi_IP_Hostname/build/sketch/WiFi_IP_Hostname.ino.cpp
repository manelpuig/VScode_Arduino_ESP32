#include <Arduino.h>
#line 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_IP_Hostname\\WiFi_IP_Hostname.ino"
#include <WiFi.h>

// Wi-Fi credentials
const char* ssid = "Robotics_UB";
const char* password = "rUBot_xx"; // Replace with your Wi-Fi password

// Network settings
IPAddress local_IP(192, 168, 1, 100); // Replace with your desired fixed IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); // Optional: set a secondary DNS if needed

// Hostname
const char* hostname = "ESP32_Robotics";

#line 16 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_IP_Hostname\\WiFi_IP_Hostname.ino"
void configureNetwork();
#line 42 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_IP_Hostname\\WiFi_IP_Hostname.ino"
void setup();
#line 47 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_IP_Hostname\\WiFi_IP_Hostname.ino"
void loop();
#line 16 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_IP_Hostname\\WiFi_IP_Hostname.ino"
void configureNetwork() {
  // Set hostname
  WiFi.setHostname(hostname);

  // Configure fixed IP
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print connection details
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(WiFi.getHostname());
}

void setup() {
  Serial.begin(115200);
  configureNetwork();
}

void loop() {
  // Your code here
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(WiFi.getHostname());
  delay(5000);
}
