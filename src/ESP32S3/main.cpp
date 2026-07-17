#if defined(ENV_ESP32)

#include "main.h"

void app_main() {
    setup();
    run();
    shutdown();
    return;
}

#endif // ENV_ESP32