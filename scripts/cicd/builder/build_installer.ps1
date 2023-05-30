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

Remove-Item -Force -Recurse build_installer -EA SilentlyContinue
mkdir -Force build_installer

cd build_installer

mkdir -Force dist/bin/thirdparty
mkdir -Force dist/plugins/UnrealisticDocs

cd ..

# copy nsis stuff
Copy-Item -Recurse -Verbose -Force -Path installer/* -Destination build_installer

# copy 3rd party: doxygen and doxybook2
# copy the plugin
# copy the certs
# copy the resources and binaries

cd build_installer/dist

$account_email = (python ../../scripts/cicd/builder/get_setting.py $mbs_root `
    "integration.gcloud_account_email")
$gcp_project_id = (python ../../scripts/cicd/builder/get_setting.py $mbs_root `
    "integration.gcp_project_id")
$version = (python ../../scripts/cicd/builder/get_setting.py `
    $mbs_root "options.version")
$bucket = (python ../../scripts/cicd/builder/get_setting.py `
    $mbs_root "integration.artifacts_bucket")

gcloud auth activate-service-account $account_email `
    --key-file="$mbs_root/gcloud_key" --project=$gcp_project_id
Check-ExitCode

gsutil -m cp -r "gs://$bucket/doxygen-ue/$branch/doxygen-ue/latest/win64" bin/thirdparty
Check-ExitCode

gsutil -m cp -r "gs://$bucket/doxybook2/$branch/doxybook2/latest/win64" bin/thirdparty
Check-ExitCode
Copy-Item -Recurse -Verbose -Path bin/thirdparty/win64/* -Destination bin/thirdparty
Remove-Item -Recurse bin/thirdparty/win64

gsutil -m cp -r "gs://$bucket/unrealistic-docs-ue/$branch/unrealistic-docs-ue/latest/win64" plugins/UnrealisticDocs
Check-ExitCode
Copy-Item -Recurse -Verbose -Path plugins/UnrealisticDocs/win64/* -Destination plugins/UnrealisticDocs
Remove-Item -Recurse plugins/UnrealisticDocs/win64

gsutil -m cp -r "gs://$bucket/udocs-processor/$branch/udocs-processor/latest/win64" ./
Check-ExitCode
Copy-Item -Recurse -Verbose -Path win64/* -Destination ./
Remove-Item -Recurse win64

Copy-Item -Recurse -Verbose -Path ../../certificates -Destination certificates

cd ..
tree /f

mkdir -Force installer

makensis -DDIST_PATH=dist -DDIST_VERSION="$version" installer.nsi
Check-ExitCode
