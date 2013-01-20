#!/usr/bin/env bash

# crude and horrible, I know!

CEED_PATH=../../ceed

# this is a hack that simply runs echo instead of forking a new bash
# we get the paths and nothing is forked
RUNWRAPPER_NO_FORK=1 source ${CEED_PATH}/bin/runwrapper.sh

${CEED_PATH}/bin/ceed-mic imagesets/AlfiskoSkin.meta-imageset
