Automated testing
=================

CEGUI employs various automated testing executables to prevent breakages.

How to run all tests
--------------------

We use ctest which is part of cmake to run tests. You first need to enable
the tests that you want to run. Enable `CEGUI_BUILD_TESTS` and/or
`CEGUI_BUILD_DATAFILES_TEST` options in cmake.

Then run the tests using:
```bash
cd $cegui_folder
cd build/
CEGUI_SAMPLE_DATAPATH=`pwd`/../datafiles/ ctest -V
```
