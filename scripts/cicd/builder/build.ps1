#
# Copyright © 2022, Medelfor, Limited. All rights reserved.
# For any additional information refer to https://medelfor.com
#

param (
    [Parameter(Mandatory=$true)][string]$branch,
    [Parameter(Mandatory=$true)][string]$mbs_root,
    [Parameter(Mandatory=$true)][string]$template
)

#must be called from the project's root

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

mkdir -Force build_release
mkdir -Force build_debug

###############
### RELEASE ###
###############

cd build_release

Remove-Item dist -Recurse -ErrorAction Ignore
Remove-Item dist_conan -Recurse -ErrorAction Ignore
Remove-Item dist_tests -Recurse -ErrorAction Ignore

# build resources
gulp default -f ../gulpfile.js --destination build_release/dist --auto_res_prefix build_release
Check-ExitCode

# build dist, tests, conan package
cmake -DCMAKE_BUILD_TYPE=Release ../ -DCMAKE_INSTALL_PREFIX=dist -DDO_BUILD_TESTS=ON `
      -DDO_BUILD_EXECUTABLE=ON -DDO_INSTALL_AS_CONAN_PACKAGE=ON -DCONAN_PACKAGE_INSTALL_POSTFIX=_conan `
      -DTESTS_INSTALL_POSTFIX=_tests -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON -DRESOURCES_DIR=dist
Check-ExitCode

cmake --build . --config Release
Check-ExitCode

cmake --install . --config Release
Check-ExitCode

# clean symlinks to script modules in order for them not to be packaged
gulp clean-symlinks -f ../gulpfile.js --destination build_release/dist --auto_res_prefix build_release
Check-ExitCode

# copy bundled resources to dist_conan
Copy-Item dist/res -Destination dist_conan/res/bundled -Recurse -Force

# remove proto from dist/res
rm -Recurse -Force -Verbose dist/res/misc/proto

# copy dev resources to dist_conan
Copy-Item ../res -Destination dist_conan/res/dev -Recurse -Force

# remove theme/* (except for html) from dev and bundled
rm -Recurse -Force -Verbose dist_conan/res/dev/misc/graphics/udocs_processor/theme/
rm -Recurse -Force -Verbose dist_conan/res/dev/scripts/udocs_processor/theme/
rm -Recurse -Force -Verbose dist_conan/res/dev/styles/udocs_processor/theme/
rm -Recurse -Force -Verbose dist_conan/res/bundled/misc/graphics/udocs_processor/theme/
rm -Recurse -Force -Verbose dist_conan/res/bundled/scripts/udocs_processor/theme/
rm -Recurse -Force -Verbose dist_conan/res/bundled/styles/udocs_processor/theme*

# package the conan package

cd dist_conan

conan export-pkg ../../ medelfor/$branch -s os=Windows -s compiler="Visual Studio" `
      -s compiler.version=16 -s compiler.runtime=MD -s build_type=Release -s arch=x86_64 `
      -j ../install_udocs-processor.json --build-folder=. --source-folder=. `
      --profile ../../conanprofiles/udocs-processor-release-win64 -o udocs-processor:Shared=True

cd ../..

#############
### DEBUG ###
#############

cd build_debug

Remove-Item dist_conan -Recurse -ErrorAction Ignore

# build conan package
cmake -DCMAKE_BUILD_TYPE=Debug ../ -DCMAKE_INSTALL_PREFIX=dist -DDO_BUILD_TESTS=OFF `
      -DDO_BUILD_EXECUTABLE=OFF -DDO_INSTALL_AS_CONAN_PACKAGE=ON -DCONAN_PACKAGE_INSTALL_POSTFIX=_conan `
      -DTESTS_INSTALL_POSTFIX=_tests -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON
Check-ExitCode

cmake --build . --config Debug
Check-ExitCode

cmake --install . --config Debug
Check-ExitCode

# copy bundled and dev resources to dist_conan
Copy-Item ../build_release/dist_conan/res -Destination dist_conan/res -Recurse -Force

# package the conan package (debug)

cd dist_conan

conan export-pkg ../../ medelfor/$branch -s os=Windows -s compiler="Visual Studio" `
      -s compiler.version=16 -s compiler.runtime=MDd -s build_type=Debug -s arch=x86_64 `
      -j ../install_udocs-processor.json --build-folder=. --source-folder=. `
      --profile ../../conanprofiles/udocs-processor-debug-win64 -o udocs-processor:Shared=True
Check-ExitCode

#it's copied here instead of Install directive, because it's actually not a part of the project
#but just a mark for CI/CD system and it can be as well absent (it's guaranteed that the
#project works without it)
cd ../../
Copy-Item -Path githash -Destination build_release/dist/githash
Check-ExitCode
