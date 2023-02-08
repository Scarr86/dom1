/*
 * protocol.c
 *
 *  Created on: Jan 31, 2023
 *      Author: Denis
 */


#include "protocol.h"

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
	sprintf(tx_buffer, "STATUS%d,%d,%d,%d,0,0,0,0,0,0,%d,%d,%d,%d,%d",
			dome_state(0),
			dome_state(1),
			dome_encoder(0),
			dome_encoder(1),
			dom_rele_is_active(0),
			dom_rele_is_active(1),
			dom_rele_is_active(2),
			dom_sensor_rain_is_enable(0),
			dom_sensor_rain_is_detected(0)
			);
	sender(tx_buffer, strlen(tx_buffer));
}

void handler_cmd_stop_dome(uint8_t * params, uint16_t len){
	dome_stop();
	sender("OK\r\n", 4);
}
void handler_cmd_open_dome(uint8_t * params, uint16_t len){
	dome_open(0, 90);
	dome_open(1, 90);
	sender("OK\r\n", 4);
}
void handler_cmd_close_dome(uint8_t * params, uint16_t len){
	dome_close();
	sender("OK\r\n", 4);
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

	sender("OK\r\n", 4);
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
	sender("OK\r\n", 4);
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

	sender("OK\r\n", 4);
}
void handler_cmd_get_move_params(uint8_t * params, uint16_t len){
	sprintf(tx_buffer, "MOVEPARAMS%d,%d,%d,%d,%.2f,%.2f,%d",
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
	uint16_t val;
	char * p = strtok(params, sep);
	if(p){
		val = atoi(p);
		if(dom_pwm_break_set(val)){
			sender(error_str, strlen(error_str));
			return;
		}
	}
	else{
		sender(error_str, strlen(error_str));
		return;
	}
	p = strtok(NULL, sep);
	if(p){
		val = atoi(p);
		if(dom_pwm_full_set(val)){
			sender(error_str, strlen(error_str));
			return;
		}
	}
	else{
		sender(error_str, strlen(error_str));
		return;
	}

	p = strtok(NULL, sep);
	if(p){
		val = atoi(p);
		if(dom_pwm_accel_set(val)){
			sender(error_str, strlen(error_str));
			return;
		}
	}
	else{
		sender(error_str, strlen(error_str));
		return;
	}

	p = strtok(NULL, sep);
	if(p){
		val = atoi(p);
		if(dom_angle_break_set(val)){
			sender(error_str, strlen(error_str));
			return;
		}
	}
	else{
		sender(error_str, strlen(error_str));
		return;
	}
	p = strtok(NULL, sep);
	p = strtok(NULL, sep);
	p = strtok(NULL, sep);
	if(p){
		val = atoi(p);
		if(dom_sensor_rain_set(0, -1, val)){
			sender(error_str, strlen(error_str));
			return;
		}
	}
	else{
		sender(error_str, strlen(error_str));
		return;
	}

	sender("OK\r\n", 4);
}
