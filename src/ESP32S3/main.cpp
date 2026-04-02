#include "main.h"

#if defined(ENV_ESP32)

void app_main() {
    can_setup();
}

#endif // ENV_ESP32