#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(ENV_STM32)

#if defined(ENV_STM32F7)

#include "stm32f7defs.h"

#endif // ENV_STM32F7

// TODO: Comment
void Error_Handler(void);

#endif // ENV_STM32

#ifdef __cplusplus
}
#endif // extern "C"

#endif // MAIN_H