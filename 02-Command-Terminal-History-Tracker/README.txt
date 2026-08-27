
Objective
==> 
 * **********************************The Project: Command History Tracker****************************************************
----------------------------------------------------------------------------------------------------------------------------------

Build a program that simulates a simplified command history buffer — like what a shell keeps track of when you press the up-arrow 
to see previous commands.

-------Core requirements:
-->Store a history of commands (strings) the user types, one at a time, in a loop, until they type "exit".

-->The history has a starting capacity (say, 5 commands) but must grow dynamically once it fills up — you can't just reject 
   new commands once full. When it's full and a new command comes in, resize the storage to hold more (doubling capacity is 
   a common, reasonable strategy).
 
-->Each stored command must be its own independently allocated string — not a fixed-size char[100] array per slot, but memory
   sized to fit whatever the user actually typed (so a 3-character command and a 200-character command shouldn't waste the same 
   fixed space).
 
-->Track, using a function-persistent counter, the total number of commands ever entered across the program's run — even ones 
   that got evicted or overwritten — and print this total when the program exits. (Yes — this is deliberately testing whether 
   static inside a function actually stuck with you.)

-->Use at least two macros, both genuinely justified by function, not just decorative:
   --->One for debug/logging output that can be compiled in or out (#ifdef DEBUG style) — think back to why this matters for embedded work.
 
   --->One that wraps memory allocation with a check — if malloc/realloc ever returns NULL, print an error and exit gracefully rather than 
       crashing with a segfault later

-->Print the full command history, in order, when the user exits.

-->Free every single allocation before the program ends — every command string, and the array holding them.
-->Run it through valgrind and confirm zero leaks, zero errors — paste me the summary output when you're done, not just "it worked."

        *******************************Things that I didn't learn yet, need to research myself********************
---------------------------------------------------------------------------------------------------------------------------------
 
-->How to safely store and grow an array of independently-allocated strings (this means an array of pointers — you'll bump into 
   "pointer to pointer" naturally here, which is actually Week 3 material; figuring out how far you can reason about it on your
   own first is part of the point)

-->How to duplicate a string into freshly allocated memory of the right size (there's a standard function for this — look it up 
   rather than hand-rolling it, but understand what it's doing under the hood once you find it)
-->Exactly how realloc behaves — what happens to your old pointer, what you must do with its return value, and what mistake 
   would cause a leak or crash if you handle it carelessly


Hardware
--> No hardware is used yet

Software
-->Codelite on Ubuntu, Valgrind, Github

Design decisions
Results
Future improvements