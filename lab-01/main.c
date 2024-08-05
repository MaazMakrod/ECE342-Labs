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
#include "main.h"
#include "config.h"
#include <stdio.h>


int8_t current_row = -1, current_col = -1;

void printButton(void);

int main(void)
{
  /* Reset of all peripherals. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();

  char message[100];
  sprintf(message, "Printing test");
  print_msg(message);

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  
  /* Initialize ROW outputs */
  HAL_GPIO_WritePin(ROW0_GPIO_Port, ROW0_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_RESET);

  /* Infinite loop */
  while (1)
  {
		HAL_GPIO_WritePin(ROW0_GPIO_Port, ROW0_Pin, GPIO_PIN_SET);
		current_row = 0;
		HAL_Delay(10);
		printButton();
		
		HAL_GPIO_WritePin(ROW0_GPIO_Port, ROW0_Pin, GPIO_PIN_RESET);
		
		HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_SET);
		current_row = 1;
		HAL_Delay(10);
		printButton();
		HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_RESET);
		
		HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_SET);
		current_row = 2;
		HAL_Delay(10);
		printButton();
		HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_RESET);
		
		HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_SET);
		current_row = 3;
		HAL_Delay(10);
		printButton();
		HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_RESET);
		
		HAL_Delay(200); // for debouncing
		
  }
}

void printButton(void) {
	char numPad [4][4] = { {'3', '2', '1', '0'},
													{'7', '6', '5', '4'},
													{'B', 'A', '9', '8'},
													{'F', 'E', 'D', 'C'} 
												};
	char pressed = '0';
	if (current_col != -1) {
			pressed = numPad[current_row][current_col];
			char buttonMsg [100];
			sprintf(buttonMsg, "Pressed %c\n", pressed);
			print_msg(buttonMsg);
			
			// halt until user release key
			while(HAL_GPIO_ReadPin(COL0_GPIO_Port, COL0_Pin) || HAL_GPIO_ReadPin(COL1_GPIO_Port, COL1_Pin)||
							HAL_GPIO_ReadPin(COL2_GPIO_Port, COL2_Pin)|| HAL_GPIO_ReadPin(COL3_GPIO_Port, COL3_Pin)){
				HAL_Delay(100);
				//print_msg("delaying");
			}
			HAL_Delay(100);
			current_col = -1;
	}
	
}
