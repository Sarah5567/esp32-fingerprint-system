#include "SensorParser.h"

// Parse raw sensor parameters into a system-level profile
SensorProfile SensorParser::parseParameters(const RawSensorParameters& raw) {
    SensorProfile profile;

    profile.capacity      = raw.capacity;
    profile.securityLevel = raw.securityLevel;
    profile.baudRate      = raw.baudRate;
    profile.isReady       = parseReadyFlag(raw.statusReg);

    return profile;
}

// Parse raw scan result into normalized status
ParsedScanResult SensorParser::parseScanResult(const RawMatchResult& raw) {
    ParsedScanResult result;
    result.id = raw.id;
    result.confidence = raw.confidence;

    switch (raw.status) {
        case FINGERPRINT_OK:
            result.status = ScanStatus::OK;
            break;

        case FINGERPRINT_NOFINGER:
            result.status = ScanStatus::NO_FINGER;
            break;

        case FINGERPRINT_PACKETRECIEVEERR:
            result.status = ScanStatus::COMM_ERROR;
            break;

        case FINGERPRINT_IMAGEFAIL:
        case FINGERPRINT_IMAGEMESS:
        case FINGERPRINT_FEATUREFAIL:
        case FINGERPRINT_INVALIDIMAGE:
            result.status = ScanStatus::IMAGE_ERROR;
            break;

        default:
            result.status = ScanStatus::UNKNOWN_ERROR;
            break;
    }

    return result;
}

// Interpret sensor status register
bool SensorParser::parseReadyFlag(uint16_t statusReg) {
    // Example: bit 0 == sensor ready (based on datasheet behavior)
    return (statusReg & 0x0001) == 0;
}
