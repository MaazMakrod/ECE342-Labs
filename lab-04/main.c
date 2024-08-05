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

#include "dfr0151.h"
#include "config.h"

#include <stdio.h>
#include <string.h>

int setTime = FALSE;
int saveTime = FALSE;

int buttonClicked = FALSE;
uint8_t dataBuffer[7];

int main(void)
{
  /* Reset of all peripherals. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();  
  rtc_init();

  char msg[100];
	uint8_t hour, min, sec, week_day, day, month, year;
	uint8_t alarm_hour, alarm_min, alarm_sec, alarm_week_day, alarm_day, alarm_month, alarm_year;

  // Your start up code here
	
	//Set the initial time
	eeprom_read(0, dataBuffer);
	rtc_set_date(dataBuffer[3], dataBuffer[2], dataBuffer[1], dataBuffer[0]);
	rtc_set_time(dataBuffer[4], dataBuffer[5], dataBuffer[6]);
	
	//Set the alarm time
	eeprom_read(7, dataBuffer);
	alarm_year = dataBuffer[0];
	alarm_month = dataBuffer[1];
	alarm_week_day = dataBuffer[2];
	alarm_day = dataBuffer[3];
	alarm_hour = dataBuffer[4];
	alarm_min = dataBuffer[5];
	alarm_sec = dataBuffer[6];
	
	
	/*rtc_write(0, dataBuffer[6]);
	rtc_write(1, dataBuffer[5]);
	rtc_write(2, dataBuffer[4]);
	rtc_write(3, dataBuffer[3]);
	rtc_write(4, dataBuffer[2]);
	rtc_write(5, dataBuffer[1]);
	rtc_write(6, bin2bcd(dataBuffer[0]));*/

  while (1)
  {
		/*
    // Your code here
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    HAL_Delay(500);
		sprintf(msg, "tick\n");
    print_msg(msg);

    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    HAL_Delay(500);
		sprintf(msg, "tock\n");
    print_msg(msg);
		*/
		
		/* if(setTime){
			setTime = FALSE;
			rtc_set_date(7, 16, 10, 95);
			rtc_set_time(6, 30, 20);
		} */
		
		rtc_get_date(&week_day, &day, &month, &year);
		rtc_get_time(&hour, &min, &sec);
		
		sprintf(msg, "%d:%d:%d:%d - %d:%d:%d |---| %d:%d:%d:%d - %d:%d:%d\n", \
		   year, month, week_day, day, hour, min, sec, \
		   alarm_year, alarm_month, alarm_week_day, alarm_day, alarm_hour, alarm_min, alarm_sec);				
    print_msg(msg);		
		
		if(year == alarm_year && month == alarm_month && week_day == alarm_week_day && \
				day == alarm_day && hour == alarm_hour && min == alarm_min && sec == alarm_sec){
				
				for(int i = 0; i < 10; i++){
					HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
					HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
					HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
					print_msg("ALARM\n");
					HAL_Delay(1000);
				}
		}
		
		HAL_Delay(1000);
		
		/*if(saveTime) {
			saveTime = FALSE;
			//uint8_t dataBuffer[7] = {bin2bcd(year), bin2bcd(month), bin2bcd(day), bin2bcd(week_day), bin2bcd(hour), bin2bcd(min), bin2bcd(sec)};
			uint8_t dataBuffer[7] = {year, month, day, week_day, hour, min, sec};
			eeprom_write(0, dataBuffer, 7);
			print_msg("Saved Time\n");
		}*/
		
		if(buttonClicked){
			
			//Set time on startup July 1 2023, 10 AM
			dataBuffer[0] = 23;
			dataBuffer[1] = 7;
			dataBuffer[2] = 1;
			dataBuffer[3] = 1;
			dataBuffer[4] = 11;
			dataBuffer[5] = 30;
			dataBuffer[6] = 0;
			
			eeprom_write(0, dataBuffer, 7);
			HAL_Delay(10);//HAL_Delay since eeprom can't be written to twice within 10ms
			
			//Set Alarm time on eeprom
			dataBuffer[6] += 5;
			eeprom_write(7, dataBuffer, 7);
			HAL_Delay(10);
			
			//Set date on clock
			rtc_set_date(dataBuffer[3], dataBuffer[2], dataBuffer[1], dataBuffer[0]);
			rtc_set_time(dataBuffer[4], dataBuffer[5], dataBuffer[6]);		

			buttonClicked = FALSE;
			
			print_msg("Saved Time\n");
		}		
  }
}
