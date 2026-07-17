#ifndef FREERTOS_THREAD_STRATEGY_H
#define FREERTOS_THREAD_STRATEGY_H

#include "core/thread.h"

#include <FreeRTOS.h>
#include <task.h>

#define DEFAULT_STACK_SIZE 2048

class FreeRTOSThread : public Core::iThreadStrategy {
public:
    void setup(const char* name, const uint32_t priority, const uint32_t attributes) override;
    uint32_t create(taskFunc task, void* argument) override;
    void kill();
    void sleep(const uint32_t millis);
private:
    const char* m_name;
    uint32_t m_priority;
    uint32_t m_attributes;
    uint32_t m_task_number;

    TaskHandle_t m_task_handle;
};

#endif