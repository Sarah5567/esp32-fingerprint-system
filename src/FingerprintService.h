#pragma once

#include <cstdint>
#include <vector>

#include "FingerprintSensor.h"
#include "SensorParser.h"

enum class AuthResult {
    AUTHORIZED,
    UNAUTHORIZED,
    LOW_CONFIDENCE,
    LOCKED,
    ERROR
};

struct BiometricResult {
    AuthResult result;
    int id;
    int confidence;
};

class FingerprintService {
public:
    explicit FingerprintService(FingerprintSensor& sensor);

    bool init();
    BiometricResult authenticate();

    // lockout query
    bool isLocked() const;
    uint32_t lockUntil() const;

private:
    bool isAuthorizedId(int id) const;
    void registerFailure();
    void resetFailures();

private:
    FingerprintSensor& sensor_;

    int failedAttempts_;
    uint32_t lockUntilMs_;

    static constexpr int MAX_ATTEMPTS = 3;
    static constexpr int MIN_CONFIDENCE = 50;
    static constexpr uint32_t LOCK_DURATION_MS = 2 * 60 * 1000; // 2 minutes

    std::vector<int> authorizedIds_;
};
