#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>

#include "command.h"

static const char DEF_PROC_NAME[] = "Ultimate Custom Night.exe";

static const ptrdiff_t GLOBAL1 = 0x000b49cc;
static const ptrdiff_t GLOBAL1_FPS = 0x78;
static const ptrdiff_t GLOBAL1_NEXT = 0x1a8;

void cmd_attach(struct state *state, char *args);
void cmd_detach(struct state *state, char *args);
void cmd_fps(struct state *state, char *args);
void cmd_quit(struct state *state, char *args);
void cmd_status(struct state *state, char *args);

static const struct command COMMANDS[] = {
    {"attach", cmd_attach},
    {"detach", cmd_detach},
    {"fps", cmd_fps},
    {"quit", cmd_quit},
    {"status", cmd_status},
};

static const size_t CMD_CNT = sizeof(COMMANDS) / sizeof(struct command);

#endif
