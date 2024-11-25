#include "arch.h"

#include <stdio.h>
#include <sys/ptrace.h>

const size_t LONG_SIZE = sizeof(long);
const size_t WORD_SIZEEE = 32;
const long WORD_MASKKK = (1L << WORD_SIZEEE) - 1;

long mask_bytes(size_t cnt) {
    // i think this assumes little-endianness but idc
    return (1 << (2 * cnt)) - 1;
}

long read_long(handle_t hnd, void *addr) {
    long word = ptrace(PTRACE_PEEKDATA, hnd, addr, 0);
    return word;
}

void write_long(handle_t hnd, void *addr, long word) {
    ptrace(PTRACE_POKEDATA, hnd, addr, word);
}

handle_t open_pid(pid_t pid) {
    long res = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    return res == 0 ? pid : 0;
}

handle_t open_auto(char *proc_name) {
    printf("unimplemented\n");
    return 0;
}

void close(handle_t hnd) { ptrace(PTRACE_DETACH, hnd, NULL, NULL); }
bool is_open(handle_t hnd) { return hnd != 0; }

long read_word(handle_t hnd, void *addr) {
    return read_long(hnd, addr) & WORD_MASKKK;
}

void read_data(handle_t hnd, void *addr, void *buf, size_t len) {
    long *addr_long = (long *)addr;
    long *buf_long = (long *)buf;

    size_t full = len / LONG_SIZE;
    for (size_t i = 0; i < full; i++) {
        *buf_long = read_long(hnd, addr_long);
        addr_long++;
        buf_long++;
    }

    size_t rest = len % LONG_SIZE;
    if (rest != 0) {
        long mask = mask_bytes(rest);
        long word = read_long(hnd, addr_long) & mask;
        *buf_long &= ~mask;
        *buf_long |= word;
    }
}

void write_word(handle_t hnd, void *addr, long word) {
    long old = read_long(hnd, addr) & ~WORD_MASKKK;
    long new = (word & WORD_MASKKK) | old;
    write_long(hnd, addr, new);
}

void write_data(handle_t hnd, void *addr, void *data, size_t len) {
    long *addr_long = (long *)addr;
    long *data_long = (long *)data;

    size_t full = len / LONG_SIZE;
    for (size_t i = 0; i < full; i++) {
        write_long(hnd, addr_long, *data_long);
        addr_long++;
        data_long++;
    }

    size_t rest = len % LONG_SIZE;
    if (rest != 0) {
        long mask = mask_bytes(rest);
        long old = read_long(hnd, addr_long) & ~mask;
        long word = (*data_long & mask) | old;
        write_long(hnd, addr_long, word);
    }
}
