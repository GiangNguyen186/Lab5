/*
 * UART_fsm.c
 *
 *  Created on: Nov 15, 2025
 *      Author: ASUS
 */

#include "UART_fsm.h"
#include "parser_fsm.h"
#include <stdio.h>
#include <string.h>

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;

typedef enum {
    IDLE,
    SEND_ADC,
    WAIT_OK
} CommState;

CommState comm_state = IDLE;
uint32_t timeout_counter = 0;
uint32_t last_adc_value = 0;

void uart_communication_fsm(void) {
    switch(comm_state) {
        case IDLE:
            // Chờ lệnh RST
            if(command_flag == CMD_RST) {
                command_flag = 0;
                comm_state = SEND_ADC;
            }
            break;

        case SEND_ADC:
            HAL_ADC_Start(&hadc1);
            HAL_ADC_PollForConversion(&hadc1, 100);
            last_adc_value = HAL_ADC_GetValue(&hadc1);
            HAL_ADC_Stop(&hadc1);

            uint8_t msg[50];
            sprintf((char*)msg, "!ADC=%lu#\r\n", last_adc_value);

            HAL_UART_Transmit(&huart2, msg, strlen((char*)msg), 1000);

            timeout_counter = HAL_GetTick();

            comm_state = WAIT_OK;
            break;

        case WAIT_OK:
            if(command_flag == CMD_OK) {
                command_flag = 0;
                comm_state = IDLE;
            }
            // Kiểm tra timeout
            else if(HAL_GetTick() - timeout_counter > 3000) {
                uint8_t msg[50];
                sprintf((char*)msg, "!ADC=%lu#\r\n", last_adc_value);
                HAL_UART_Transmit(&huart2, msg, strlen((char*)msg), 1000);

                timeout_counter = HAL_GetTick();

            }
            break;

        default:
            comm_state = IDLE;
            break;
    }
}
