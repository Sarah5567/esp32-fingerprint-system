#pragma once

#include "SystemState.h"
#include "FingerprintService.h"

class SystemController {
public:
    explicit SystemController(FingerprintService& service);

    void update();
    SystemState getState() const;

private:
    void handleIdle();
    void handleValidating();
    void handleAuthorized();
    void handleLocked();

private:
    FingerprintService& service_;
    SystemState state_;
};
