#include "FreeRTOS_thread_strategy.h"

#include <core/logger.h>

void FreeRTOSThread::setup(const char* name, const uint32_t priority, const uint32_t attributes) {
    m_name = name;
    m_priority = priority;
    m_attributes = attributes;

    printf("Setting task name to %s\n", name);
}

uint32_t FreeRTOSThread::create(taskFunc task, void* argument) {
    m_handle = xTaskCreate(task, m_name, DEFAULT_STACK_SIZE, argument, m_priority, &m_task_handle);

    if(m_handle != pdPASS) {
        LOG_ERR("FreeRTOSThread", "Failed to create thread: %d", m_handle);
        vTaskDelete(m_task_handle);
        return -1;
    }

    return m_handle;
}

void FreeRTOSThread::kill() {
    vTaskDelete(m_task_handle);
}

void FreeRTOSThread::sleep(const uint32_t millis) {
    vTaskDelay(pdMS_TO_TICKS(millis));
}