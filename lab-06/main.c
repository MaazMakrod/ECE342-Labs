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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "ov7670.h"


/* USER CODE BEGIN PV */
#define PREAMBLE "\r\n!START!\r\n"
#define DELTA_PREAMBLE "\r\n!DELTA!\r\n"
#define SUFFIX "!END!\r\n"

uint16_t snapshot_buff[IMG_ROWS * IMG_COLS];
//uint8_t old_snapshot_buff[(IMG_ROWS * IMG_COLS)/ 2];
uint8_t old_snapshot_buff[IMG_ROWS * IMG_COLS];
uint8_t delta_buff[IMG_ROWS * IMG_COLS];

uint8_t dma_flag = 0;
int i, j;
int step = 1;

int main(void)
{
  /* Reset of all peripherals */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_DCMI_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM6_Init();
  
  char msg[100];
  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  
  ov7670_init();
  HAL_Delay(100);
	
	int currPos;
	uint8_t currNum, count;
	uint8_t N = 1;
	
	ov7670_capture(snapshot_buff);
	
	//Part 3 
	/*while(1){		
		if(HAL_UART_GetState(&huart3) == HAL_UART_STATE_READY){
				switch(step){
					case 1:
						if(dma_flag){
							//HAL_DCMI_Suspend(&hdcmi);
							//HAL_DCMI_Stop(&hdcmi);
							HAL_Delay(10);							
							for(i = 0; i < IMG_ROWS * IMG_COLS; i++){
								old_snapshot_buff[i] = snapshot_buff[i] >> 8;		
							}
							HAL_Delay(10);
							//HAL_DCMI_Resume(&hdcmi);
							ov7670_capture(snapshot_buff);
							
							step++;
							dma_flag = 0;			
							
							print_msg(PREAMBLE);												
						}
						break;
					case 2:
						uart_send_bin(old_snapshot_buff, IMG_COLS*IMG_ROWS);
						step++;
						break;
					case 3:
						print_msg(SUFFIX);
						step=1;
						break;
				}		
		}
	}*/
	
	//4.2
	/*while(1){		
		if(HAL_UART_GetState(&huart3) == HAL_UART_STATE_READY){
				switch(step){
					case 1:
						if(dma_flag){
							dma_flag = 0;				

							//HAL_DCMI_Suspend(&hdcmi);
							for(i = 0; i < (IMG_ROWS * IMG_COLS)/2; i++){
								old_snapshot_buff[i] = ((snapshot_buff[2*i] >> 8) & 0xf0) | (snapshot_buff[2*i + 1] >> 12);		
							}
							//HAL_DCMI_Resume(&hdcmi);
							ov7670_capture(snapshot_buff);							
							
							print_msg(PREAMBLE);			
							step++;
						}
						break;
					case 2:
						uart_send_bin(old_snapshot_buff, (IMG_COLS*IMG_ROWS)/2);
						step++;
						break;
					case 3:
						print_msg(SUFFIX);
						step=1;
						break;
				}		
		}
	}*/
	
	//4.3 
	//We can do the same computation on the spot
	//Need local var for count, current MSB 4, and current poisiton in the old snap shot buff
	//Outer for loop iterates through snapshot buff
	
	/*while(1){		
		if(HAL_UART_GetState(&huart3) == HAL_UART_STATE_READY){
				switch(step){
					case 1:
						if(dma_flag){
							//HAL_DCMI_Suspend(&hdcmi);
							dma_flag = 0;	
							currNum = snapshot_buff[0] >> 12;
							currPos = 0;
							count = 0;							

							for(int i = 0; i < IMG_ROWS*IMG_COLS; i++){
								if (count == 15) {
										old_snapshot_buff[currPos] = (currNum << 4) | count;
										currPos++;
										currNum = snapshot_buff[i] >> 12;
										count = 0;
								}
								if(currNum == snapshot_buff[i] >> 12) {
										count++;
								}
								else {
										old_snapshot_buff[currPos] = (currNum << 4) | count;
										currPos++;
										currNum = snapshot_buff[i] >> 12;
										count = 1;
								}
							}
							old_snapshot_buff[currPos] = (currNum << 4) | count;
						
							//HAL_DCMI_Resume(&hdcmi);
							ov7670_capture(snapshot_buff);
							print_msg(PREAMBLE);
			
							step++;
						}
						break;
					case 2:
						uart_send_bin(old_snapshot_buff, currPos + 1);
						step++;
						break;
					case 3:
						print_msg(SUFFIX);
						step=1;
						break;
				}		
		}
	}*/
	
	//Part 5
	while(1){		
		if(HAL_UART_GetState(&huart3) == HAL_UART_STATE_READY){
				switch(step){
					case 1:
						if(dma_flag){
							
							//HAL_DCMI_Suspend(&hdcmi);
							//HAL_DCMI_Stop(&hdcmi);
							dma_flag = 0;	
							currPos = 0;
							count = 0;		

							if(N == 1){ //New Frame
								currNum = snapshot_buff[0] >> 12;
								
								for(i = 0; i < IMG_ROWS*IMG_COLS; i++){								
									if (count == 15) {
											delta_buff[currPos] = (currNum << 4) | count;
											currPos++;
											currNum = snapshot_buff[i] >> 12;
											count = 0;
									}
									if(currNum == snapshot_buff[i] >> 12) {
											count++;
									}
									else {
											delta_buff[currPos] = (currNum << 4) | count;
											currPos++;
											currNum = snapshot_buff[i] >> 12;
											count = 1;
									}
									
									old_snapshot_buff[i] = snapshot_buff[i] >> 12;
								}
								delta_buff[currPos] = (currNum << 4) | count;
								
							} else {// Delta
								currNum = (snapshot_buff[0] >> 12) - old_snapshot_buff[0];
								
								for(i = 0; i < IMG_ROWS*IMG_COLS; i++){
									if (count == 15) {
											delta_buff[currPos] = (currNum << 4) | count;
											currPos++;
											currNum = (snapshot_buff[i] >> 12) - old_snapshot_buff[i];
											count = 0;
									}
									if(currNum == ( (snapshot_buff[i] >> 12) - old_snapshot_buff[i] )) {
											count++;
									}
									else {
											delta_buff[currPos] = (currNum << 4) | count;
											currPos++;
											currNum = (snapshot_buff[i] >> 12) - old_snapshot_buff[i];
											count = 1;
									}
									
									old_snapshot_buff[i] = snapshot_buff[i] >> 12;
								}
								
								delta_buff[currPos] = (currNum << 4) | count;
							}
						
							//HAL_DCMI_Resume(&hdcmi);
							ov7670_capture(snapshot_buff);
							
							if(N ==  1)
								print_msg(PREAMBLE);
							else
								print_msg(DELTA_PREAMBLE);
							
							
							N = (N%6)+1;							
							step++;
						}
						break;
					case 2:						
						uart_send_bin(delta_buff, currPos +1);						
						step++;
						break;
					case 3:
						print_msg(SUFFIX);
						step=1;
						break;
				}		
		}
	}
}
