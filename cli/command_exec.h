#ifndef COMMAND_EXEC_H
#define COMMAND_EXEC_H

#include "status.h"

#include "workspace.h"

#include "commands.h"


#define SUB_ENTRIES_PRINTED_PER_LINE 7
#define SUB_ENTRY_PADDING_WIDTH 7
#define SUB_ENTRY_SPACING_WIDTH 3

StatusCode cmd_cd_exec(Workspace *workspace, Command *cmd);
StatusCode cmd_pwd_exec(Workspace *workspace, Command *cmd);
StatusCode cmd_ls_exec(Workspace *workspace, Command *cmd);
StatusCode cmd_touch_exec(Workspace *workspace, Command *cmd);
StatusCode cmd_mkdir_exec(Workspace *workspace, Command *cmd);
StatusCode cmd_rm_exec(Workspace *workspace, Command *cmd);
StatusCode cmd_rmdir_exec(Workspace *workspace, Command *cmd);
StatusCode cmd_mv_exec(Workspace *workspace, Command *cmd);
StatusCode cmd_rename_exec(Workspace *workspace, Command *cmd);
StatusCode cmd_clear_exec(Workspace *workspace, Command *cmd);


StatusCode cmd_append_exec(Workspace *workspace, Command *cmd, char *data_buffer, size_t data_buffer_length);
StatusCode cmd_cat_exec(Workspace *workspace, Command *cmd, char *data_buffer, size_t data_buffer_length);

#endif