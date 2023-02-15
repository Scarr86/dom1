/*
 * protocol.c
 *
 *  Created on: Jan 31, 2023
 *      Author: Denis
 */


#include "protocol.h"
#include "string.h"
#include "stdio.h"
#define PROTOCOL_BUF_SIZE (1000)
static uint8_t buffer[PROTOCOL_BUF_SIZE];
static uint8_t tx_buffer[PROTOCOL_BUF_SIZE];
static uint16_t length;
static char * sep = ",";
static char * error_str = "ERROR";


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

static uint8_t def_sender(char * buf, uint16_t len);

uint8_t def_sender(char * buf, uint16_t len){

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
	sprintf(tx_buffer, "STATUS%d,%d,%d,%d,0,0,0,0,0,0,%d,%d,%d,%d,%d\r",
			dome_status(GATE_1),
			dome_status(GATE_2),
			dome_encoder(GATE_1),
			dome_encoder(GATE_2),
			dom_rele_is_active(RELE_1),
			dom_rele_is_active(RELE_2),
			dom_rele_is_active(RELE_3),
			dom_sensor_rain_is_enable(SENSOR_RAIN_1),
			dom_sensor_rain_is_detected(SENSOR_RAIN_1)
			);
	sender(tx_buffer, strlen(tx_buffer));
}

void handler_cmd_stop_dome(uint8_t * params, uint16_t len){
	dome_stop();
	sender("OK\r", 4);
}
void handler_cmd_open_dome(uint8_t * params, uint16_t len){
	dome_open(GATE_1, 90);
	dome_open(GATE_2, 90);
	sender("OK\r", 4);
}
void handler_cmd_close_dome(uint8_t * params, uint16_t len){
	dome_open(GATE_1, 0);
	dome_open(GATE_2, 0);
	sender("OK\r", 4);
}

void handler_cmd_shutter_move_deg(uint8_t * params, uint16_t len){
	uint8_t n;
	uint16_t angle;
	char * p = strtok(params, sep);
	if(p){
		n = atoi(p);
	}
	else{
		sender(error_str, strlen(error_str));
		return;
	}
	p = strtok(NULL, sep);
	if(p){
		angle = atoi(p);
	}
	else{
		sender(error_str, strlen(error_str));
		return;
	}
	dome_open(n, angle);

	sender("OK\r", 4);
}
void handler_cmd_switch_toggle(uint8_t * params, uint16_t len){
	uint8_t n, on;
	char * p = strtok(params, sep);
	if(p){
		n = atoi(p);
	}
	else{
		sender(error_str, strlen(error_str));
		return;
	}
	p = strtok(NULL, sep);
	if(p){
		on = atoi(p);
	}
	else{
		sender(error_str, strlen(error_str));
		return;
	}

	if(on){
		dom_rele_active(n - 1);
	}
	else{
		dom_rele_inactive(n - 1);
	}
	sender("OK\r", 4);
}
void handler_cmd_arm_rain(uint8_t * params, uint16_t len){
	uint8_t on;
	char * p = strtok(params, sep);
	if(p){
		on = atoi(p);
	}
	else{
		sender(error_str, strlen(error_str));
		return;
	}

	dom_sensor_rain_set(0, on, -1);

	sender("OK\n", 4);
}
void handler_cmd_get_move_params(uint8_t * params, uint16_t len){
	sprintf(tx_buffer, "MOVEPARAMS%d,%d,%d,%d,%.2f,%.2f,%d\r",
			dom_pwm_break(),
			dom_pwm_full(),
			dom_pwm_accel(),
			dom_angle_break(),
			dome_koef(0),
			dome_koef(1),
			dom_sensor_rain_cmpval(0));
	sender(tx_buffer, strlen(tx_buffer));
}
void handler_cmd_set_move_params(uint8_t * params, uint16_t len){
	uint16_t val[7];
	char * p = strtok(params, sep);
	uint8_t i = 1;

	if(p){
		val[0] = atoi(p);
	}

	while(p){
		p = strtok(NULL, sep);
		val[i] = atoi(p);
		i += 1;
	}
	if(i == sizeof val){
		if(dome_move_params_set(val[0], val[1], val[2], val[3], val[4], val[5], val[6])){
			sender("OK\r", 4);
		}
		else{
			sender(error_str, strlen(error_str));
		}
	}
	else{
		sender(error_str, strlen(error_str));
	}
}
