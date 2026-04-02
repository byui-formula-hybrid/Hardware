#include "main.h"

#if defined(ENV_ESP32)

void app_main() {
    setup();
    run();
    shutdown();
    return;
}

#endif // ENV_ESP32