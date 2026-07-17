#include "main.h"

#if defined(ENV_STM32)

#include "hal_service.h"
#include "FreeRTOS_thread_strategy.h"
#include "FreeRTOS_queue.h"
#include <core/task.h>
#include <FreeRTOS_lock_strategy.h>

#include <inverter/DTIX50.h>

// Services
static HALService s_service;
static Dispatcher* s_dispatcher;
static Transmitter* s_transmitter;
static Core::TaskController* s_taskController;

// Heartbeat(s)
static Inverter::DTIX50::Heartbeat* s_inverter_heart;

// Other
static bool running = false;

void test_loop(void* data) {
    while(true) {
        HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
        vTaskDelay(pdMS_TO_TICKS(20));
        HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
        HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
        vTaskDelay(pdMS_TO_TICKS(20));
        HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
        HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
        vTaskDelay(pdMS_TO_TICKS(20));
        HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
    }
}

/**
 * @brief All transmitters should be initialized here to keep our main function clean
 * 
 * This should always be called after the dispatch and transmit threads have been activated
 */
bool start_transmitters() {
    std::unique_ptr<Core::iLockStrategy> inverter_lock(new FreeRTOSLockStrategy());

    s_inverter_heart = new Inverter::DTIX50::Heartbeat(s_transmitter, std::move(inverter_lock));

    s_inverter_heart->start(new FreeRTOSThread());

    return true;
};


int main(void) {
    s_service.setup(nullptr);

    s_dispatcher = Dispatcher::get_instance();
    s_transmitter = Transmitter::get_instance();
    s_taskController = Core::TaskController::get_instance();

    s_dispatcher->set_queue(new FreeRTOSQueue<CAN::Frame>(10));
    s_transmitter->set_queue(new FreeRTOSQueue<CAN::Frame>(10));
    s_transmitter->set_service(&s_service);

    printf("Pre tasks\n");

    s_taskController->setup_task("Dispatcher", 1, 0);
    int dispatch_id = s_taskController->create_task(new FreeRTOSThread(), Dispatcher::dispatch, s_dispatcher);
    s_taskController->setup_task("Transmitter", 1, 0);
    int transmit_id = s_taskController->create_task(new FreeRTOSThread(), Transmitter::transmit, s_transmitter);

    // Enable the interrupt now that we have our CAN systems running
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING)) {
        printf("Failed to activate interrupt for hcan1\n");
        // attempt to initialize hcan2 instead
        Error_Handler();
    }

    s_taskController->setup_task("Test_loop", 1, 0);
    int test_id = s_taskController->create_task(new FreeRTOSThread(), test_loop, nullptr);

    vTaskStartScheduler();

    running = true;

    start_transmitters();

    while(running){}

    s_inverter_heart->stop();

    // Wait for the heartbeat to stop
    while(s_inverter_heart->started()) {}

    s_taskController->delete_task(test_id);
    s_taskController->delete_task(transmit_id);
    s_taskController->delete_task(dispatch_id);

    //NOTE: Currently nothing happens in this function
    s_service.tear_down();
}

// Extern C so that the HAL driver can see the Callback
extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];

    //printf("Interrupt, probably will crash!\n");

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) != HAL_OK) {
        //printf("CAN_RX ERROR: Failed to get message!");
        Error_Handler();
    }

    uint32_t id = header.IDE ? header.ExtId : header.StdId;

    s_dispatcher->enqueue(Frame(id, data));
}

#endif // ENV_STM32