#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arch.h"
#include "commands.h"

void cmd_attach(struct state *state, char *args) {
    if (is_attached(state->ucn)) {
        printf("Already attached to process %i\n", state->ucn.pid);
        return;
    }

    char *pid_arg = strtok(args, " ");
    if (pid_arg) {
        pid_t pid;
        if (!strcmp(pid_arg, "prev")) {
            if (state->prev_pid) {
                pid = state->prev_pid;
            } else {
                printf("No previous pid saved\n");
                return;
            }
        } else {
            pid = atoi(pid_arg);
            if (pid <= 0) {
                printf("Usage: attach [pid]\n");
                printf("       attach prev\n");
                return;
            }
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

    state->prev_pid = state->ucn.pid;
}

void cmd_detach(struct state *state, char *args) {
    if (is_attached(state->ucn)) {
        detach(&state->ucn);
    } else {
        printf("Not currently attached\n");
    }
}

void cmd_status(struct state *state, char *args) {
    if (is_attached(state->ucn)) {
        printf("Currently attached to process %i\n", state->ucn.pid);
        printf("Base address: %p\n", state->ucn.base);
    } else {
        printf("Currently not attached\n");
    }
}

void cmd_fps(struct state *state, char *args) {
    if (!is_attached(state->ucn)) {
        printf("Currently not attached\n");
        return;
    }

    char *fps_arg = strtok(args, " ");
    if (fps_arg) {
        int fps = atoi(fps_arg);
        if (fps <= 0) {
            printf("Usage: attach [fps]\n");
            return;
        }

        const void *global1 =
            (const void *)read_word(state->ucn, state->ucn.base + GLOBAL1);
        while (global1) {
            write_word(state->ucn, global1 + GLOBAL1_FPS, fps);
            global1 =
                (const void *)read_word(state->ucn, global1 + GLOBAL1_NEXT);
        }
    } else {
        const void *global1 =
            (const void *)read_word(state->ucn, state->ucn.base + GLOBAL1);
        if (!global1) {
            printf("No fps pointer found\n");
            return;
        }

        long fps_first = read_word(state->ucn, global1 + GLOBAL1_FPS);
        global1 = (const void *)read_word(state->ucn, global1 + GLOBAL1_NEXT);
        long fps = fps_first;

        printf("Fps list: [");
        while (global1) {
            printf("%li, ", fps);
            fps = read_word(state->ucn, global1 + GLOBAL1_FPS);
            global1 =
                (const void *)read_word(state->ucn, global1 + GLOBAL1_NEXT);
        }
        printf("%li]\nCurrent fps: %li\n", fps, fps_first);
    }
}

void cmd_quit(struct state *state, char *args) { state->running = false; }
