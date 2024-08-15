#ifndef _PINS_H
#define _PINS_H

// LoRa device SPI handler name of type SPI_HandleTypeDef
#define SPI_HANDLE hspi1

// LoRa NSS
#define LORA_NSS_PORT GPIOB
#define LORA_NSS_PIN GPIO_PIN_12

// LoRa DIO0
#define LORA_DIO0_PORT GPIOB
#define LORA_DIO0_PIN GPIO_PIN_1

// LoRa reset
#define LORA_RESET_PORT GPIOB
#define LORA_RESET_PIN GPIO_PIN_0

// stepper motor pins
#define STEPPER_PORT GPIOC
#define STEPPER_IN1 GPIO_PIN_0
#define STEPPER_IN2 GPIO_PIN_1
#define STEPPER_IN3 GPIO_PIN_2
#define STEPPER_IN4 GPIO_PIN_3

// ballast stepper motor: descend position (absolute step count from hover position)
#define BALLAST_DESCEND_POSITION (200 * 1024)

// ballast stepper motor: ascend position (absolute step count from hover position)
#define BALLAST_ASCEND_POSITION (200 * 1024)

// dc motor input pins
#define DC_MOTOR_PORT GPIOA
#define DC_MOTOR_IN1 GPIO_PIN_0
#define DC_MOTOR_IN2 GPIO_PIN_1
#define DC_MOTOR_IN3 GPIO_PIN_2
#define DC_MOTOR_IN4 GPIO_PIN_3

// dc motor enable pins
#define DC_MOTOR_EN1_2 GPIO_PIN_6
#define DC_MOTOR_EN3_4 GPIO_PIN_7

// dc motor enable pins: PWM channels
#define DC_MOTOR_EN1_2_PWM_CHANNEL 1
#define DC_MOTOR_EN3_4_PWM_CHANNEL 2

// electrogamnet pin
#define ELECTROMAGNET_PORT GPIOB
#define ELECTROMAGNET_PIN GPIO_PIN_3

// TIM_HandleTypeDef, configured to 2s
#define TIMER_ELECTROMAGNET_HANDLE htim6

// TIM_HandleTypeDef, configured to 5ms
#define TIMER_STEPPER_HANDLE htim7

// TIM_HandleTypeDef, used for PWM
#define TIMER_PWM_HANDLE htim3

#endif
