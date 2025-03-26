#include <Wire.h>
#include "MPU9250.h"

MPU9250 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);

  if (!mpu.setup(0x68)) {
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
  Serial.println("MPU connected");
}

void loop() {
  if (mpu.update()) {
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 25) { // Update and send data every 25ms (40Hz)
      prev_ms = millis();
      send_rpy_serial();
    }
  }
}

void send_rpy_serial() {
  float yaw = -mpu.getYaw();
  float pitch = -mpu.getPitch();
  float roll = mpu.getRoll();

  // Send data via Serial Monitor
  Serial.print("Roll, Pitch, Yaw: ");
  Serial.print(roll, 0); Serial.print(", ");
  Serial.print(pitch, 0); Serial.print(", ");
  Serial.println(yaw, 0);
}