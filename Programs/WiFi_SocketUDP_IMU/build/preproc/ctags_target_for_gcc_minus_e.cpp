# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_IMU\\WiFi_SocketUDP_IMU.ino"
# 2 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_IMU\\WiFi_SocketUDP_IMU.ino" 2
# 3 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_IMU\\WiFi_SocketUDP_IMU.ino" 2
# 4 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_IMU\\WiFi_SocketUDP_IMU.ino" 2

// Wi-Fi credentials
const char *ssid = "Robotics_UB";
const char *password = "rUBot_xx";

// MPU-9250 object
MPU9250 mpu;

// UDP object
WiFiUDP udp;

// IP i port del receptor
const char *udpAddress = "192.168.1.3"; // Canvia-ho per la IP del teu ordinador
const int udpPort = 12345; // Port del receptor

// Orientation data
float roll = 0.0, pitch = 0.0, yaw = 0.0;

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

void updateOrientation() {
  if (mpu.update()) {
    yaw = -mpu.getYaw();
    pitch = -mpu.getPitch();
    roll = mpu.getRoll();
  }
}

void sendOrientationUDP() {
  // Prepara les dades a enviar en format JSON
  String json = "{";
  json += "\"roll\":" + String(roll, 2) + ",";
  json += "\"pitch\":" + String(pitch, 2) + ",";
  json += "\"yaw\":" + String(yaw, 2);
  json += "}";
  // Print to Serial Monitor
  //Serial.println("Sending data: " + json);
  // Envia les dades via UDP
  udp.beginPacket(udpAddress, udpPort);
  udp.print(json);
  udp.endPacket();
}

void setup() {
  Serial0.begin(115200);
  Wire.begin();
  delay(2000);

  // Inicialitza el MPU-9250
  if (!mpu.setup(0x68)) {
    while (1) {
      Serial0.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
  Serial0.println("MPU connected");
  delay(2000);

  // Connecta a la xarxa Wi-Fi
  connectToWiFi();

  // Comença UDP
  udp.begin(udpPort);
  Serial0.println("UDP initialized.");
}

void loop() {
  updateOrientation(); // Actualitza les dades del sensor
  sendOrientationUDP(); // Envia les dades al receptor via UDP
  delay(10); // Ajusta la freqüència d'enviament si cal
}
