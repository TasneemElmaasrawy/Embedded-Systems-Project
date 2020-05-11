#include "main.h"

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);

uint8_t hexToAscii(uint8_t n)//4-bit hex value converted to an ascii character
{
if (n>=0 && n<=9) n = n + '0';
else n = n - 10 + 'A';
return n;
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();

	
	// I2c buffers 
 	uint8_t second [2], minute [2], hour [2];					
	uint8_t	day [2], date [2], month [2], year [2];
	
	//receiving charachter 
  char receive_c='\0';
	char receive_c2='\0';
	
	second[0] = 0x00; //register address
	second[1] = 0x00; //data 
	HAL_I2C_Master_Transmit(&hi2c1, 0xD0, second, 2, 10);

	minute[0] = 0x01; //register address
	minute[1] = 0x00; //data 
	HAL_I2C_Master_Transmit(&hi2c1, 0xD0, minute, 2, 10);

	hour[0] = 0x02; //register address
	hour[1] = 0x21; //data 
  HAL_I2C_Master_Transmit(&hi2c1, 0xD0, hour, 2, 10);
    
  day[0] = 0x03; //register address
  day[1] = 0x03; //data 
  HAL_I2C_Master_Transmit(&hi2c1, 0xD0, day, 2, 10);

  date[0] = 0x04; //register address
  date[1] = 0x11; //data 
  HAL_I2C_Master_Transmit(&hi2c1, 0xD0, date, 2, 10);

  month[0] = 0x05; //register address
  month[1] = 0x05; //data 
  HAL_I2C_Master_Transmit(&hi2c1, 0xD0, month, 2, 10);

  year[0] = 0x06; //register address
  year[1] = 0x20; //data 
  HAL_I2C_Master_Transmit(&hi2c1, 0xD0, year, 2, 10);
	
	
	// date/time output buffer  format: day date month year-hour:minute:second
	uint8_t date_time[] = {0,0,0,' ',0,0,' ',0,0,' ',0,0,'-',0,0,':',0,0,':',0,0,'\r','\n'};  
		
	//output format for date and time
	while (1)
	{
	
		//receive seconds
		HAL_I2C_Master_Transmit(&hi2c1, 0xD0, second, 1, 10);
		HAL_I2C_Master_Receive(&hi2c1, 0xD1, second+1, 1, 10);
		//store in the buffer
		date_time[19] = hexToAscii(second[1] >> 4 );
		date_time[20] = hexToAscii(second[1] & 0x0F );
        
		//receive minutes
		HAL_I2C_Master_Transmit(&hi2c1, 0xD0, minute, 1, 10);
		HAL_I2C_Master_Receive(&hi2c1, 0xD1, minute+1, 1, 10);
		//store in the buffer
		date_time[16] = hexToAscii(minute[1] >> 4 );
		date_time[17] = hexToAscii(minute[1] & 0x0F );

    //receive hour
		HAL_I2C_Master_Transmit(&hi2c1, 0xD0, hour, 1, 10);
		HAL_I2C_Master_Receive(&hi2c1, 0xD1, hour+1, 1, 10);
		//store in the buffer
		date_time[13] = hexToAscii(hour[1] >> 4 );
		date_time[14] = hexToAscii(hour[1] & 0x0F);
        
    //receive year
    HAL_I2C_Master_Transmit(&hi2c1, 0xD0, year, 1, 10);
    HAL_I2C_Master_Receive(&hi2c1, 0xD1, year+1, 1, 10);
    //store in the buffer
    date_time[10] = hexToAscii(year[1] >> 4 );
    date_time[11] = hexToAscii(year[1] & 0x0F);
    
		//receive month
    HAL_I2C_Master_Transmit(&hi2c1, 0xD0, month, 1, 10);
    HAL_I2C_Master_Receive(&hi2c1, 0xD1, month+1, 1, 10);
    //store in the buffer
    date_time[7] = hexToAscii(month[1] >> 4 );
    date_time[8] = hexToAscii(month[1] & 0x0F);
        
		//receive day date 		
    HAL_I2C_Master_Transmit(&hi2c1, 0xD0, date, 1, 10);
    HAL_I2C_Master_Receive(&hi2c1, 0xD1, date+1, 1, 10);
    //store in the buffer
    date_time[4] = hexToAscii(date[1] >> 4 );
    date_time[5] = hexToAscii(date[1] & 0x0F);
    
		//receive week day 
    HAL_I2C_Master_Transmit(&hi2c1, 0xD0, day, 1, 10);
    HAL_I2C_Master_Receive(&hi2c1, 0xD1, day+1, 1, 10);
    
		//day output
		switch(day[1])
		{
			case 0x01:		//saturday
				date_time [0] = 'S';
				date_time [1] = 'A';
				date_time [2] = 'T';
				break;
			case 0x02:		//sunday
				date_time [0] = 'S';
				date_time [1] = 'U';
				date_time [2] = 'N';
				break;
			case 0x03:		//monday
				date_time [0] = 'M';
				date_time [1] = 'O';
				date_time [2] = 'N';
				break;
			case 0x04:		//tuesday
				date_time [0] = 'T';
				date_time [1] = 'U';
				date_time [2] = 'E';
				break;
			case 0x05:		//wednesday
				date_time [0] = 'W';
				date_time [1] = 'E';
				date_time [2] = 'D';
				break;
			case 0x06:		//thursday
				date_time [0] = 'T';
				date_time [1] = 'H';
				date_time [2] = 'U';
				break;
			case 0x07:		//friday
				date_time [0] = 'F';
				date_time [1] = 'R';
				date_time [2] = 'I';
				break;
		}
		
		HAL_UART_Receive(&huart1,&receive_c, sizeof(receive_c),50); 			// receive command from tera term
		HAL_UART_Receive(&huart2,&receive_c2, sizeof(receive_c),50); 			// receive command from the web interface through wifi
		if (receive_c == 'd' || receive_c2 == 'd' )								//send date and time when recieving 'd'
		{
			HAL_UART_Transmit(&huart1,date_time, sizeof(date_time),500);
			receive_c = '\0';
		}
				else if (receive_c == 't' || receive_c2 == 't')				//toggle led when recieving 't'
		{
			HAL_GPIO_TogglePin (GPIOB, GPIO_PIN_12);
			HAL_Delay(1000);
		}
		
		else if (receive_c == 'n' || receive_c2 == 'n')				//turn led on when recieving 'n'
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,0);
			receive_c = '\0';
		}
				else if (receive_c == 'f' || receive_c2 == 'f')				//turn led off when recieving 'f'
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12,1);
			receive_c = '\0';
		}
		
		
        
	}

}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
