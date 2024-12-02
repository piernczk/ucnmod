#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

void cmd_attach(struct state *state, char *args) {
    if (is_attached(state->ucn)) {
        printf("Already attached to process %i\n", state->ucn.pid);
        return;
    }

    char *pid_arg = strtok(args, " ");
    if (pid_arg) {
        pid_t pid = atoi(pid_arg);
        if (pid <= 0) {
            printf("Usage: attach [pid]\n");
            return;
        }

        state->ucn = attach_pid(pid);
        if (!is_attached(state->ucn)) {
            printf("Couldn't attach to process %i\n", pid);
            return;
        }
    } else {
        static const size_t BUF_SIZE = 128;
        printf("Process name [%s]: ", DEF_PROC_NAME);
        char *pname = malloc(BUF_SIZE);
        fgets(pname, BUF_SIZE, stdin);
        pname[strcspn(pname, "\r\n")] = '\0';
        bool to_free = true;
        if (!*pname) {
            free(pname);
            pname = (char *)DEF_PROC_NAME;
            to_free = false;
        }

        state->ucn = attach_pname(pname);
        if (!is_attached(state->ucn)) {
            printf("Couldn't attach to process \"%s\"\n", pname);
            if (to_free) {
                free(pname);
            }
            return;
        } else if (to_free) {
            free(pname);
        }
    }

    printf("Successfully attached to process %i\n", state->ucn.pid);
}

void cmd_detach(struct state *state, char *args) {
    if (is_attached(state->ucn)) {
        pid_t pid = state->ucn.pid;
        detach(&state->ucn);
        printf("Successfully detached from process %i\n", pid);
    } else {
        printf("Not currently attached\n");
    }
}

void cmd_status(struct state *state, char *args) {
    if (is_attached(state->ucn)) {
        printf("Currently attached to process %i\n", state->ucn.pid);
        printf("Base address: 0x%p\n", state->ucn.base);
    } else {
        printf("Currently not attached\n");
    }
}

void cmd_quit(struct state *state, char *args) { state->running = false; }
