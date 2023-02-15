/*
 * cli.c
 *
 *  Created on: 12 янв. 2023 г.
 *      Author: Denis
 */
#include "cli.h"

#define CLI_BUF_SIZE (1000)


static void cli_cmd_parser(uint8_t * cmd);

static void cli_btn_on_click(uint8_t id);

static void cli_sensor_on_detected(uint8_t id);

static void cli_gate_change(GATE_ENUM id);

const char * sep = " ,";

const char * help = "\r\n/h - help\r\n"
		"/? [option] [id] - state  all devices\r\n"
		"option:\r\n"
		"\t 'm' - state and settings all or [id] motor\r\n"
		"\t 'b' - state and settings all or [id] button\r\n"
		"\t 's' - state all or [id] sensor\r\n"
		"\t 'o' - state all or [id] odometer\r\n"
		"\t 'l' - state led\r\n"
		"\t 'g' - state gate all or [id]\r\n"
		"/m id option [speed, min_speed, deg_speed, ...] - settings and control motor\r\n"
		"option:\r\n"
		"\t '=' - set [speed, ...]\r\n"
		"\t '>' - turn forward\r\n"
		"\t '<' - turn back\r\n"
		"\t '*' - stop\r\n"
		"/b id option [debounceTime, ...] - settings button\r\n"
		"option:\r\n"
		"\t '=' - set [debounceTime, ...]\r\n"
		"/s id option [cmpVal, ...] - settings sensor\r\n"
		"option:\r\n"
		"\t '=' - set [cmpVal, ...]\r\n"
		"/l option [frq,...] - setting and control led\r\n"
		"option:\r\n"
		"\t '+' - led on\r\n"
		"\t '-' - led off\r\n"
		"\t '~' - led blink mode [frq, ...]";

char cbuf[CLI_BUF_SIZE];
uint8_t tbuf[80];
uint16_t cbuf_len;
uint8_t cbuf_owf;

cli_sender_tt sender;

uint8_t def_sender(uint8_t * buf, uint16_t len);
void parser_btn(uint8_t id, uint8_t * p);


uint8_t def_sender(uint8_t * buf, uint16_t len){
	return 0;
}

void cli_init(cli_sender_tt cli_sender){
	sender = cli_sender ? cli_sender: def_sender;
	cbuf_len = 0;
	cbuf_owf = 0;

	dom_btn_subscribe(cli_btn_on_click);
	dom_sensor_subscribe(cli_sensor_on_detected);
	gate_subscribe(cli_gate_change);
}

void cli_parser(uint8_t * buf, uint16_t len){


	if(cbuf_len + len < CLI_BUF_SIZE){
		memcpy(cbuf + cbuf_len, buf, len);
		cbuf_len += len;

		while(1){
			char * line_feed =  memchr(cbuf, '\r', cbuf_len);

			if(line_feed == NULL)
				break;

			*line_feed = '\0';
			char * cmd_start = memchr(cbuf, '/', line_feed - cbuf );
			if(cmd_start){
				cli_cmd_parser(cmd_start + 1 );
				sender("\r\n", 2);
			}

			cbuf_len -= line_feed + 1 - cbuf;
			memcpy(cbuf, line_feed + 1, cbuf_len);
		}
	}
	else{
		cbuf_len = 0;
		cbuf_owf = 1;
	}
}

void cli_cmd_parser(uint8_t * cmd){
	uint16_t slen;

	uint8_t id;
	uint8_t result;
	int16_t param[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	uint16_t iparam = 0;

	char * p = strtok(cmd, sep);
	if(strlen(p) > 1){
		slen = sprintf(cbuf, "\r\nincorrect command");
		sender(cbuf, slen);
		return;
	}
	switch(*p){
		case 'h':
			sender(help, strlen(help));
		break;
		case '?':
			p = strtok(NULL, sep);
			if(p == NULL){
				sprintf(tbuf, "? b");
				cli_cmd_parser(tbuf);
				sprintf(tbuf, "? s");
				cli_cmd_parser(tbuf);
				sprintf(tbuf, "? m");
				cli_cmd_parser(tbuf);
				sprintf(tbuf, "? g");
				cli_cmd_parser(tbuf);
				sprintf(tbuf, "? l");
				cli_cmd_parser(tbuf);
				return;
			}
			switch(*p){
				case 'b':
					p = strtok(NULL, sep);
					if(p == NULL){
						for(uint8_t i = 0; i < BUTTON_COUNT; ++i){
							parser_btn( i+1 , NULL);
						}
						return;
					}
					parser_btn(atoi(p), NULL);
				break;
				case 'l':
					slen = sprintf(cbuf, "\r\n-LED-"
							"\r\nblink: %s"
							"\r\nstate: %s"
							"\r\nfrq: %d\r\n", dom_led_pwm_mode() ? "on" : "off", dom_led_pwm_state() ? "on" : "off", dom_led_pwm_frq() );
					sender(cbuf, slen);
				break;
				case 's':
					p = strtok(NULL, sep);
					if(p ==  NULL){
						for(uint8_t i = 0; i < SENSOR_COUNT; ++i){
							sprintf(tbuf, "? s %d", i+1);
							cli_cmd_parser(tbuf);
						}
						return;
					}
					slen = sprintf(cbuf, "\r\n-SENSOR [%d]-\r\n"
													"state: %d\r\n"
													"cmpVal: %d\r\n", atoi(p),
													dom_sensor_state(atoi(p)),
													dom_sensor_cmp_val(atoi(p)));
					sender(cbuf, slen);
				break;
				case 'm':
					p = strtok(NULL, sep);
					if(p == NULL){
						for(uint8_t i = 0; i < MOTOR_COUNT; ++i){
							sprintf(tbuf, "? m %d", i+1);
							cli_cmd_parser(tbuf);
						}
						return;
					}
					id = atoi(p);
					if(id == 0 || id > MOTOR_COUNT){
						slen = sprintf(cbuf, "\r\nincorrect command");
						sender(cbuf, slen);
						return;
					}
					id -= 1;
					slen = sprintf(cbuf, "\r\n-MOTOR [%d]-\r\n"
							"state: %d\r\n"
							"dir: %d\r\n"
							"speed: %d\r\n"
							"deg: %d\r\n"
							"deg_speed_set / deg_speed: %lu / %lu\r\n"
							"dist / pos: %ld / %ld\r\n"
							"pos_0 / pos_90: %ld / %ld\r\n",
							id + 1,
							dom_motor_state(id),
							dom_motor_dir(id),
							dom_motor_speed(id),
							dom_motor_deg(id),
							dom_motor_deg_speed(id), motor_deg_speed(get_motor(id)),
							dom_motor_dist(id), dom_motor_pos(id),
							dom_motor_pos_0(id), dom_motor_pos_90(id)
							);
					sender(cbuf, slen);
				break;
				case 'g':
					p = strtok(NULL, sep);
					if(p == NULL){
						for(uint8_t i = 0; i < GATE_COUNT; ++i){
							sprintf(tbuf, "? g %d", i+1);
							cli_cmd_parser(tbuf);
						}
						return;
					}
					id = atoi(p);
					if(id == 0 || id > GATE_COUNT){
						slen = sprintf(cbuf, "\r\nincorrect command");
						sender(cbuf, slen);
						return;
					}
					id -= 1;
					slen = sprintf(cbuf, "\r\n-GATE [%d]-\r\n"
							"state: %s\r\n"
							"dist / pos: %ld / %ld\r\n"
							"deg_speed_set / deg_speed: %lu / %lu\r\n"
							"deg: %d\r\n",
							id + 1,
							dome_state(id) == GATE_STATE_STOP ? "STOP":
									dome_state(id) == GATE_STATE_ClOSING ? "ClOSING" : "OPENING",
							dome_dist(id),
							dome_pos(id),
							(dom_motor_deg_speed(get_gate(id)->mid[0]) + dom_motor_deg_speed(get_gate(id)->mid[1])) / 2,
							dome_deg_speed(id),
							dome_encoder(id)
							);
					sender(cbuf, slen);

				break;
				case 'o':
					p = strtok(NULL, sep);
					if(p == NULL){
						for(uint8_t i = 0; i < ODOMETER_COUNT; ++i){
							sprintf(tbuf, "? o %d", i+1);
							cli_cmd_parser(tbuf);
						}
						return;
					}
					id = atoi(p);
					if(id == 0 || id > ODOMETER_COUNT){
						slen = sprintf(cbuf, "\r\nincorrect command");
						sender(cbuf, slen);
					}
					else{
						slen = sprintf(cbuf, "\r\n-ODOMETER [%d]-\r\n"
								"value: %lu\r\n", id, dom_odometer_value(id - 1) );
						sender(cbuf, slen);
					}
				break;
				default:
					slen = sprintf(cbuf, "\r\nincorrect command");
					sender(cbuf, slen);
				break;
			}

		break;
		case 'b':
			p = strtok(NULL, sep);// read id
			id = atoi(p);
			p = strtok(NULL, sep);// read option
			if(p == NULL || id == 0){
				slen = sprintf(cbuf, "\r\nincorrect command");
				sender(cbuf, slen);
				return;
			}
			parser_btn(id, p);
		break;
		case 'l':
			p = strtok(NULL, sep);
			if(p == NULL){
				slen = sprintf(cbuf, "\r\nincorrect command");
				sender(cbuf, slen);
				return;
			}
			switch(*p){
				case '-':
					dom_led_pwm_set(0, LED_BLINK_OFF, 0);
				break;
				case '+':
					dom_led_pwm_set(1, LED_BLINK_OFF, 0);
				break;
				case '~':
					p = strtok(NULL, sep);
					if(p == NULL){
						result = dom_led_pwm_set(0, LED_BLINK_ON, 0);
					}
					else{
						result = dom_led_pwm_set(0, LED_BLINK_ON, atoi(p));
					}
					slen = sprintf(cbuf, "\n%s", result > 0 ? "fail" : "done");
					sender(cbuf, slen);
				break;
				default:
					slen = sprintf(cbuf, "\r\nincorrect command");
					sender(cbuf, slen);
				break;
			}
		break;

		case 'm':
			p = strtok(NULL, sep);
			if(p == NULL){
				slen = sprintf(cbuf, "\r\nincorrect command");
				sender(cbuf, slen);
				return;
			}
			id = atoi(p);
			p = strtok(NULL, sep);
			if(p == NULL){
				slen = sprintf(cbuf, "\r\nincorrect command");
				sender(cbuf, slen);
				return;
			}
			switch(*p){
				case '>':
					dom_motor_forward(id, dom_pwm_full());
				break;
				case '<':
					dom_motor_back(id, dom_pwm_full());
				break;
				case '*':
					dom_motor_stop(id);
				break;
				case '=':
					iparam = 0;
					while(p = strtok(NULL, sep)){
						param[iparam] = atoi(p);
						++iparam;
					}
					result = dom_motor_set(id, param[0], param[1], param[2]);
					slen = sprintf(cbuf, "\n%s", result > 0 ? "fail" : "done");
					sender(cbuf, slen);
				break;
				default:
					slen = sprintf(cbuf, "\r\nincorrect command");
					sender(cbuf, slen);
				break;
			}

		break;

		case 's':
			p = strtok(NULL, sep);// read id
			id = atoi(p);
			p = strtok(NULL, sep);// read option
			if(p == NULL || id == 0){
				slen = sprintf(cbuf, "\r\nincorrect command");
				sender(cbuf, slen);
				return;
			}
			switch(*p){
				case '=':
					p = strtok(NULL, sep);
					if(p == NULL){
						result = dom_sensor_set(id, 0);
					}
					else{
						result = dom_sensor_set(id, atoi(p));
					}
					slen = sprintf(cbuf, "\n%s", result > 0 ? "fail" : "done");
					sender(cbuf, slen);
				break;
			}

		break;

		default:
			slen = sprintf(cbuf, "\r\nincorrect command");
			sender(cbuf, slen);
		break;
	}
}

void parser_btn(uint8_t id, uint8_t * p){
	//char * p = strtok(cmd, sep);
	uint16_t slen, result;
	if(id == 0){
		slen = sprintf(cbuf, "\r\nincorrect command");
		sender(cbuf, slen);
		return;
	}
	if(p == NULL){
		slen = sprintf(cbuf, "\r\n-BUTTON [%d]-"
				"\r\nstate: %d"
				"\r\ndebounceTime: %d\r\n", id, dom_btn_state(id), dom_btn_debounce_time(id));
		sender(cbuf, slen);
		return;
	}
	switch(*p){
		case '=':
			p = strtok(NULL, sep);
			if(p == NULL){
				result = dom_btn_set(id, 0);
			}
			else{
				result =dom_btn_set(id, atoi(p));
			}
			slen = sprintf(cbuf, "\n%s", result > 0 ? "fail" : "done");
			sender(cbuf, slen);
		break;
		default:
			slen = sprintf(cbuf, "\r\nincorrect command");
			sender(cbuf, slen);
		break;
	}

}

void cli_btn_on_click(uint8_t id){
	parser_btn(id+1, NULL);
	sender("\r\n", 2);
}

void cli_sensor_on_detected(uint8_t id){
	sprintf(tbuf, "/? s %d\r", id+1);
	cli_parser(tbuf, strlen(tbuf));
}

static void cli_gate_change(GATE_ENUM id){
	sprintf(tbuf, "/? g %d\r", id+1);
	cli_parser(tbuf, strlen(tbuf));
}

