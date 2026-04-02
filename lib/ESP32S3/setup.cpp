#include "setup.h"

void can_setup() {
    static TWAIService can_service;
    can_service.setup(nullptr);
    // TODO: Setup queues and link them to the dispatcher and transmitter
    // CAN::Dispatcher::get_instance().set_queue();
    // CAN::Transmitter::get_instance().set_queue();
    CAN::Transmitter::get_instance().set_service(&can_service);

    // TODO: Use something else to manage tasks. This is just temporary for testing
    // Create the dispatch Task
    xTaskCreate([](void*){
        while(true) { CAN::Dispatcher::get_instance().dispatch(); }
    }, "can_rx", 4096, nullptr, 10, nullptr);

    // Create the transmit Task
    xTaskCreate([](void*){
        while(true) { CAN::Transmitter::get_instance().transmit(); }
    }, "can_tx", 4096, nullptr, 9, nullptr);
}
