# Atomatrix
Dynamic Compiler (OOP)

Note: this program runs under an atomic flow of instructions. There is one main thread and many fibers. This is easily confused with traditional programming languages that share stack spaces. The stack is becoming optional. Meta-programming may seem molecular for simplicity.

This compiler comes with a default virtual world mainly used for source samples and regression tests.

Don't be fooled, the atomic flow means less locks, stacks, and variables. A function that passes variables does it by the atomic flow instead of the non-atomic flow that requires synchronization and separate memory allocations. There are no shadow copies.
