#ifndef ARCH_H
#define ARCH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32)
#include <windows.h>
typedef DWORD pid_t;
typedef HANDLE ucn_t;
#elif defined(__linux__)
#include <sys/types.h>
typedef pid_t ucn_t;
#endif

ucn_t open_pid(pid_t pid);
ucn_t open_auto(char *proc_name);

void close(ucn_t ucn);
bool is_open(ucn_t ucn);

long read_word(ucn_t ucn, void *addr);
void read_data(ucn_t ucn, void *addr, void *buf, size_t len);

void write_word(ucn_t ucn, void *addr, long word);
void write_data(ucn_t ucn, void *addr, void *data, size_t len);

#endif
