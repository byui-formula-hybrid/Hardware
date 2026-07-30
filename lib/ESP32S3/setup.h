#ifndef ESP32S3_SETUP_H
#define ESP32S3_SETUP_H

#include <core_can.h>
#include <core/queue.h>

#include "twai_service.h"
#include "twai_types.h"

#include "FreeRTOS_queue.h"
#include "FreeRTOS_thread_strategy.h"

#include <core/task.h>

#include "types.h"

SetupResult setup();
HardwareSetupResult hardware_setup();
CanSetupResult can_setup();

#endif // ESP32S3_SETUP_H
