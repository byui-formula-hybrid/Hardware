#ifndef FREERTOS_TASK_CONTROLLER_H
#define FREERTOS_TASK_CONTROLLER_H

// TODO: import FreeRTOS
#include <task.h>
#include <logger.h>

namespace Core {

    class FreeRTOSTaskController : public TaskController<TaskHandle_t, TaskFunction_t> {
    public:
        TaskHandle_t create_task(TaskFunction_t task) override;
        bool delete_task(TaskHandle_t identifier) override;
    };

} // namespace Core

#endif // FREERTOS_TASK_CONTROLLER_H