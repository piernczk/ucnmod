#include "arch.h"

#include <stdio.h>
#include <sys/ptrace.h>

const size_t LONG_SIZE = sizeof(long);
const size_t WORD_SIZE = 32;
const long WORD_MASK = (1L << WORD_SIZE) - 1;
// it's always this address in wine by default
// i think it could be changed but i don't bother with this
// do you really need aslr for fnaf??
const void *const BASE = (const void *)0x00400000;
const struct ucn UCN_NULL = {0, 0};

long mask_bytes(size_t cnt) {
    // i think this assumes little-endianness but idc
    return (1 << (2 * cnt)) - 1;
}

long read_long(struct ucn ucn, const void *addr) {
    long word = ptrace(PTRACE_PEEKDATA, ucn.pid, addr, 0);
    return word;
}

void write_long(struct ucn ucn, const void *addr, long word) {
    ptrace(PTRACE_POKEDATA, ucn.pid, addr, word);
}

struct ucn attach_pid(pid_t pid) {
    long res = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    struct ucn ucn = {res == 0 ? pid : 0, BASE};
    return ucn;
}

struct ucn attach_pname(const char *proc_name) {
    printf("unimplemented\n");
    return UCN_NULL;
}

void detach(struct ucn *ucn) {
    ptrace(PTRACE_DETACH, ucn->pid, NULL, NULL);
    *ucn = UCN_NULL;
}

bool is_attached(struct ucn ucn) { return ucn.pid != 0; }

long read_word(struct ucn ucn, const void *addr) {
    return read_long(ucn, addr) & WORD_MASK;
}

void read_data(struct ucn ucn, const void *addr, void *buf, size_t len) {
    long *addr_long = (long *)addr;
    long *buf_long = (long *)buf;

    size_t full = len / LONG_SIZE;
    for (size_t i = 0; i < full; i++) {
        *buf_long = read_long(ucn, addr_long);
        addr_long++;
        buf_long++;
    }

    size_t rest = len % LONG_SIZE;
    if (rest != 0) {
        long mask = mask_bytes(rest);
        long word = read_long(ucn, addr_long) & mask;
        *buf_long &= ~mask;
        *buf_long |= word;
    }
}

void write_word(struct ucn ucn, const void *addr, long word) {
    long old = read_long(ucn, addr) & ~WORD_MASK;
    long new = (word & WORD_MASK) | old;
    write_long(ucn, addr, new);
}

void write_data(
    struct ucn ucn, const void *addr, const void *data, size_t len) {
    long *addr_long = (long *)addr;
    long *data_long = (long *)data;

    size_t full = len / LONG_SIZE;
    for (size_t i = 0; i < full; i++) {
        write_long(ucn, addr_long, *data_long);
        addr_long++;
        data_long++;
    }

    size_t rest = len % LONG_SIZE;
    if (rest != 0) {
        long mask = mask_bytes(rest);
        long old = read_long(ucn, addr_long) & ~mask;
        long word = (*data_long & mask) | old;
        write_long(ucn, addr_long, word);
    }
}
