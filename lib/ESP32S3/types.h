#ifndef ESP32S3_TYPES_H
#define ESP32S3_TYPES_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

struct CanSetupResult {
    int transmit_task_id;
    int dispatch_task_id;
};

struct HardwareSetupResult {};

struct SetupResult {
    HardwareSetupResult hardware_setup_result;
    CanSetupResult can_setup_result;
};

#endif // ESP32S3_TYPES_H
