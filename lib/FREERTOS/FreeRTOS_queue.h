#ifndef FREERTOS_QUEUE_H
#define FREERTOS_QUEUE_H

#include <core/queue.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class FreeRTOSQueue: public IQueue {
public:
    FreeRTOSQueue(size_t capacity);
    bool enqueue(const T& data) override;
    bool dequeue(T& data, uint32_t timeout_ms = 0) override;

private:
    QueueHandle_t queue_handle;
};

#endif // FREERTOS_QUEUE_H