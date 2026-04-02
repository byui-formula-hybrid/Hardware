#include "FREERTOS_task_controller.h"

TaskHandle_t FreeRTOSTaskController::create_task(TaskFunction_t task) {
    BaseType_t xReturned;
    TaskHandle_t xHandle = nullptr;

    xReturned = xTaskCreate(task, "Task", STACK_SIZE, void, tskIDLE_PRIORITY, &xHandle);

    if (xReturned != pdPASS) {
        LOG_ERR("FreeRTOSTaskController", "Failed to create task: %d", xReturned);
        vTaskDelete(identifier);
        return nullptr;
    }

    // TODO: Validate this will not crash
    task_identifiers.push_back(xHandle);

    return xHandle;
}

bool FreeRTOSTaskController::delete_task(TaskHandle_t identifier) {
    vTaskDelete(identifier);
    // TODO: This is O(n) and could be optimized with a different data structure if needed
    // TODO: Validate this will not crash
    task_identifiers.erase(std::remove(task_identifiers.begin(), task_identifiers.end(), identifier), task_identifiers.end());
    return true;
}

