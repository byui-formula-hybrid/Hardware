#include "FREERTOS_queue.h"

FreeRTOSQueue(size_t capacity) {
    queue_handle = xQueueCreate(capacity, sizeof(T));
}

bool FreeRTOSQueue::enqueue(const T& data) {
    BaseType_t result = xQueueSend(queue_handle, &data, portMAX_DELAY);
    bool success = result == pdPASS;
    if (!success) {
        LOG_ERR("FreeRTOSQueue", "Failed to enqueue data: %d", result);
    }
    return success;
}

bool FreeRTOSQueue::dequeue(T& data, uint32_t timeout_ms) {
    TickType_t ticks_to_wait = (timeout_ms == 0) ? 0 : pdMS_TO_TICKS(timeout_ms);
    BaseType_t result = xQueueReceive(queue_handle, &data, ticks_to_wait);
    bool success = result == pdPASS;
    if (!success) {
        LOG_ERR("FreeRTOSQueue", "Failed to dequeue data: %d", result);
    }
    return success;
}