/*
 * tpad.c
 *  Created on: Mar 22, 2020
 *      Author: Du
 */
#include "../BSP/TPAD/tpad.h"

#define TPAD_ARR_MAX_VAL  0XFFFFFFFF

extern TIM_HandleTypeDef htim2;

uint16_t tpad_default_val = 0;


void tpad_Reset (void)
{
  GPIO_InitTypeDef GPIO_Initure;

  GPIO_Initure.Pin = GPIO_PIN_5;
  GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Initure.Pull = GPIO_PULLDOWN;
  GPIO_Initure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init (GPIOA, &GPIO_Initure);

  HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
  HAL_Delay (5);
  __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC1|TIM_FLAG_UPDATE);
  __HAL_TIM_SET_COUNTER(&htim2, 0);

  GPIO_Initure.Mode = GPIO_MODE_AF_PP;
  GPIO_Initure.Pull = GPIO_NOPULL;
  GPIO_Initure.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init (GPIOA, &GPIO_Initure);
}


uint8_t tpad_init (void)
{
  uint16_t buf[10];
  uint16_t temp;
  uint8_t j, i;

  HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_1);

  for (i = 0; i < 10; i++)
  {
    buf[i] = tpad_get_value ();
    HAL_Delay (10);
  }
  for (i = 0; i < 9; i++) //排序
  {
    for (j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j]) //升序排列
      {
	temp = buf[i];
	buf[i] = buf[j];
	buf[j] = temp;
      }
    }
  }
  temp = 0;
  for (i = 2; i < 8; i++)
    temp += buf[i];
  tpad_default_val = temp / 6;
  printf ("tpad_default_val:%d\r\n", tpad_default_val);
  if (tpad_default_val > TPAD_ARR_MAX_VAL / 2)
    return 1;
  return 0;
}

uint16_t tpad_get_value (void)
{
  tpad_Reset ();
  while (__HAL_TIM_GET_FLAG(&htim2,TIM_FLAG_CC1) == RESET)
  {
    if (__HAL_TIM_GET_COUNTER(&htim2) > TPAD_ARR_MAX_VAL - 500)
    {
      return __HAL_TIM_GetCounter(&htim2);
    }
  }
  return HAL_TIM_ReadCapturedValue (&htim2, TIM_CHANNEL_1);
}

uint16_t TPAD_Get_MaxVal (uint8_t n)
{
  uint16_t temp = 0;
  uint16_t res = 0;
  uint8_t lcntnum = n * 2 / 3; //至少2/3*n的有效个触摸,才算有效
  uint8_t okcnt = 0;

  while (n--)
  {
    temp = tpad_get_value (); //得到一次值
    if (temp > (tpad_default_val * 5 / 4))
      okcnt++; //至少大于默认值的5/4才算有效
    if (temp > res)
      res = temp;
  }
  if (okcnt >= lcntnum)
    return res; //至少2/3的概率,要大于默认值的5/4才算有效
  else
    return 0;
}


uint8_t TPAD_Scan(uint8_t mode)
{
  static uint8_t keyen=0;	//0,可以开始检测;>0,还不能开始检测
  uint8_t res=0;
  uint8_t sample=3;	//默认采样次数为3次
  uint16_t rval;
  if(mode)
  {
    sample=6;	//支持连按的时候，设置采样次数为6次
    keyen=0;	//支持连按
  }
  rval=TPAD_Get_MaxVal(sample);
  if(rval>(tpad_default_val*4/3)&&rval<(10*tpad_default_val))//大于tpad_default_val+(1/3)*tpad_default_val,且小于10倍tpad_default_val,则有效
  {
    if(keyen==0)res=1;	//keyen==0,有效
    printf("r:%d\r\n",rval);
    keyen=3;				//至少要再过3次之后才能按键有效
  }
  if(keyen)keyen--;
  return res;
}



