#ifndef FREERTOS_TASK_CONTROLLER_H
#define FREERTOS_TASK_CONTROLLER_H

#include "freertos/FreeRTOS.h"

#include <task.h>
#include <logger.h>

class FreeRTOSTaskController : public Core::TaskController<TaskHandle_t, TaskFunction_t> {
public:
    TaskHandle_t create_task(TaskFunction_t task) override;
    bool delete_task(TaskHandle_t identifier) override;
};

#endif // FREERTOS_TASK_CONTROLLER_H