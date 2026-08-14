#pragma once

#include "driver/gpio.h"

#define BSP_BLACK   0
#define BSP_WHITE   1

#define BSP_VERSION BSP_BLACK

#if BSP_VERSION == BSP_BLACK

#define DC_PIN      GPIO_NUM_12
#define RES_PIN     GPIO_NUM_13
#define CS_PIN      GPIO_NUM_11
#define SCLK_PIN    GPIO_NUM_10
#define SDIN_PIN    GPIO_NUM_9

#elif BSP_VERSION == BSP_WHITE

#define DC_PIN      GPIO_NUM_38
#define RES_PIN     GPIO_NUM_45
#define CS_PIN      GPIO_NUM_39
#define SCLK_PIN    GPIO_NUM_21
#define SDIN_PIN    GPIO_NUM_47
#define TE_PIN      GPIO_NUM_40

#endif