#!/bin/bash

#
# Copyright © 2022, Medelfor, Limited. All rights reserved.
# For any additional information refer to https://medelfor.com
#

#must be called from the project's root

mbs_root=$1
template=$2

function Check-ExitCode {
    if [ $? -ne 0 ]; then
        exit 1
    fi
}

# go where all DLLs are
cd build_release/dist

# run tests executable which is in other folder

export LD_LIBRARY_PATH=$(pwd)
../dist_tests/tests
Check-ExitCode
