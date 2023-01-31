/*
 * protocol.c
 *
 *  Created on: Jan 31, 2023
 *      Author: Denis
 */


#include "protocol.h"

#define PROTOCOL_BUF_SIZE (1000)
static uint8_t buffer[PROTOCOL_BUF_SIZE];
static uint16_t length;

void handler_cmd_status(uint8_t * params, uint16_t len);
void handler_cmd_stop_dome(uint8_t * params, uint16_t len);
void handler_cmd_open_dome(uint8_t * params, uint16_t len);
void handler_cmd_close_dome(uint8_t * params, uint16_t len);
void handler_cmd_shutter_move_deg(uint8_t * params, uint16_t len);
void handler_cmd_switch_toggle(uint8_t * params, uint16_t len);
void handler_cmd_arm_rain(uint8_t * params, uint16_t len);
void handler_cmd_get_move_params(uint8_t * params, uint16_t len);
void handler_cmd_set_move_params(uint8_t * params, uint16_t len);

xCmd_tt cmds[] = {
		{
				.cmd = "STATUS",
				.handler = handler_cmd_status
		},
		{
				.cmd = "STOPDOME",
				.handler = handler_cmd_stop_dome
		},
		{
				.cmd = "OPENDOME",
				.handler = handler_cmd_open_dome
		},
		{
				.cmd = "CLOSEDOME",
				.handler = handler_cmd_close_dome
		},
		{
				.cmd = "SHUTTERMOVEDEG",
				.handler = handler_cmd_shutter_move_deg
		},
		{
				.cmd = "SWITCHTOGGILE",
				.handler = handler_cmd_switch_toggle
		},
		{
				.cmd = "ARMRAIN",
				.handler = handler_cmd_arm_rain
		},
		{
				.cmd = "GETMOVEPARAMS",
				.handler = handler_cmd_get_move_params
		},
		{
				.cmd = "SETMOVEPARAMS",
				.handler = handler_cmd_set_move_params
		},
};


static protocol_sender_tt sender;

static uint8_t def_sender(uint8_t * buf, uint16_t len);

uint8_t def_sender(uint8_t * buf, uint16_t len){

}

void protocol_init(protocol_sender_tt protocol_sender){
	sender = protocol_sender ? protocol_sender: def_sender;
}
void protocol_parser(uint8_t * buf, uint16_t len){

	if(length + len < PROTOCOL_BUF_SIZE){
		memcpy(buffer + length, buf, len);
		length += len;

		while(1){
			uint8_t * feed = memchr(buffer, 0x0d, length );
			if(feed == NULL)
				break;
			*feed = '\0';

			uint16_t cmd_cnt = sizeof cmds / sizeof cmds[0];
			for(uint16_t i = 0; i < cmd_cnt; ++i){
				if(strstr(buffer, cmds[i].cmd)){
					cmds[i].handler(buffer + strlen(cmds[i].cmd), strlen(buffer) - strlen(cmds[i].cmd));
				}
			}

			length -= feed - buffer + 1;
			memcpy(buffer, feed + 1, length);
		}
	}

}

void handler_cmd_status(uint8_t * params, uint16_t len){
	sender("OK\r\n", 4);
}

void handler_cmd_stop_dome(uint8_t * params, uint16_t len){
	sender("OK\r\n", 4);
	dome_stop();
}
void handler_cmd_open_dome(uint8_t * params, uint16_t len){
	sender("OK\r\n", 4);
	dome_open();
}
void handler_cmd_close_dome(uint8_t * params, uint16_t len){
	sender("OK\r\n", 4);
	dome_close();
}

void handler_cmd_shutter_move_deg(uint8_t * params, uint16_t len){
	sender("OK\r\n", 4);
}
void handler_cmd_switch_toggle(uint8_t * params, uint16_t len){
	sender("OK\r\n", 4);
}
void handler_cmd_arm_rain(uint8_t * params, uint16_t len){
	sender("OK\r\n", 4);
}
void handler_cmd_get_move_params(uint8_t * params, uint16_t len){
	sender("OK\r\n", 4);
}
void handler_cmd_set_move_params(uint8_t * params, uint16_t len){
	sender("OK\r\n", 4);
}
