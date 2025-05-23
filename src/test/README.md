# Notes
The sources in this directory are unit test cases.  Boost includes a
unit testing framework, and since Neobytes Core already uses boost, it makes
sense to simply use this framework rather than require developers to
configure some other framework (we want as few impediments to creating
unit tests as possible).

The build system is setup to compile an executable called "test_neobytes"
that runs all of the unit tests.  The main source file is called
test_neobytes.cpp, which simply includes other files that contain the
actual unit tests (outside of a couple required preprocessor
directives).  The pattern is to create one test file for each class or
source file for which you want to create unit tests.  The file naming
convention is "<source_filename>_tests.cpp" and such files should wrap
their tests in a test suite called "<source_filename>_tests".  For an
examples of this pattern, examine uint160_tests.cpp and
uint256_tests.cpp.

Add the source files to /src/Makefile.test.include to add them to the build.

For further reading, I found the following website to be helpful in
explaining how the boost unit test framework works:
[http://www.alittlemadness.com/2009/03/31/c-unit-testing-with-boosttest/](http://www.alittlemadness.com/2009/03/31/c-unit-testing-with-boosttest/).

test_neobytes has some built-in command-line arguments; for
example, to run just the getarg_tests verbosely:

    test_neobytes --log_level=all --run_test=getarg_tests

... or to run just the doubleneobytes test:

    test_neobytes --run_test=getarg_tests/doubleneobytes

Run  test_neobytes --help   for the full list.

