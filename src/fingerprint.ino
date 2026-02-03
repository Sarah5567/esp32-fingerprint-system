#include <Arduino.h>
#include <HardwareSerial.h>
#include <Adafruit_Fingerprint.h>

#include "FingerprintSensor.h"
#include "SensorParser.h"
#include "FingerprintService.h"
#include "SystemController.h"

// Hardware
HardwareSerial mySerial(2); // UART2 on ESP32
Adafruit_Fingerprint finger(&mySerial);

// Layers
FingerprintSensor hwSensor(finger);
FingerprintService authService(hwSensor);
SystemController systemController(authService);

// Lifecycle
void setup() {
    Serial.begin(9600);
    delay(100);

    // Initialize UART for fingerprint sensor
    mySerial.begin(57600, SERIAL_8N1, 16, 17);
    finger.begin(57600);

    Serial.println("System booting...");

    // Initialize authentication system
    if (!authService.init()) {
        Serial.println("Sensor initialization failed");
        while (1); // halt system
    }

    Serial.println("System ready");
}

void loop() {
    systemController.update();
    delay(50); // small delay to avoid busy loop
}
