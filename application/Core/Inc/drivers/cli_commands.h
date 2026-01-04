/*
 * cli_commands.h
 *
 *  Created on: Jan 2, 2026
 *      Author: sande
 */

#ifndef INC_DRIVERS_CLI_COMMANDS_H_
#define INC_DRIVERS_CLI_COMMANDS_H_

#include "stdint.h"

//void cmd_fault_dump(int argc, char *argv[]);
//void cmd_fault_clear(int argc, char *argv[]);
void cli_dispatch(char *cmd);

#endif /* INC_DRIVERS_CLI_COMMANDS_H_ */
