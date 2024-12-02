#ifndef ARCH_H
#define ARCH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_WIN32)
#include <windows.h>
typedef DWORD pid_t;
#elif defined(__linux__)
#include <sys/types.h>
#endif

struct ucn {
    pid_t pid;
    const void *base;
#if defined(_WIN32)
    HANDLE hnd;
#endif
};

struct ucn attach_pid(pid_t pid);
struct ucn attach_pname(const char *proc_name);

void detach(struct ucn *ucn);
bool is_attached(struct ucn ucn);

long read_word(struct ucn ucn, const void *addr);
void read_data(struct ucn ucn, const void *addr, void *buf, size_t len);

void write_word(struct ucn ucn, const void *addr, long word);
void write_data(struct ucn ucn, const void *addr, const void *data, size_t len);

#endif
