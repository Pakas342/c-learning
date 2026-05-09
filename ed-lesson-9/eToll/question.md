eToll

You’re writing a “toll‑booth” server that handles vehicle payments on N separate lanes. Each lane is represented by a non-blocking file descriptor (e.g. a pipe, socket). “Cars” arrive at random intervals on any lane and send a small payment message. The booth must process each payment and log it, then immediately be ready for the next car—across all lanes—without busy‑waiting.

Setup lanes

    Create N nonblocking FIFOs (or sockets).

    Fork N “car generator” child processes, each writing “CAR–ID:amount\n” to its assigned FIFO at random intervals (e.g. sleep 0–2 s).

Write the toll‑booth server

    Create an epoll instance with epoll_create1().

    Register all N lane fds in level‑triggered mode:

Should we make read blocking or non-blocking? Why?
