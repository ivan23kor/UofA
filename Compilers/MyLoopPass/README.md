# My first LLVM optimization pass
Finds all loop nests and reports the following information for each one:
1. How many levels does the loop nest contain?
2. Is it a perfect nest?
3. Are the loops normalized?
4. Is the iteration space a rectangular polyhedron or does it have a different shape?

## Installation
The directory MyLoopPass and the file CMakeLists.txt here can be copied to ...llvm-project/llvm/lib/Transforms

To run, change the line below the comment *# FIX HERE* in mypass and run `./mypass`
