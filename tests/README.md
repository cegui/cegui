Automated testing
=================

CEGUI employs various automated testing executables to prevent breakages.

How to run all tests
--------------------

We use ctest which is part of cmake to run tests. You first need to enable
the tests that you want to run. Enable `CEGUI_BUILD_TESTS` and/or
`CEGUI_BUILD_DATAFILES_TEST` options in cmake.

Then run the tests using the following commands, based on your operating system:

* Linux / Mac OS X
```bash
cd $cegui_folder
cd build/
CEGUI_SAMPLE_DATAPATH=`pwd`/../datafiles/ ctest -V
```

* Windows
```
cd <cegui folder>
cd build/
set CEGUI_SAMPLE_DATAPATH=%CD%/../datafiles/
ctest -V -C <configuration>
```
Where `<configuration>` is one of: `Debug`, `Release` or `RelWithDebInfo`, depending on what configuration you have built.