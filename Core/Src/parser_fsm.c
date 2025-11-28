/*
 * parser_fsm.c
 *
 *  Created on: Nov 15, 2025
 *      Author: ASUS
 */

#include "parser_fsm.h"
#include <string.h>

extern UART_HandleTypeDef huart2;
extern uint8_t buffer[30];
extern uint8_t index_buffer;

uint8_t command_flag = 0;
uint8_t command_data[30];
uint8_t overflow_flag = 0;

typedef enum {
    WAIT_START,
    RECEIVE_DATA
} ParserState;

ParserState parser_state = WAIT_START;
uint8_t cmd_index = 0;

void command_parser_fsm(void) {
    for(int i = 0; i < index_buffer; i++) {
        uint8_t c = buffer[i];

        switch(parser_state) {
            case WAIT_START:
                if(c == '!') {
                    parser_state = RECEIVE_DATA;
                    cmd_index = 0;
                    overflow_flag = 0;
                }
                break;

            case RECEIVE_DATA:
                if(c == '#') {
                    if(overflow_flag) {
                        parser_state = WAIT_START;
                        cmd_index = 0;
                        overflow_flag = 0;
                        break;
                    }

                    command_data[cmd_index] = '\0';

                    if(strcmp((char*)command_data, "RST") == 0) {
                        command_flag = CMD_RST;
                    }
                    else if(strcmp((char*)command_data, "OK") == 0) {
                        command_flag = CMD_OK;
                    }

                    parser_state = WAIT_START;
                }
                else {
                    if(cmd_index < 30) {
                        command_data[cmd_index++] = c;
                    }
                    else {
                        overflow_flag = 1;
                    }
                }
                break;
        }
    }

    index_buffer = 0;
}
