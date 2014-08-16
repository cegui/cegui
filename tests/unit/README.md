Unit tests (a.k.a. regression tests)
====================================

This folder contains regression tests of the CEGUI library.

The whole system uses boost::test as a driving framework. This doesn't mean
you have to get boost to use CEGUI! You only need boost to run the test suite
which you likely aren't going to need.

However if you are a package maintainer please run our test suite.

Coverage
--------
We are not aiming for 100% coverage! CEGUI wasn't developed with tests
initially and it would be very hard to get everything covered at this point.

However tests will be gradually added and we plan to maintain and keep them
around from now on. They should help us make bigger changes with more
confidence and potentially point out issues we wouldn't have known about
otherwise.
