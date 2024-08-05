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
#include <stdlib.h> 

double sinArr [628]; // I believe it is necessary to make this global. Otherwise it breaks in weird ways

double sineFreq(int omega, int i);

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

  uint16_t adc_res, mask = 0xff00;
  char message[100];
  
  //  ADC example
	/*
  HAL_ADC_Start(&hadc3);
  HAL_ADC_PollForConversion(&hadc3, 100);
  adc_res = HAL_ADC_GetValue(&hadc3);
  sprintf(message, "adc_res=%d\r\n", adc_res);
  print_msg(message);
  
  // DAC example
  HAL_DAC_Init(&hdac);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 255);
	*/
	
	// generate sine array (sin(0.00) -- sin(6.27))
	//double sineArr [628];
	
	int i = 0;
	for (i = 0; i < 628; i++) {
			sinArr[i] = sin(i * 0.01);
			//HAL_Delay(10);
			sprintf(message, "%d = %f\n", i,  sinArr[i]);
      print_msg(message);
  }
	
	sprintf(message, "Done gen\n");
	print_msg(message);

  while (1)
  {
		/* //For Part 1/2:
		
		// Get the value from the generated wave
		HAL_ADC_Start(&hadc3);
		HAL_ADC_PollForConversion(&hadc3, 100);
		adc_res = HAL_ADC_GetValue(&hadc3);
		
		//sprintf(message, "adc_res=%d\r\n", adc_res);
		//print_msg(message);
		
		//Set the value for the new wave
		adc_res = adc_res & mask_bits;
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, adc_res);
		*/
		double fourierTerm;
    for (i = 0; i < 628; i++) {
			//HAL_Delay(1);
			fourierTerm = ((sinArr[i] + sinArr[(i*3)%628]/3 + sinArr[(i*5)%628]/5) + 1) * 1000;
			fourierTerm *= 1.273; // normalization
			//fourierTerm = floor(fourierTerm*100)/100;
      //sprintf(message, "%d = %f\n", i, fourierTerm);
      //print_msg(message);
			//HAL_Delay(100);
			HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, fourierTerm);
    }
    
  }
 }

double sineFreq(int omega, int i) {
    int term = omega * i;
    term=term%628;
    return sinArr[term];
}
