#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arch.h"
#include "command.h"
#include "commands.h"

int main() {
    static const size_t BUF_SIZE = 1024;
    struct state state;
    state.prev_pid = 0;
    char *input = malloc(BUF_SIZE);
    state.running = true;
    while (state.running) {
        bool not_eof = true;
        do {
            printf("ucnmod> ");
            if (!(not_eof = fgets(input, BUF_SIZE, stdin))) {
                break;
            }
            input[strcspn(input, "\r\n")] = '\0';
        } while (!*input);
        if (!not_eof) {
            break;
        }

        char *first_space = strchr(input, ' ');
        char *cmd_name = input;
        if (first_space) {
            *first_space = '\0';
        }

        const struct command *cmd = command_find(COMMANDS, CMD_CNT, cmd_name);
        if (!cmd) {
            printf("Command \"%s\" not found\n", cmd_name);
            continue;
        }

        char *args;
        if (first_space) {
            args = first_space + 1;
        } else {
            *input = '\0';
            args = input;
        }

        cmd->run(&state, args);
    }

    if (is_attached(state.ucn)) {
        detach(&state.ucn);
    }
    free(input);
    return 0;
}
