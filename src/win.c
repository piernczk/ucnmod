#include "arch.h"

#include <psapi.h>
#include <string.h>

static const struct ucn UCN_NULL = {0, 0, 0};

const void *get_base(HANDLE hnd, const char *proc_name, size_t name_len) {
    // get modules inside the process
    HMODULE init_module;
    DWORD modules_bytes = 0;
    EnumProcessModules(hnd, &init_module, sizeof(HMODULE), &modules_bytes);
    HMODULE *modules = malloc(modules_bytes);
    EnumProcessModules(hnd, modules, modules_bytes, &modules_bytes);

    // allocate some space for the module name
    int modules_count = modules_bytes / sizeof(HMODULE);
    HMODULE hmod;
    size_t buf_size = name_len + 1;
    char *name = malloc(sizeof(char) * buf_size);

    // search for the base module
    for (int i = 0; i < modules_count; i++) {
        hmod = modules[i];
        GetModuleBaseNameA(hnd, hmod, name, buf_size);
        if (!strcmp(name, proc_name)) {
            break;
        }
    }

    MODULEINFO info;
    GetModuleInformation(hnd, hmod, &info, sizeof(MODULEINFO));
    const void *base = info.lpBaseOfDll;

    free(modules);
    free(name);

    return base;
}

struct ucn attach_pid(pid_t pid) {
    HANDLE hnd = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false,
        pid);
    if (!hnd)
        return UCN_NULL;

    // get the process namse
    char *name = NULL;
    size_t buf_size = 0;
    size_t last_len = 0;
    while (buf_size == last_len) {
        buf_size += 20;
        name = realloc(name, buf_size);
        last_len = GetModuleBaseNameA(hnd, NULL, name, buf_size);
    }

    const void *base = get_base(hnd, name, last_len);
    free(name);
    struct ucn ucn = {pid, base, hnd};
    return ucn;
}

struct ucn attach_pname(const char *proc_name) {
    // get process list
    size_t size = 0;
    DWORD count = 0;
    DWORD *processes = NULL;
    while (size == count) {
        size += 0x200;
        processes = realloc(processes, size);
        EnumProcesses(processes, size, &count);
    }

    size_t name_len = strlen(proc_name);
    size_t buf_size = name_len + 1;
    char *name = malloc(buf_size);

    // search through the processes
    DWORD pid;
    bool cont;
    for (DWORD i = 0; i < count / sizeof(DWORD); i++) {
        pid = processes[i];
        HANDLE hnd = OpenProcess(
            PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, false, pid);
        GetModuleBaseNameA(hnd, NULL, name, name_len);
        cont = strcmp(proc_name, name);
        CloseHandle(hnd);
        if (!cont) {
            break;
        }
    }

    free(processes);

    if (cont) {
        // the process doesn't exist
        free(name);
        return UCN_NULL;
    }

    HANDLE hnd = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false,
        pid);
    if (!hnd) {
        free(name);
        return UCN_NULL;
    }

    const void *base = get_base(hnd, proc_name, name_len);
    free(name);
    struct ucn ucn = {pid, base, hnd};
    return ucn;
}

void detach(struct ucn *ucn) {
    CloseHandle(ucn->hnd);
    *ucn = UCN_NULL;
}

bool is_attached(struct ucn ucn) { return ucn.pid != 0; }

void read_data(struct ucn ucn, const void *addr, void *buf, size_t len) {
    ReadProcessMemory(ucn.hnd, addr, buf, len, NULL);
}

long read_word(struct ucn ucn, const void *addr) {
    long word;
    read_data(ucn, addr, &word, 4);
    return word;
}

void write_data(
    struct ucn ucn, const void *addr, const void *data, size_t len) {
    WriteProcessMemory(ucn.hnd, (LPVOID)addr, data, len, NULL);
}

void write_word(struct ucn ucn, const void *addr, long word) {
    write_data(ucn, addr, &word, 4);
}
