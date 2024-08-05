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
#include "ov7670.h"

#include <stdio.h>
#include <string.h>



uint16_t snapshot_buff[IMG_ROWS * IMG_COLS];
uint8_t print_buffer[IMG_COLS];// * IMG_ROWS

uint8_t dma_flag = 0;
int takePic = 1;
int i = 0;
uint8_t pixel;

void print_buf(void);

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
	
/*	uint8_t val = ov7670_read(0x34);
	sprintf(msg, "Here: %x\n", val); 
	print_msg(msg);*/

  // Your start up code here
	for(i = 0; i < IMG_ROWS * IMG_COLS; i++){
		snapshot_buff[i] = 0xff00;
	}
	print_buf();

  /*while (1)
  {
    // Your code here
		if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin)) {
      HAL_Delay(100);  // debounce
			ov7670_snapshot(snapshot_buff);
    }
		
		if(dma_flag){
			print_buf();
			dma_flag = 0;
		} 
  }*/
	
	while(1){
		
		if(takePic){
			ov7670_capture(snapshot_buff);
			takePic = 0;
		}
		
		if(dma_flag){	
			//HAL_DCMI_Suspend(&hdcmi);
			HAL_DCMI_Stop(&hdcmi);
			print_buf();
			dma_flag = 0;
			takePic = 1;
			//HAL_DCMI_Resume(&hdcmi);
			HAL_Delay(10);
		}	
	}
	
}


void print_buf() {
	print_msg("\r\nPREAMBLE!\r\n");
	
	/*for(i = 0; i < IMG_ROWS * IMG_COLS; i++){
		pixel = snapshot_buff[i] >> 8;		
		uart_send_bin(&pixel, 1);
	}*/
	
	for(i = 0; i < IMG_ROWS; i++){
		for(int j = 0; j < IMG_COLS; j++){
			print_buffer[j] = snapshot_buff[i*IMG_COLS + j] >> 8;
		}	
		uart_send_bin(print_buffer, IMG_COLS);
	}
	
	/*for(i = 0; i < IMG_ROWS * IMG_COLS; i++){
		print_buffer[i] = snapshot_buff[i] >> 8;		
	}
	
	uart_send_bin(print_buffer, IMG_ROWS * IMG_COLS);*/
}
