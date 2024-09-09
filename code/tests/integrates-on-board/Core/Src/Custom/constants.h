#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// LoRa device SPI handler name of type SPI_HandleTypeDef
#define SPI_HANDLE hspi2

// ballast stepper motor: descend position (absolute step count from hover position)
#define BALLAST_DESCEND_POSITION 200

// ballast stepper motor: ascend position (absolute step count from hover position)
#define BALLAST_ASCEND_POSITION 200

// TIM_HandleTypeDef, configured to 2s
#define TIMER_ELECTROMAGNET_HANDLE htim6

// TIM_HandleTypeDef, configured to <5ms
#define TIMER_STEPPER_HANDLE htim7

// TIM_HandleTypeDef, used for PWM
#define TIMER_PWM_HANDLE htim2

#endif
