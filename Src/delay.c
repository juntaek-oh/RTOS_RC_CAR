
#include "delay.h"




void delay_us1(uint16_t us)
{

  __HAL_TIM_SET_COUNTER(&htim1, 0);  // set을 시킬땐 호출하면 바로 set 시켜서 여러개 호출하지 말고
  while((__HAL_TIM_GET_COUNTER(&htim1)) < us); // 가져 오는걸 여러개 가져오자.
}


void delay_us2(uint16_t us)
{

  __HAL_TIM_SET_COUNTER(&htim2, 0);  // set을 시킬땐 호출하면 바로 set 시켜서 여러개 호출하지 말고
  while((__HAL_TIM_GET_COUNTER(&htim2)) < us); // 가져 오는걸 여러개 가져오자.
}


void delay_us4(uint16_t us)
{

  __HAL_TIM_SET_COUNTER(&htim4, 0);  // set을 시킬땐 호출하면 바로 set 시켜서 여러개 호출하지 말고
  while((__HAL_TIM_GET_COUNTER(&htim4)) < us); // 가져 오는걸 여러개 가져오자.
}
