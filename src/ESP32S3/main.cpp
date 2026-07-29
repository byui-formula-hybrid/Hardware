/**
 * @file main.cpp
 * @brief ESP32 Main File
 */

#if defined(ENV_ESP32)

#include "main.h"

/**
 * @brief ESP32 Entry Function
 */
void app_main() {
    setup();
    run();
    shutdown();
    return;
}

#endif // ENV_ESP32
