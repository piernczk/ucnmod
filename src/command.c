#include "command.h"

#include <string.h>

const struct command *command_find(
    const struct command *cmds, size_t cmd_cnt, const char *name) {
    const struct command *end = cmds + cmd_cnt;
    for (const struct command* cmd = cmds; cmd < end; cmd++) {
        if (!strcmp(name, cmd->name)) {
            return cmd;
        }
    }

    return NULL;
}
