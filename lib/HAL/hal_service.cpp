#if defined(ENV_STM32)

#include "hal_service.h"

#include <core_can.h>

using namespace Core;

bool HALService::setup(const void* config) {
    MPU_Config();
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART3_UART_Init();
    printf("STM32 Device started!\n");

    // CAN will fail to initialize if a transceiver is not connected.
    printf("CAN initializing!\n");
    MX_CAN1_Init();
    printf("CAN initialized!\n");
    // MX_CAN2_Init();
    //MX_USB_OTG_FS_PCD_Init();

    return true;
}

bool HALService::tear_down() {
    // FIXME: Not sure if we need anything in here yet
}

bool HALService::can_send() {
    // If this is 0 then the mailboxes are full, otherwise we're good to send
    return HAL_CAN_GetTxMailboxesFreeLevel(&m_hcan);
}

bool HALService::send(const Frame& frame) {
    // TODO: rearrange Frame so that we can grab the header directly
    // TODO: get rid of hardcoded can header
    // Testing purposes only
    CAN_TxHeaderTypeDef header;
    header.IDE = CAN_ID_STD;
    header.StdId = 0x01;
    header.DLC = 0x8;

    if(HAL_CAN_AddTxMessage(&m_hcan, &header, frame.data, &m_txMailbox)) {
        Error_Handler();
    }
};

const void HALService::read() {

    uint8_t data[8];
    CAN_RxHeaderTypeDef header;
    if(HAL_CAN_GetRxMessage(&m_hcan, CAN_RX_FIFO0, &header, data)) {
        Error_Handler();
    }

    m_dispatcher->enqueue(Frame(header.StdId, data));
}

bool HALService::recover() {
    Error_Handler();
}


#endif
