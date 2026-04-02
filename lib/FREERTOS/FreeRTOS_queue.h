#ifndef FREERTOS_QUEUE_H
#define FREERTOS_QUEUE_H

#include <core/queue.h>
#include <core/logger.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

template<typename T>
class FreeRTOSQueue: public IQueue<T> {
public:
    FreeRTOSQueue(size_t capacity) {
        this->capacity = capacity;
        this->queue_handle = xQueueCreate(capacity, sizeof(T));
    }

    bool enqueue(const T& data) override {
        BaseType_t result = xQueueSend(queue_handle, &data, portMAX_DELAY);
        bool success = result == pdPASS;
        if (!success) {
            LOG_ERR("FreeRTOSQueue", "Failed to enqueue data: %d", result);
        }
        return success;
    }

    bool dequeue(T& data, uint32_t timeout_ms = 0) override {
        TickType_t ticks_to_wait = (timeout_ms == 0) ? 0 : pdMS_TO_TICKS(timeout_ms);
        BaseType_t result = xQueueReceive(queue_handle, &data, ticks_to_wait);
        bool success = result == pdPASS;
        if (!success) {
            LOG_ERR("FreeRTOSQueue", "Failed to dequeue data: %d", result);
        }
        return success;
}

    size_t size() const override {
        return uxQueueMessagesWaiting(queue_handle);
    }

    bool is_full() const override {
        return uxQueueSpacesAvailable(queue_handle) == 0;
    }

private:
    QueueHandle_t queue_handle;
    size_t capacity;
};

#endif // FREERTOS_QUEUE_H