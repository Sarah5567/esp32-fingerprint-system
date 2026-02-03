#pragma once

#include <cstdint>
#include "FingerprintSensor.h"

// Parsed & normalized sensor configuration
struct SensorProfile {
    uint16_t capacity;        // number of stored fingerprints
    uint8_t securityLevel;    // sensor security level
    uint32_t baudRate;        // UART baud rate
    bool isReady;             // derived from status register
};

// Parsed scan result (still no system decision)
enum class ScanStatus {
    OK,
    NO_FINGER,
    COMM_ERROR,
    IMAGE_ERROR,
    UNKNOWN_ERROR
};

struct ParsedScanResult {
    int id;
    int confidence;
    ScanStatus status;
};

class SensorParser {
public:
    static SensorProfile parseParameters(const RawSensorParameters& raw);
    static ParsedScanResult parseScanResult(const RawMatchResult& raw);

private:
    static bool parseReadyFlag(uint16_t statusReg);
};
