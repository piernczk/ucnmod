#include "command.h"

#include <string.h>

const struct command *command_find(
    const struct command *cmds, size_t cmd_cnt, const char *name) {
    // maybe should have used linear search since there won't be a lot of
    // commands anyways
    // l, r and m must be signed to prevent underflow
    ssize_t l = 0;
    ssize_t r = cmd_cnt - 1;
    while (l <= r) {
        ssize_t m = (l + r) / 2;
        const char *cur_name = cmds[m].name;

        int cmp = strcmp(name, cur_name);
        if (cmp > 0)
            l = m + 1;
        else if (cmp < 0)
            r = m - 1;
        else
            return cmds + m;
    }

    return NULL;
}
