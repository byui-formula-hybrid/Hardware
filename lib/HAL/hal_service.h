#ifndef HAL_SERVICE_H
#define HAL_SERVICE_H

#include <core/logger.h>
#include <core_can.h>

#include <stm32f7xx_common.h>

#include <stm32f7defs.h>

using namespace CAN;

/**
 * @brief The FreeRTOS implementation for the CAN Service
 */
class HALService : public Service {
private:
    CAN_HandleTypeDef m_hcan;
    uint32_t m_txMailbox;
public:
    HALService() = default;

    /**
     * @brief Sets up the FreeRTOS Service
     *
     * @param config Configuration data for the service
     * @return true if setup was successful
     */
    bool setup(const void* config) override;

    /**
     * @brief Clean up anything that should be cleaned up before the service dies
     *
     * @return true if tear down was successful
     *
     * TODO: Should this just be the deconstructor?
     */
    bool tear_down() override;

    /**
     * @brief Returns true if it is possible to add a CAN Frame to the HAL CAN queue
     */
    bool can_send() override;

    /**
     * @brief Adds CAN Frame to the HAL CAN queue to be sent on the CAN Bus
     *
     * @return True if successfully queue frame into HAL CAN Queue
     */
    bool send(const Frame& frame) override;

    /**
     * @brief Gets most recent recieved CAN Frame and queues it in the dispatcher
     */
    const void read() override;

    /**
     * @brief Recovers the service into a usable state in case of error
     *
     * TODO: Needs implementation
     */
    bool recover() override;

    /**
     * @brief Start listening on the CAN Bus
     *
     * TODO: Implement this
     */
    void start_listening() override {}

    /**
     * @brief Stop listening on the CAN Bus
     *
     * TODO: Implement this
     */
    void stop_listening() override {}

    ~HALService() = default;
};


#endif
