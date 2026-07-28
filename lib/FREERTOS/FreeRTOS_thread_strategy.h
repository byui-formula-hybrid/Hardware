#ifndef FREERTOS_THREAD_STRATEGY_H
#define FREERTOS_THREAD_STRATEGY_H

#include "core/thread.h"

#include <FreeRTOS.h>
#include <task.h>

#define DEFAULT_STACK_SIZE 2048

/**
 * @brief The FreeRTOS Implementation for iThreadStrategy
 *
 * Typically used by the ThreadController
 */
class FreeRTOSThread : public Core::iThreadStrategy {
public:
    /**
     * @brief Sets up the class with all of the needed information to start a thread
     *
     * @param name The name for the FreeRTOS task
     * @param priority The Priority to give the FreeRTOS task
     * @param attributes Any Attributes the FreeRTOS task may need
     */
    void setup(const char* name, const uint32_t priority, const uint32_t attributes) override;

    /**
     * @brief Creates and starts the thread
     *
     * @param task The function to create the task with
     * @param argument The data to be passed into the task
     *
     * @return -1 if the task was not successfully
     */
    uint32_t create(taskFunc task, void* argument) override;

    /**
     * @brief kills the thread when we are done with it
     *
     * TODO: Should we just make this the deconstructor?
     */
    void kill() override;

    /**
     * @brief Make the task sleep
     */
    void sleep(const uint32_t millis) override;
private:
    const char* m_name;
    uint32_t m_priority;
    uint32_t m_attributes;
    uint32_t m_task_number;

    TaskHandle_t m_task_handle;
};

#endif
