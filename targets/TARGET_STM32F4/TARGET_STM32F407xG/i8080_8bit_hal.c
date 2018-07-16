/*
 * i8080_8bit_hal.c
 *
 * HAL functions for STM32 libraries
 *
 *  Created on: Jul 16, 2018
 *      Author: gdbeckstein
 */

#include "stm32f4xx_hal.h"

/**
 * These functions will be called automatically by the underlying
 * STM32F4 HAL drivers/libraries
 *
 * They will be called when HAL_SRAM_Init or HAL_SRAM_DeInit are called
 */

static uint32_t FSMC_Initialized = 0;

static uint32_t FSMC_DeInitialized = 0;

static void HAL_FSMC_MspInit(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if (FSMC_Initialized) {
    return;
  }

  FSMC_Initialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FSMC_CLK_ENABLE();

  /** FSMC GPIO Configuration
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PD11   ------> FSMC_A16 (Data/Command) NOTE! No longer managed by FSMC peripheral
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE (Output/Read Enable)
  PD5   ------> FSMC_NWE (Write Enable)
  PD7   ------> FSMC_NE1 (Chip select)
  */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0
                        |GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7; //| GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram) {
  HAL_FSMC_MspInit();
}

static void HAL_FSMC_MspDeInit(void)
{

  if (FSMC_DeInitialized) {
    return;
  }

  FSMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FSMC_CLK_DISABLE();

  /** FSMC GPIO Configuration
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PD11   ------> FSMC_A16 (No longer managed by FSMC peripheral)
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PD7   ------> FSMC_NE1
  */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0
                         |GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7); //| GPIO_PIN_11);

}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* hsram) {
  HAL_FSMC_MspDeInit();
}
