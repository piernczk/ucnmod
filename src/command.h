#ifndef COMMAND_H
#define COMMAND_H

#include "arch.h"

struct state {
    struct ucn ucn;
    pid_t prev_pid;
    bool running;
};

struct command {
    const char *name;
    void (*run)(struct state *state, char *args);
};

const struct command *command_find(
    const struct command *cmds, size_t cmd_cnt, const char *name);

#endif
