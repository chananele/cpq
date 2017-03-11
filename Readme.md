Installation
============
The installation requires `cmake >= 3.1.0`, `bison >= 3.0.4` and `flex`.

To build the compiler run `cmake path/to/source` from your build directory. 
Next run `cmake --build /path/to/build/directory`.
If you are using MSVC add the parameter `--config Release` (or debug).
Note that in source builds are disabled, you cannot run cmake from the source root.

Testing
=======
The tests require `python 2.7` with the `pytest`  and `pathlib2` packages.
To run the test enter the command `ctest`, adding the parameter `-C Release` (or debug) when using MSVC.

Design
======


Todo
====
1. Add error handling in the compiler.
2. Refactor the statements.
3. Add documentation about design decisions.
4. Complete the tests.
