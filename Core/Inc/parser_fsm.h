/*
 * parser_fsm.h
 *
 *  Created on: Nov 15, 2025
 *      Author: ASUS
 */

#ifndef INC_PARSER_FSM_H_
#define INC_PARSER_FSM_H_

#include "main.h"
#include <string.h>

#define CMD_RST 1
#define CMD_OK 2

extern uint8_t command_flag;
extern uint8_t command_data[30];

void command_parser_fsm(void);

#endif /* INC_PARSER_FSM_H_ */
