#ifndef MAIN_H
#define MAIN_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "setup.h"
#include "run.h"
#include "shutdown.h"

#ifdef __cplusplus
extern "C" {
#endif

void app_main();

#ifdef __cplusplus
}
#endif // extern "C"

#endif // MAIN_H