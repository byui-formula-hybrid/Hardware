#include "setup.h"

SetupResult setup() {
    return {
        hardware_setup(),
        can_setup()
    }
}

HardwareSetupResult hardware_setup() {
    return {};
}

CanSetupResult can_setup() {
    static TWAIService can_service;
    TWAIServiceConfig can_config = {
        .general_config = &twai_general_config,
        .timing_config = &twai_timing_config,
        .filter_config = &twai_filter_config
    };
    can_service.setup(&can_config);

    FreeRTOSQueue<Frame> rx_queue(100);
    CAN::Dispatcher dispatcher = CAN::Dispatcher::get_instance();
    dispatcher.set_queue(&rx_queue);

    FreeRTOSQueue<Frame> tx_queue(100);
    CAN::Transmitter transmitter = CAN::Transmitter::get_instance();
    transmitter.set_queue(&tx_queue);
    transmitter.set_service(&can_service);

    FreeRTOSTaskController controller = FreeRTOSTaskController::get_instance();
    controller.set_max_tasks(2);
    TaskHandle_t transmit_task_id = controller.create_task([](void*){
        while (true) {
            transmitter.transmit();
        }
    });
    TaskHandle_t dispatch_task_id = controller.create_task([](void*){
        while (true) {
            dispatcher.dispatch();
        }
    });

    return {
        transmit_task_id,
        dispatch_task_id
    }
}
