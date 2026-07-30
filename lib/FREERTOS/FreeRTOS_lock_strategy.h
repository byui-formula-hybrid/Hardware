#ifndef FREERTOS_LOCK_STRATEGY_H
#define FREERTOS_LOCK_STRATEGY_H

#include <core/lock/i_lock_strategy.h>

#if defined(ENV_ESP32)
#include <freertos/FreeRTOS.h>
#else
#include <FreeRTOS.h>
#endif

#include <semphr.h>

/**
 * @brief FreeRTOS locking strategy for microcontroller use
 */
class FreeRTOSLockStrategy : public Core::iLockStrategy {
public:

    FreeRTOSLockStrategy() {}

    ~FreeRTOSLockStrategy() override {
        xSemaphoreGive(handle);
        vSemaphoreDelete(handle);
    }

    void lock() override {
        if(xSemaphoreTake(handle, pdMS_TO_TICKS(0)) != pdPASS) {
            printf("Unable to acquire the lock!");
        }
    }

    void unlock() override {
        xSemaphoreGive(handle);
    }

private:
    SemaphoreHandle_t handle;
};

#endif
