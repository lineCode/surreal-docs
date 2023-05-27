#  
#  Copyright © 2022, Kommpute Entertainment™ Sp. z o.o. All rights reserved.
#  For any additional information refer to https://kommpute.com
#  

#must be called from the project's root

param (
    [Parameter(Mandatory=$true)][string]$mbs_root,
    [Parameter(Mandatory=$true)][string]$template
)

trap
{
    write-output $_
    exit 1
}

function Check-ExitCode {
    if ($LastExitCode -ne 0) {
        exit 1
    }
}

# go where all DLLs are
cd build_release/dist

# run tests executable which is in other folder
../dist_tests/tests
Check-ExitCode
