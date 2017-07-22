exec_commands - A program that executs a list of data block commands

1) To build and run: type make at the command-line, followed by ./exec_commands
   $ make
   $ ./exec_commands

   Note: Written in C using some C++ features. 
   Built and tested on Mac OS X 10.11.3 
   (Apple LLVM version 7.3.0 (clang-703.0.31), Target: x86_64-apple-darwin15.3.0)

2) Code details:
   
main.cpp is the main program, it creates a block manager and executes a series of
commands against it.

block_manager.{h,cpp} contains the data structures and logic for implementing and
executing block commands.

BEList.{h,cpp}, BEHash.{h,cpp} list and hash structures used by block managers.
Based on code I had written previously (ref: https://github.com/colm-mchugh/calcit,
specifically list.{h,cpp} and hashtable.{h,cpp})

3) Improvements/next steps
- Implement Updating when an update could result in clobbering.
- Analyze memory usage.
- Adapt consistent coding style. 
- Makefile: add header file dependency, only recompile changed source files
