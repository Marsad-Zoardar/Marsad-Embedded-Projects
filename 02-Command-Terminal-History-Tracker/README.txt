
## Objective

A command-line utility that stores a dynamically growing history of user-entered commands, similar to what a shell keeps track of for the up-arrow key. 
Each command is independently allocated to fit its actual length, and the whole history grows on demand as more commands are entered.

## Core Requirements

- Store user-typed commands in a loop until "exit" is entered
- Start with a fixed capacity, but grow dynamically (doubling) once full
- Each command gets its own independently-sized allocation — no wasted 
  space from a fixed-size buffer
- Track total commands entered across the run using a persistent counter
- Use at least two justified macros: one for conditional debug logging, 
  one for safe allocation with a NULL check
- Print the full history and free every allocation before exiting
- Verify correctness with valgrind

## Hardware

No hardware used yet — this project runs entirely on the development machine.

## Software

CodeLite on Ubuntu, Valgrind, GitHub

## Design Decisions

- **`char **` over a fixed 2D array (`char[N][BUFFER]`):** an earlier version used a fixed-row 2D array, which was simpler but wasted memory 
  on short commands. Switching to `char **` — an array of independently allocated string pointers — matches each command's actual length, at 
  the cost of managing two separate levels of allocation and cleanup.

- **`SAFE_ALLOC` used deliberately in only one place:** the very first allocation, where nothing needs cleanup on failure. Every later 
  allocation uses a manual check instead, since a macro can't express "also free everything allocated so far" — a case where a macro was the 
  wrong tool once cleanup logic started varying by call site.

- **A conditional `LOG` macro** using `#ifdef DEBUG`, so debug tracing compiles out entirely (zero runtime cost) when disabled — a pattern 
  that matters in embedded work, where debug output often can't ship in production builds.

## Bugs Found and Fixed

- **Silent overwrite:** the array index counter was declared inside the outer input loop, resetting to 0 every iteration and silently 
  overwriting the first slot on every new command instead of filling new ones. Fixed by moving the counter outside the loop so it persists.

- **Double-free:** in the array-resize failure path, the last-stored string was freed once explicitly and a second time inside the cleanup 
  helper's loop — genuine undefined behavior, not just a style issue. Fixed by removing the redundant explicit free.

- **Missing NULL check on `fgets`:** on end-of-input, `fgets` returns NULL without writing anything, and the following code would have 
  operated on uninitialized memory. Added an explicit check that cleans up and exits.

## Results

Compiled with `-std=c99 -Wall`, run under `valgrind --leak-check=full`: 
zero memory leaks, zero errors.

## Future Improvements

- Use `strdup` instead of manual `malloc` + copy for each command string
- Add command search/retrieval by index, not just print-all-on-exit
- Persist history to a file between runs
- Replace the array-based growth with a linked list to avoid the 
  periodic large `realloc` calls

## Build & Run

\`\`\`bash
gcc -std=c99 -Wall commandTerminal.c -o commandTerminal
./commandTerminal
\`\`\`
