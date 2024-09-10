#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// platform constant acceleration in m/s (1 in/s)
#define PLATFORM_ACCELERATION 0.0245

// hard-coded code
#define CODE_INITIAL_VALUE 1234

// main SPI handle of type SPI_HandleTypeDef
#define SPI_HANDLE hspi1

// TIM_HandleTypeDef, configured to 100ms
#define TIMER_HANDLE htim7

// TIM_HandleTypeDef, configured to 100ms (only enabled when moving up or down)
#define TIMER_DEPTH_HANDLE htim6

// TIMER_DEPTH_HANDLE: get interval in seconds
#define TIMER_DEPTH_INTERVAL 0.1

// ADC handle of type ADC_HandleTypeDef
#define ADC_HANDLE hadc1
// number of ADC conversions per (item count)
#define ADC_NCONV 2

#define ADC_IDX_X 0
#define ADC_IDX_Y 1

// joystick x-y axis bounds
#define JOYSTICK_X_MIN 922
#define JOYSTICK_X_MAX 2709
#define JOYSTICK_Y_MIN 922
#define JOYSTICK_Y_MAX 2622
// tolerance before update
#define JOYSTICK_TOLERANCE 150

#endif
