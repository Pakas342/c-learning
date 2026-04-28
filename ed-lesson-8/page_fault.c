#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

static char *page; // global so the handler can remap it
static int page_size;

static void page_fault_handler_simulation(int sig, siginfo_t *info,
                                          void *uctx) {
    (void)sig;
    (void)uctx;

    const char *reason = (info->si_code == SEGV_MAPERR) ? "address not mapped"
                                                        : "permission denied";

    dprintf(STDERR_FILENO, "SIGSEGV at %p: %s\n", info->si_addr, reason);

    // TODO: remap `page` with read/write permissions so the faulting
    //       instruction can be retried successfully when the handler
    //       returns (hint: see mprotect in `man 2 mprotect`)
    //
    mprotect(page, page_size, PROT_READ | PROT_WRITE);

    // No _exit here — we want to return and let the access succeed.
}

int main(void) {
    struct sigaction sa = {0};
    sa.sa_sigaction = page_fault_handler_simulation;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    page_size = getpagesize();

    // Reserve a page with no access permissions.
    page = mmap(NULL, page_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (page == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // dprintf is similar to fprintf but uses fd (int) instead of FILE*
    // safer when working with signals
    dprintf(STDERR_FILENO, "mapped PROT_NONE page at %p\n", page);

    // TODO: dereference `page` to trigger the fault.
    //       After the handler fixes the permissions, this access
    //       should succeed and the program should continue.

    dprintf(STDERR_FILENO, "value at page: page[0] = %d\n", *page);

    // dprintf is similar to fprintf but uses fd (int) instead of FILE*
    // safer when working with signals
    dprintf(STDERR_FILENO, "recovered: page[0] = %d\n", page[0]);
    return 0;
}
