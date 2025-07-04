# Advanced C shell 

---

This project is a custom Unix-like shell written in C that supports a wide range of features including command parsing, background execution, piping, I/O redirection, and built-in commands like `hop`, `reveal`, `log`, `seek`, and `proclore`. It mimics the behavior of standard shells while adding features like persistent command logging, signal handling, and custom aliases/functions via `.myshrc`. Advanced features include support for background/foreground process control, combining redirection with pipes, process information display, and even fetching online man pages using sockets (`iMan`). The code is modular, error-resilient, and designed to be extended easily.

---
 The implementation is based on the specifications provided in [`Questions.md`](./Questions.md).
