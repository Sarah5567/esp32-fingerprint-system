#include "FingerprintService.h"
#include <Arduino.h>

FingerprintService::FingerprintService(FingerprintSensor& sensor)
    : sensor_(sensor),
      failedAttempts_(0),
      lockUntilMs_(0) {}

BiometricResult FingerprintService::authenticate() {
    // Check lockout (logic only)
    if (isLocked()) {
        return {AuthResult::LOCKED, -1, 0};
    }

    RawMatchResult raw = sensor_.scan();
    ParsedScanResult parsed = SensorParser::parseScanResult(raw);

    // 🔹 No finger detected - NOT a failure, just waiting for input
    if (parsed.status == ScanStatus::NO_FINGER) {
        return {AuthResult::NO_INPUT, -1, 0};
    }

    // 🔹 Any other non-OK status IS a failure (communication, image errors, etc)
    if (parsed.status != ScanStatus::OK) {
        registerFailure();
        return {AuthResult::ERROR, -1, 0};
    }

    if (parsed.confidence < MIN_CONFIDENCE) {
        registerFailure();
        return {AuthResult::LOW_CONFIDENCE, parsed.id, parsed.confidence};
    }

    if (!isAuthorizedId(parsed.id)) {
        registerFailure();
        return {AuthResult::UNAUTHORIZED, parsed.id, parsed.confidence};
    }

    // 🔹 Success resets failures
    resetFailures();
    return {AuthResult::AUTHORIZED, parsed.id, parsed.confidence};
}

void FingerprintService::registerFailure() {
    failedAttempts_++;

    if (failedAttempts_ >= MAX_ATTEMPTS) {
        lockUntilMs_ = millis() + LOCK_DURATION_MS;
    }
}

void FingerprintService::resetFailures() {
    failedAttempts_ = 0;
    lockUntilMs_ = 0;
}

bool FingerprintService::isLocked() const {
    return millis() < lockUntilMs_;
}

uint32_t FingerprintService::lockUntil() const {
    return lockUntilMs_;
}
