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

// This should probably be more accessible
static bool running = false; // Current just always true in main after initialization

#ifndef PROD
// Flashes the USER LEDs on the Nucleo board for non-production builds
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
#endif

/**
 * @brief All transmit classes should be initialized here to keep our main function clean
 *
 * This should always be called after the dispatch and transmit threads have been activated
 */
bool start_transmit_classes() {
    std::unique_ptr<Core::iLockStrategy> inverter_lock(new FreeRTOSLockStrategy());

    s_inverter_heart = new Inverter::DTIX50::Heartbeat(s_transmitter, std::move(inverter_lock));

    s_inverter_heart->start(new FreeRTOSThread());

    return true;
};


/**
 * @brief The STM32 Entry. This is responsible for initialization of threads
 */
int main(void) {
    s_service.setup(nullptr);

// Create global static instances of singletons
    s_dispatcher = Dispatcher::get_instance();
    s_transmitter = Transmitter::get_instance();
    s_taskController = Core::TaskController::get_instance();

// Set up various necessary items for the singletons
    s_dispatcher->set_queue(new FreeRTOSQueue<CAN::Frame>(10));
    s_transmitter->set_queue(new FreeRTOSQueue<CAN::Frame>(10));
    s_transmitter->set_service(&s_service);

// Create the tasks for Dispatch and Transmit loops
    s_taskController->setup_task("Dispatcher", 1, 0);
    int dispatch_id = s_taskController->create_task(new FreeRTOSThread(), Dispatcher::dispatch, s_dispatcher);
    s_taskController->setup_task("Transmitter", 1, 0);
    int transmit_id = s_taskController->create_task(new FreeRTOSThread(), Transmitter::transmit, s_transmitter);

    // CAN Interrupts have to be activated after the dispatcher
    // TODO: This should be in Service::start_listening
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING)) {
        printf("Failed to activate interrupt for hcan1\n");
        Error_Handler();
    }

#ifndef PROD
    s_taskController->setup_task("Test_loop", 1, 0);
    int test_id = s_taskController->create_task(new FreeRTOSThread(), test_loop, nullptr);
#endif

    vTaskStartScheduler();

    running = start_transmit_classes();

    if(!running) {
        printf("Failed to start transmitters");
    }

    while(running){}

    s_inverter_heart->stop();

    // Wait for the heartbeat to stop
    while(s_inverter_heart->started()) {}

#ifndef PROD
    s_taskController->delete_task(test_id);
#endif
    s_taskController->delete_task(transmit_id);
    s_taskController->delete_task(dispatch_id);

    //NOTE: Currently nothing happens in this function
    s_service.tear_down();
}

/**
 * @brief Callback function for when CAN messages are recieved
 *
 * Extern C to fulfill the weak definition in the HAL Driver
 * NOTE: This is an interrupt so we can't use normal logging or enqueue functions
 * TODO: Call the Service Read Function instead or Point the interrupt to that function
 */
extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) != HAL_OK) {
        //printf("CAN_RX ERROR: Failed to get message!");
        Error_Handler();
    }

    uint32_t id = (header.IDE == CAN_ID_EXT) ? header.ExtId : header.StdId;

    s_dispatcher->enqueue(Frame(id, data));
}

#endif // ENV_STM32
