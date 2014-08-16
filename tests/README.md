Automated testing
=================

CEGUI employs various automated testing executables to prevent breakages.

How to run all tests
--------------------

We use ctest which is part of cmake to run tests.

```bash
cd $cegui_folder
cd build/
CEGUI_SAMPLE_DATAPATH=`pwd`/../datafiles/ ctest -V
```
