#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>

#include "command.h"

const char DEF_PROC_NAME[] = "Ultimate Custom Night.exe";

const ptrdiff_t FRAMES = 0x000b49cc;
const ptrdiff_t OFFSET_FPS_VAL = 0x78;
const ptrdiff_t OFFSET_FPS_NEXT = 0x1a8;

void cmd_attach(struct state *state, char *args);
void cmd_detach(struct state *state, char *args);
void cmd_quit(struct state *state, char *args);
void cmd_status(struct state *state, char *args);

const struct command COMMANDS[] = {
    {"attach", cmd_attach},
    {"detach", cmd_detach},
    {"quit", cmd_quit},
    {"status", cmd_status},
};

const size_t CMD_CNT = sizeof(COMMANDS) / sizeof(struct command);

#endif
