/*
 * cli.h
 *
 *  Created on: 12 янв. 2023 г.
 *      Author: Denis
 */

#ifndef CLI_H_
#define CLI_H_

#include "stdint.h"
#include "string.h"
#include "dom.h"
#include "gate.h"
#include "stdio.h"
#include "stdlib.h"


typedef uint8_t (*cli_sender_tt)(uint8_t*, uint16_t);

void cli_init(cli_sender_tt cli_sender);
void cli_parser(uint8_t * buf, uint16_t len);



#endif /* CLI_H_ */
