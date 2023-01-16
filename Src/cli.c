/*
 * cli.c
 *
 *  Created on: 12 янв. 2023 г.
 *      Author: Denis
 */
#include "cli.h"

const char * sep = " ,";

const char * help = "\r\n/h - help\r\n"
		"/? [option] [id] - state  all devices\r\n"
		"options:\r\n"
		"\t m - state all motor\r\n"
		"\t id - concrete device";
#define CLI_BUF_SIZE (1000)
char cbuf[CLI_BUF_SIZE];
uint16_t cbuf_len;
uint8_t cbuf_is_lock;
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
	cbuf_is_lock = 0;
	cbuf_owf = 0;
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
			}
			sender("\r\n", 2);
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
		case 'b':
			p = strtok(NULL, sep);
			if(p == NULL){
				for(uint8_t i = 0; i < BUTTON_COUNT; ++i){
					parser_btn( i+1 , NULL);
				}
				return;
			}
			parser_btn(atoi(p), strtok(NULL, sep));
		break;
		case 'l':
			p = strtok(NULL, sep);
			if(p == NULL){
				slen = sprintf(cbuf, "\r\nLED"
						"\r\nblink: %s"
						"\r\nstate: %s"
						"\r\nfrq: %d", dom_led_mode() ? "on" : "off", dom_led_state() ? "on" : "off", dom_led_frq() );
				sender(cbuf, slen);
				return;
			}
			switch(*p){
				case '-':
					dom_led_set(0, LED_BLINK_OFF, 0);
				break;
				case '+':
					dom_led_set(1, LED_BLINK_OFF, 0);
				break;
				case '~':
					p = strtok(NULL, sep);
					if(p == NULL){
						dom_led_set(0, LED_BLINK_ON, 0);
					}
					else{
						dom_led_set(0, LED_BLINK_ON, atoi(p));
					}
				break;
				default:
					slen = sprintf(cbuf, "\r\nincorrect command");
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
	uint16_t slen;
	if(id == 0){
		slen = sprintf(cbuf, "\r\nincorrect command");
		sender(cbuf, slen);
		return;
	}
	if(p == NULL){
		slen = sprintf(cbuf, "\r\nBUTTON %d"
				"\r\nstate: %d"
				"\r\ndebounceTime: %d", id, btn_state_by_id(id), btn_debounce_time_by_id(id));
		sender(cbuf, slen);
		return;
	}
	switch(*p){
		case '=':
			p = strtok(NULL, sep);
			if(p == NULL){
				// TODO send setting btn id;
				return;
			}
			//TODO set while has params
		break;
		default:
			slen = sprintf(cbuf, "\r\nincorrect command");
			sender(cbuf, slen);
		break;
	}

}



