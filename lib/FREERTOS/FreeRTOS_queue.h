#ifndef FREERTOS_QUEUE_H
#define FREERTOS_QUEUE_H

#include <core/queue.h>
#include <core/logger.h>

#include "FreeRTOS.h"
#include "projdefs.h"
#include "queue.h"

template<typename T>
class FreeRTOSQueue: public Core::IQueue<T> {
public:
    FreeRTOSQueue(size_t capacity) {
        this->capacity = capacity;
        this->queue_handle = xQueueCreate(capacity, sizeof(T));
    }

    /**
     * @brief Copies the given data in the queue
     *
     * @param data The data to be copied into the queue
     * @return true if successful copy of data into queue
     *
     * NOTE: Do not call from interrupts
     */
    bool enqueue(const T& data) override {
        BaseType_t result = xQueueSend(queue_handle, data, portMAX_DELAY);
        if (result != pdPASS) {
            printf("FreeRTOSQueue: Failed to enqueue data: %d\n", result);
            return false;
        }
        return true;
    }

    /**
     * @brief This is just like our normal enqueue function except for use from interrupts
     *
     * @param data The data to be copied into the queue
     * @return true if successful copy of data into queue
     */
    bool enqueueFromISR(const T data) override {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        // Use the ISR-safe FreeRTOS API
        BaseType_t result = xQueueSendFromISR(queue_handle, &data, &xHigherPriorityTaskWoken);
        bool success = (result == pdPASS);

        if (!success) {
            // TODO: Keep track of failed messages here
        } else {
            // Call this to make sure we give priority back to high priority tasks
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }

        return success;
    }

    /**
     * @brief Copies the top item in the queue into a given object and deletes the object
     *
     * @param data A reference to an object for the queue data to be copied into
     * @param timeout_ms How long to try adding to the queue
     *
     * @return true if successfully dequeued item
     * NOTE: Not for use in interrupts
     */
    bool dequeue(T& data, uint32_t timeout_ms = 0) override {
        TickType_t ticks_to_wait = pdMS_TO_TICKS(timeout_ms);
        BaseType_t result = xQueueReceive(queue_handle, &data, ticks_to_wait);
        bool success = result == pdPASS;
        if (!success) {
            //printf("Failed to dequeue data: %d\n", result);
        }
        return success;
    }

    /**
     * @brief Returns the number of items in the queue
     */
    size_t size() const override {
        return uxQueueMessagesWaiting(queue_handle);
    }

    /**
     * @brief Returns if the queue is full
     */
    bool is_full() override {
        return uxQueueSpacesAvailable(queue_handle) == 0;
    }

private:
    QueueHandle_t queue_handle;
    size_t capacity;
};

#endif // FREERTOS_QUEUE_H
