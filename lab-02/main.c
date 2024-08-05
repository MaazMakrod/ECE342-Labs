/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "main.h"
#include "config.h"

#include <stdio.h>
#include <math.h>

double sineArr [628];
char message[100];
int i;

int main(void)
{
  /* Reset of all peripherals. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC3_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_DAC_Init();
	HAL_DAC_Init(&hdac);

  uint16_t adc_res, mask = 0xFFFF;
	
  while (1)
  {
		//For Part 1/2:
		
		//  Get the value from the generated wave
		HAL_ADC_Start(&hadc3);
		HAL_ADC_PollForConversion(&hadc3, 100);
		adc_res = HAL_ADC_GetValue(&hadc3);
		
		//sprintf(message, "adc_res = %d\n", adc_res);
		//print_msg(message);
		
		//HAL_Delay(50);
		//Set the value for the new wave
		//adc_res = adc_res & mask;
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, adc_res);
		//HAL_Delay(100);
  }
 }
