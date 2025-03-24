# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_IP_Hostname\\WiFi_IP_Hostname.ino"
# 2 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_IP_Hostname\\WiFi_IP_Hostname.ino" 2

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

void configureNetwork() {
  // Set hostname
  WiFi.setHostname(hostname);

  // Configure fixed IP
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
    Serial0.println("STA Failed to configure");
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial0.println("Connecting to WiFi...");
  }

  // Print connection details
  Serial0.println("Connected to WiFi");
  Serial0.print("IP Address: ");
  Serial0.println(WiFi.localIP());
  Serial0.print("Hostname: ");
  Serial0.println(WiFi.getHostname());
}

void setup() {
  Serial0.begin(115200);
  configureNetwork();
}

void loop() {
  // Your code here
  Serial0.println("Connected to WiFi");
  Serial0.print("IP Address: ");
  Serial0.println(WiFi.localIP());
  Serial0.print("Hostname: ");
  Serial0.println(WiFi.getHostname());
  delay(5000);
}
