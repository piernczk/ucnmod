#ifndef ARCH_H
#define ARCH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __linux__
#include <sys/types.h>
#endif
#define handle_t pid_t

handle_t open_pid(pid_t pid);
handle_t open_auto(char *proc_name);

void close(handle_t hnd);
bool is_open(handle_t hnd);

long read_word(handle_t hnd, void *addr);
void read_data(handle_t hnd, void *addr, void *buf, size_t len);

void write_word(handle_t hnd, void *addr, long word);
void write_data(handle_t hnd, void *addr, void *data, size_t len);

#endif
