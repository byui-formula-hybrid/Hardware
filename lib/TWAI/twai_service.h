#ifndef TWAI_SERVICE_H
#define TWAI_SERVICE_H

#include <driver/twai.h>
#include <driver/gpio.h>
#include <logger.h>

#include "service.h"
#include "types.h"

using namespace CAN;

class TWAIService : public Service {
public:
    bool setup(const void * config) override;
    bool tear_down() override;
    bool send(const Frame& frame) override;
    const Frame* read() override;
    bool recover() override;

    ~TWAIService() = default;
};

#endif // TWAI_SERVICE_H