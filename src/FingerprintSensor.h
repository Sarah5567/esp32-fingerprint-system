#pragma once

#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

// Raw result from the sensor (no system logic here)
struct RawMatchResult {
    int id;             // fingerID from sensor
    int confidence;     // confidence score
    uint8_t status;    // FINGERPRINT_* status code
};

// Raw sensor parameters (as provided by the sensor)
struct RawSensorParameters {
    uint16_t statusReg;
    uint16_t systemId;
    uint16_t capacity;
    uint8_t securityLevel;
    uint32_t deviceAddress;
    uint16_t packetLen;
    uint32_t baudRate;
};

class FingerprintSensor {
public:
    explicit FingerprintSensor(Adafruit_Fingerprint& sensor);

    bool init();                          // initialize sensor
    RawMatchResult scan();                // perform fingerprint scan
    RawSensorParameters readParameters(); // read sensor parameters

private:
    Adafruit_Fingerprint& sensor_;
};
