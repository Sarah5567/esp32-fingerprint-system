#pragma once

enum class SystemState {
    IDLE,        // waiting for user interaction
    SCANNING,    // fingerprint scan in progress
    VALIDATING,  // authentication logic running
    AUTHORIZED,  // successful authentication
    ERROR        // error or locked state
};
