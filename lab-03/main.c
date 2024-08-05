/* USER CODE BEGIN Header */
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "config.h"
#include "fixedpt.h"
#include <string.h>

/**
  * @brief  The application entry point.
  * @retval int
  */
	
#define CHANGE_RATE 100

double sinArr [628]; // It is necessary to make this global. Local varl is on stack, which is just weird
fixedpt sinArrFixed [628];
int numberTerms = 1;

double sineFreq(int omega, int i);
fixedpt sineFreqFixed(int omega, int i);

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC_Init();
  MX_TIM6_Init(); // go to this function to tweak pre-scalar
  MX_USART3_Init();
	
	char message[100];
	
	HAL_TIM_Base_Start(&htim6);
	int startTime, endTime, timeElapsed;
	
	
	// sine lut generation
	int i = 0, j = 0;
	for (i = 0; i < 628; i++) {
			sinArr[i] = sin((double)i * 0.01); // add 1 offset
			sinArrFixed [i] = FXD_FROM_FLOAT(sinArr[i]);
  }
	sprintf(message, "Done gen\n");
	print_msg(message);
	
	int counter = 0;
	
  while (1)
  {		
		double fourierTerm;
		fixedpt fourierTermFixed;
		//startTime = __HAL_TIM_GET_COUNTER(&htim6);
		
    for (i = 0; i < 628; i++) {
			
			//if(counter < CHANGE_RATE){
				//LUT STATE
				fourierTerm = 0;
				
				for(j = 1; j < numberTerms*2; j+=2) {
					fourierTerm += sineFreq(j, i) / j; //sinArr[ ((j)*i)%628 ]/(j);
				}
				
				//fourierTerm = sinArr[i] + (sineFreq(3, i) / 3) + (sineFreq(5, i) / 5) + (sineFreq(7,i)) / 7 + sineFreq(9,i) / 9 + sineFreq(11,i) / 11;
				fourierTerm *= 1.273; // normalize to amplitude of 1
				fourierTerm += 1.5; // shift up by 1.5V so it's all positive
				fourierTerm *= 1241; // DAC map 0-4096 to 0-3.3V, so multiply by 4096/3.3 = 1241
				
				//sprintf(message, "%d(fl) = %.0f\n", i, fourierTerm);
				//print_msg(message);
				//HAL_Delay(50);
				
				HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, fourierTerm);
				
			//} 
			/*
				// else if (counter < CHANGE_RATE*2){
				//FIXED STATE
				fourierTermFixed = FXD_FROM_INT(0);
				
				for(j = 1; j < numberTerms*2; j+=2) {
					fourierTermFixed += FXD_DIV(sineFreqFixed(j, i), FXD_FROM_INT(j));
				}
				
				//fourierTermFixed = sinArrFixed[i] + FXD_DIV(sineFreqFixed(3, i), FXD_FROM_INT(3)) 
							//+ FXD_DIV(sineFreqFixed(5, i), FXD_FROM_INT(5)) + FXD_DIV(sineFreqFixed(7,i), FXD_FROM_INT(7))
							//+ FXD_DIV(sineFreqFixed(9,i), FXD_FROM_INT(9)) + FXD_DIV(sineFreqFixed(11,i), FXD_FROM_INT(11));
				fourierTermFixed = FXD_MUL(fourierTermFixed, FXD_FROM_FLOAT(1.273));
				fourierTermFixed = FXD_ADD(fourierTermFixed, FXD_FROM_FLOAT(1.5));
				// from this step forward, it's not longer a fixed, but an int
				fourierTermFixed = (int)(1241 * FXD_TO_FLOAT(fourierTermFixed));
				
				//sprintf(message, "%d(fi) = %d\n", i, fourierTermFixed);
				//print_msg(message);
				//HAL_Delay(50);
				
				HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, fourierTermFixed);
				
			/*} else {
				//SINE STATE
				fourierTerm = 0;
				for(j = 1; j < numberTerms*2; j+=2) {
					fourierTerm += sin((double)i * 0.01 * j) / j; //sinArr[ ((j)*i)%628 ]/(j);
				}
				//fourierTerm = sin((double)i * 0.01) + sin((double)i * 0.01 * 3) / 3 + sin((double)i * 0.01 * 5) / 5
																														//									+ sin((double)i * 0.01 * 7) / 7;
				fourierTerm *= 1.273; // normalize to amplitude of 1
				fourierTerm += 1.5; // shift up by 1.5V so it's all positive
				fourierTerm *= 1241; // DAC map 0-4096 to 0-3.3V, so multiply by 4096/3.3 = 1241
				
				//sprintf(message, "%d(fl) = %.0f\n", i, fourierTerm);
				//print_msg(message);
				//HAL_Delay(50);
				
				HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, fourierTerm);
			}		*/	
    }
		
		counter = (counter+1)%(CHANGE_RATE*3);
		
		//endTime = __HAL_TIM_GET_COUNTER(&htim6);
		//timeElapsed = endTime - startTime;
		//sprintf(message, "time = %d - %d = %d\n", endTime, startTime, timeElapsed);
		//print_msg(message);
		//HAL_Delay(1000);
  }
	// for some reason, that last else if adds 200 cycles for everything.
	// without printing, float cost 2699*100 cycles, fixed takes 437 * 100 cycles, sine takes 29458 * 100 cycles
	
}

double sineFreq(int omega, int i) {
	int term = omega * i;
	term = term % 628;
	return sinArr[term];
}

fixedpt sineFreqFixed(int omega, int i) {
	int term = omega * i;
	term = term % 628;
	return sinArrFixed[term];
}
