#include "arch.h"

#include <stdio.h>
#include <sys/ptrace.h>

const size_t LONG_SIZE = sizeof(long);
const size_t WORD_SIZE = 32;
const long WORD_MASK = (1L << WORD_SIZE) - 1;

long mask_bytes(size_t cnt) {
    // i think this assumes little-endianness but idc
    return (1 << (2 * cnt)) - 1;
}

long read_long(ucn_t ucn, void *addr) {
    long word = ptrace(PTRACE_PEEKDATA, ucn, addr, 0);
    return word;
}

void write_long(ucn_t ucn, void *addr, long word) {
    ptrace(PTRACE_POKEDATA, ucn, addr, word);
}

ucn_t attach_pid(pid_t pid) {
    long res = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    return res == 0 ? pid : 0;
}

ucn_t attach_pname(char *proc_name) {
    printf("unimplemented\n");
    return 0;
}

void detach(ucn_t ucn) { ptrace(PTRACE_DETACH, ucn, NULL, NULL); }
bool is_attached(ucn_t ucn) { return ucn != 0; }

long read_word(ucn_t ucn, void *addr) {
    return read_long(ucn, addr) & WORD_MASK;
}

void read_data(ucn_t ucn, void *addr, void *buf, size_t len) {
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

void write_word(ucn_t ucn, void *addr, long word) {
    long old = read_long(ucn, addr) & ~WORD_MASK;
    long new = (word & WORD_MASK) | old;
    write_long(ucn, addr, new);
}

void write_data(ucn_t ucn, void *addr, void *data, size_t len) {
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
