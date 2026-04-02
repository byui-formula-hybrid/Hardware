#ifndef TWAI_TYPES_H
#define TWAI_TYPES_H

#include <driver/twai.h>

struct TWAIConfig: public BaseServiceConfig {
    const twai_general_config_t *general_config;
    const twai_timing_config_t *timing_config;
    const twai_filter_config_t *filter_config;
};

#endif // TWAI_TYPES_H