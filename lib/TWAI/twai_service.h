#ifndef TWAI_SERVICE_H
#define TWAI_SERVICE_H

#include <driver/twai.h>
#include <driver/gpio.h>
#include <core/logger.h>

#include <core_can.h>
#include "twai_types.h"

using namespace CAN;

class TWAIService : public Service {
public:
    bool setup(const void * config) override;
    bool tear_down() override;
    bool send(const Frame& frame) override;
    const void read() override;
    bool recover() override;

    ~TWAIService() = default;

    void stop_listening() override {}
    void start_listening() override {}
    bool can_send() override;
private:
    esp_err_t get_status(twai_status_info_t& status);
};

#endif // TWAI_SERVICE_H
