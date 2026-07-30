#include "setup.h"
#include "can/types.h"
#include "core/task/controller.h"
#include "driver/twai.h"

// 1. General Config: TX pin 21, RX pin 22, Normal mode
twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21, GPIO_NUM_20, TWAI_MODE_NORMAL);

// 2. Timing Config: 500 kbps
twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();

// 3. Filter Config: Accept all CAN IDs
twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

SetupResult setup() {
    return {
        hardware_setup(),
        can_setup()
    };
}

HardwareSetupResult hardware_setup() {
    return {};
}

CanSetupResult can_setup() {
    static TWAIService can_service;
    TWAIConfig can_config;
    can_config.general_config = &g_config;
    can_config.timing_config = &t_config;
    can_config.filter_config = &f_config;

    can_service.setup(&can_config);

    FreeRTOSQueue<CAN::Frame> rx_queue(100);
    CAN::Dispatcher* dispatcher = CAN::Dispatcher::get_instance();
    dispatcher->set_queue(&rx_queue);

    FreeRTOSQueue<CAN::Frame> tx_queue(100);
    CAN::Transmitter* transmitter = CAN::Transmitter::get_instance();
    transmitter->set_queue(&tx_queue);
    transmitter->set_service(&can_service);

    Core::TaskController* controller = Core::TaskController::get_instance();
    controller->set_max_tasks(2);
    int transmit_task_id = controller->create_task(new FreeRTOSThread(), Transmitter::transmit, transmitter);
    int dispatch_task_id = controller->create_task(new FreeRTOSThread(), Dispatcher::dispatch, dispatcher);

    return {
        transmit_task_id,
        dispatch_task_id
    };
}
