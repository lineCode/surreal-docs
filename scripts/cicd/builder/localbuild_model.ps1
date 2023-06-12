cd build_release

Remove-Item dist_conan -Recurse -ErrorAction Ignore

# build dist, tests, conan package
cmake -DCMAKE_BUILD_TYPE=Debug ../ -DCMAKE_INSTALL_PREFIX=dist -DDO_BUILD_TESTS=OFF `
      -DDO_BUILD_EXECUTABLE=OFF -DDO_INSTALL_AS_CONAN_PACKAGE=ON -DCONAN_PACKAGE_INSTALL_POSTFIX=_conan `
      -DTESTS_INSTALL_POSTFIX=_tests -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON

cmake --build . --config Debug

cmake --install . --config Debug

# package the conan package

gulp mails -f ../gulpfile.js --destination build_release/dist --auto_res_prefix build_release
gulp process-html -f ../gulpfile.js --destination build_release/dist --auto_res_prefix build_release

mkdir -Force dist/res/html/site/emails
mkdir -Force dist/res/html/site/dashboard/common

# copy bundled resources to dist_conan
Copy-Item dist/res/html/site/emails -Destination `
      dist_conan/res/bundled/html/site/emails -Recurse -Force

Copy-Item dist/res/html/site/dashboard/common -Destination `
      dist_conan/res/bundled/html/site/dashboard/common -Recurse -Force

cd dist_conan

conan export-pkg ../../ medelfor/local -s os=Windows -s compiler="Visual Studio" `
      -s compiler.version=16 -s compiler.runtime=MD -s build_type=Release -s arch=x86_64 `
      -j ../install_udocs-processor.json --build-folder=. --source-folder=. `
      --profile ../../conanprofiles/udocs-processor-release-win64 -o udocs-processor:Shared=False -f

cd ../..
