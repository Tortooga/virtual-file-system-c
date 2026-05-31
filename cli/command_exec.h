#ifndef COMMAND_EXEC_H
#define COMMAND_EXEC_H

#include "status.h"

#include "workspace.h"

#include "commands.h"

StatusCode cmd_cd_exec(Workspace *workspace, Command *cmd);
StatusCode cmd_pwd_exec(Workspace *workspace, Command *cmd);

#endif