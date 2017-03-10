Installation
=========
The installation requires `cmake >= 3.1.0`, `bison >= 3.0.4` and `flex`.

To build the compiler run `cmake path/to/source` from your build directory. 
It is advised to create a directory named *bin* in the source as your build directory. 
(Then the command becomes `cmake ..`)

Note that in source builds are disabled, you cannot run cmake from the source root.

There are two ways to compile the project (after running cmake):
	1. Using gnu make: run `make` from the build directory.
	2. Using MSVC: run `devenv cpq.sln /build` from the build directory.

Testing
======
The tests require `python 2.7` with the `pytest`  and `pathlib` packages.
To run the tests go to the *tests* directory in the source directory and run `py.test --compiler=/path/to/compiler`.
If you compiled the compiler in the *bin* directory below the source root the `--compiler` option is not needed.

Todo
====
1. Add error handling in the compiler.
2. Refactor the statements.
3. Add documentation about design decisions.
4. Complete the tests.

