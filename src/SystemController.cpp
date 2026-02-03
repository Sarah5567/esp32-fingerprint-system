#include "SystemController.h"

SystemController::SystemController(FingerprintService& service)
    : service_(service), state_(SystemState::IDLE) {}

void SystemController::update() {
    switch (state_) {
        case SystemState::IDLE:
            handleIdle();
            break;

        case SystemState::VALIDATING:
            handleValidating();
            break;

        case SystemState::AUTHORIZED:
            handleAuthorized();
            break;

        case SystemState::ERROR:
            handleLocked();
            break;
    }
}

SystemState SystemController::getState() const {
    return state_;
}

// Handlers

void SystemController::handleIdle() {
    // Do not start auth if locked
    if (service_.isLocked()) {
        state_ = SystemState::ERROR;
        return;
    }

    state_ = SystemState::VALIDATING;
}

void SystemController::handleValidating() {
    BiometricResult result = service_.authenticate();

    switch (result.result) {
        case AuthResult::AUTHORIZED:
            state_ = SystemState::AUTHORIZED;
            break;

        case AuthResult::LOCKED:
            state_ = SystemState::ERROR;
            break;

        default:
            state_ = SystemState::IDLE;
            break;
    }
}

void SystemController::handleAuthorized() {
    // One-shot success
    state_ = SystemState::IDLE;
}

void SystemController::handleLocked() {
    // Stay locked until service unlocks
    if (!service_.isLocked()) {
        state_ = SystemState::IDLE;
    }
}
