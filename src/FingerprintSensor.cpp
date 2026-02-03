#include "FingerprintSensor.h"

FingerprintSensor::FingerprintSensor(Adafruit_Fingerprint& sensor)
    : sensor_(sensor) {}

// Initialize communication with the sensor
bool FingerprintSensor::init() {
    return sensor_.verifyPassword();
}

// Perform a full fingerprint scan using the sensor
RawMatchResult FingerprintSensor::scan() {
    uint8_t p = sensor_.getImage();
    if (p != FINGERPRINT_OK) {
        return { -1, 0, p };
    }

    p = sensor_.image2Tz();
    if (p != FINGERPRINT_OK) {
        return { -1, 0, p };
    }

    p = sensor_.fingerSearch();
    if (p != FINGERPRINT_OK) {
        return { -1, 0, p };
    }

    // Successful match
    return {
        sensor_.fingerID,
        sensor_.confidence,
        FINGERPRINT_OK
    };
}

// Read raw parameters from the sensor
RawSensorParameters FingerprintSensor::readParameters() {
    sensor_.getParameters();

    RawSensorParameters params;
    params.statusReg     = sensor_.status_reg;
    params.systemId      = sensor_.system_id;
    params.capacity      = sensor_.capacity;
    params.securityLevel = sensor_.security_level;
    params.deviceAddress = sensor_.device_addr;
    params.packetLen     = sensor_.packet_len;
    params.baudRate      = sensor_.baud_rate;

    return params;
}
