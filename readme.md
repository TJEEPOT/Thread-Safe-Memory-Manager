# What It Does #
I built this project to understand more about Systems Programming, how memory management works in C, pointers and function pointers in C as well as how to make a program thread-safe. The program uses Unix system calls and was designed to work in the Linux OS.

The project is split into three folders, each one containing the code in C, a C header file and a file written in C to test the given solution. 

The first folder (MemMan) was my implementation of a basic memory manager using the "First Fit" strategy. It contains functions to initialise a given amount of memory from the operating system, to allocate a portion of that memory to another service and another to de-allocate the memory. This system will look through the blocks of initialised memory until it finds one large enough to store what is being requested, then forms that block and marks it as in-use. 

The second folder (MemMan2) extends the first solution with support for "Next Fit", "Best Fit" and "Worst Fit" strategies. The initialise function was then modified to accept a function pointer to select which strategy should be used (with the default being "First Fit").

The third folder (MemMan3) extends the second solution with thread-safe capabilities. This uses a mutex lock to ensure two threads don't claim the same piece of memory at the same time, and relies on Linux POSIX thread system calls.

# What I Learned #
I gained a greater understanding of programming in **C**, including using **Header files** and separating out test files. An understanding of **memory management** in low-level programming using **malloc** and assigning blocks of this memory using **linked list** data structures. I also understood how to use **function pointers** to have a function perform multiple different roles depending on what name was passed to it. Finally, I learned how to use **Linux System Calls**, including **pthread** to enable my program to be **multi-threaded**.

# Usage Notes #
To test the first two programs (MemMan, MemMan2), compile with GCC in Linux as standard
<sub><code>gcc part1.c part1_test.c -o part1 
gcc part2.c part2_test.c -o part2 </sub></code>

For testing MemMan3, you will need to use the -lpthread command line switch to link the pthread library to the compiled program
<sub><code>gcc part3.c part3_test.c -o part3 -lpthread </sub></code>

There are no command line arguments for the compiled program.