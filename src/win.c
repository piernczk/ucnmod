#include "arch.h"

#include <psapi.h>

ucn_t open_pid(pid_t pid) {
    HANDLE hnd = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ |
                                 PROCESS_VM_WRITE,
                             false, pid);
    return hnd;
}

ucn_t open_auto(char *proc_name) {
    int size = 0;
    DWORD count = 0;
    DWORD *processes = NULL;
    while (size == count) {
        free(processes);
        size += 0x200;
        processes = malloc(size);
        EnumProcesses(processes, size, &count);
    }

    int len = strlen(proc_name) + 1;
    char *name = malloc(len);

    DWORD pid;
    bool cont;
    for (int i = 0; i < count / sizeof(DWORD); i++) {
        pid = processes[i];
        HANDLE hnd = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
                                 false, pid);
        GetModuleBaseNameA(hnd, NULL, name, len);
        cont = strcmp(proc_name, name);
        CloseHandle(hnd);
        if (!cont) {
            break;
        }
    }

    free(processes);

    if (cont) {
        return 0;
    }

    return open_pid(pid);
}

void close(ucn_t ucn) { CloseHandle(ucn); }
bool is_open(ucn_t ucn) { return ucn != 0; }

void read_data(ucn_t ucn, void *addr, void *buf, size_t len) {
    ReadProcessMemory(ucn, addr, buf, len, NULL);
}

long read_word(ucn_t ucn, void *addr) {
    long word;
    read_data(ucn, addr, &word, 4);
    return word;
}

void write_data(ucn_t ucn, void *addr, void *data, size_t len) {
    WriteProcessMemory(ucn, addr, data, len, NULL);
}

void write_word(ucn_t ucn, void *addr, long word) {
    write_data(ucn, addr, &word, 4);
}
