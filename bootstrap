#!/bin/sh

export WANT_AUTOMAKE='1.10'
export WANT_AUTOCONF_2_5=1

libtoolize --force --no-warn && aclocal $ACLOCAL_FLAGS && autoheader && automake --include-deps --add-missing --foreign && autoconf 
