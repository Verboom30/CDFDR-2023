#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f2xx_hal.h"
#ifdef __cplusplus
extern "C" {
#endif


void Error_Handler(void);
/*
 ****************  tim  ****************
 */
extern TIM_HandleTypeDef htim1;


void MX_TIM1_Init(void);
/**TIM1 GPIO Configuration
PB0     ------> TIM1_CH2N
PB1     ------> TIM1_CH3N
PB13     ------> TIM1_CH1N
PA8     ------> TIM1_CH1
PA9     ------> TIM1_CH2
PA10     ------> TIM1_CH3
*/


void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */