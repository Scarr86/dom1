/*
 * cli.c
 *
 *  Created on: 12 янв. 2023 г.
 *      Author: Denis
 */
#include "cli.h"

const char * help = "h - help\r\n"
		"? [option] [id] - state  all devices\r\n"
		"options:\r\n"
		"\t m - state all motor\r\n"
		"\t id - concrete device\r\n";

cli_sender_t sender;

uint8_t def_sender(uint8_t * buf, uint16_t len){
	return 0;
}

void cli_init(cli_sender_t cli_sender){
	sender = cli_sender ? cli_sender: def_sender;
}

void cli_parser(uint8_t * cmd, uint16_t len){
	char * p = strtok(cmd, " ");
	switch(*p){
		case 'h':
			sender(help, strlen(help));
		break;
	}
}


