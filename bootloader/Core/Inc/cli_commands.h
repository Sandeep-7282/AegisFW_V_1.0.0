/*
 * cli_commands.h
 *
 *  Created on: Dec 26, 2025
 *      Author: sande
 */

#ifndef INC_CLI_COMMANDS_H_
#define INC_CLI_COMMANDS_H_

#pragma once
#include "boot_metadata.h"

void cli_commands(const char *cmd, boot_metadata_t *meta);
void cmd_help(void);

#endif /* INC_CLI_COMMANDS_H_ */
