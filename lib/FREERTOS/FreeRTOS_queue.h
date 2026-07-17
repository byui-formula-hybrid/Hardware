#ifndef FREERTOS_QUEUE_H
#define FREERTOS_QUEUE_H

#include <core/queue.h>
#include <core/logger.h>

#include "FreeRTOS.h"
#include "queue.h"

template<typename T>
class FreeRTOSQueue: public Core::IQueue<T> {
public:
    FreeRTOSQueue(size_t capacity) {
        this->capacity = capacity;
        this->queue_handle = xQueueCreate(capacity, sizeof(T));
    }

    // Store the actual data here so that we can keep the heap clean
    bool enqueue(const T data) override {
        BaseType_t result = xQueueSend(queue_handle, &data, portMAX_DELAY);
        bool success = result == pdPASS;
        if (!success) {
            //LOG_ERR("FreeRTOSQueue", "Failed to enqueue data: %d", result);
            printf("FreeRTOSQueue: Failed to enqueue data: %d\n", result);
        }
        return success;
    }

    // Enqueue designed for being called from interrupts
    bool enqueueFromISR(const T data) override {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
        // Use the ISR-safe FreeRTOS API
        BaseType_t result = xQueueSendFromISR(queue_handle, &data, &xHigherPriorityTaskWoken);
        bool success = (result == pdPASS);
        
        if (!success) {
            // REMINDER: Do not use printf() in an ISR!
            // isr_enqueue_error_count++; 
        } else {
            // If sending to the queue woke a higher priority task, request a context switch.
            // On modern FreeRTOS ports (like ARM Cortex-M or ESP32), it is perfectly 
            // safe to call this macro here rather than at the very end of the ISR, 
            // as it simply sets a hardware interrupt flag (like PendSV) to defer the switch.
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
        
        return success;
    }

    // A copy of the data should be made on the stack so we can clear the queue
    // and avoid heap fragmentation
    bool dequeue(T& data, uint32_t timeout_ms = 0) override {
        TickType_t ticks_to_wait = (timeout_ms == 0) ? 0 : pdMS_TO_TICKS(timeout_ms);
        BaseType_t result = xQueueReceive(queue_handle, &data, ticks_to_wait);
        bool success = result == pdPASS;
        if (!success) {
            //printf("Failed to dequeue data: %d\n", result);
        }
        return success;
    }

    size_t size() const override {
        return uxQueueMessagesWaiting(queue_handle);
    }

    bool is_full() override {
        return uxQueueSpacesAvailable(queue_handle) == 0;
    }

private:
    QueueHandle_t queue_handle;
    size_t capacity;
};

#endif // FREERTOS_QUEUE_H