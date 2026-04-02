#ifndef ESP32S3_SETUP_H
#define ESP32S3_SETUP_H

#include <can.h>
#include <core/queue/FREERTOS/FREERTOS_queue.h>

#include "types.h"

SetupResult setup();
void hardware_setup();
CanSetupResult can_setup();

#endif // ESP32S3_SETUP_H 