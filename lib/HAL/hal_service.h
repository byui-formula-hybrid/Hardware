#ifndef HAL_SERVICE_H
#define HAL_SERVICE_H

#include <core/logger.h>
#include <core_can.h>

#include <stm32f7xx_common.h>

#include <stm32f7defs.h>

using namespace CAN;

class HALService : public Service {
private:
    CAN_HandleTypeDef m_hcan;
    uint32_t m_txMailbox;
public:
    HALService() = default;
    bool setup(const void* config) override;
    bool tear_down() override;
    bool can_send() override;
    bool send(const Frame& frame) override;
    const Frame* read() override;
    bool recover() override;

    void start_listening() override {}
    void stop_listening() override {}

    ~HALService() = default;
};


#endif
