#
#  Copyright © 2023, Medelfor, Limited. All rights reserved.
#  For any additional information refer to https://medelfor.com
#
  Unicode True

  !include "MUI2.nsh"
  !include "WordFunc.nsh"
  !include "Junction.nsh"
  Name "Surreal Docs v${DIST_VERSION}"
  OutFile "SurrealDocs-v${DIST_VERSION}-x64.exe"

  !define NodeJSNew "18.16.0"
  !define NodeJSRequired "10.18.1"

Function .onInit
  Var /GLOBAL CACHE_DIR
  StrCpy $CACHE_DIR "$APPDATA\Medelfor\SurrealDocs"

  Var /GLOBAL BIN_DIR
  StrCpy $BIN_DIR "$INSTDIR\bin"

  SetOutPath $TEMP
  File /oname=spltmp.bmp "Splash.bmp"

  splash::show 2500 $TEMP\spltmp

  Pop $0

  Delete $TEMP\spltmp.bmp
FunctionEnd

  InstallDir "$PROGRAMFILES64\Medelfor\SurrealDocs\"
  BrandingText "Copyright (c) 2023, Medelfor, Limited. All rights reserved"
  InstallDirRegKey HKCU "Software\Medelfor\SurrealDocs" ""

  RequestExecutionLevel admin

  !define MUI_ABORTWARNING
  !define MUI_LICENSEPAGE_CHECKBOX
  !define MUI_ICON "Icon.ico"
  !define MUI_UNICON "Icon.ico"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "InstallerSurrealDocsWelcomeFinish.bmp"

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "EULA.rtf"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

  !insertmacro MUI_LANGUAGE "English"

Section "!Surreal Docs CLI" SecCore
  SectionInstType RO

  SetOutPath "$INSTDIR"

  File /r "${DIST_PATH}"

  EnVar::Check "Path" "$BIN_DIR"
  Pop $0
  ${If} $0 != 0
    EnVar::AddValue "Path" "$BIN_DIR"
    Pop $0
    ${If} $0 != 0
      MessageBox MB_OK|MB_ICONEXCLAMATION "Unable to add $BIN_DIR to Path. Please add it manually" /SD IDOK
    ${EndIf}
  ${EndIf}

  WriteRegStr HKCU "Software\Medelfor\SurrealDocs" "" $INSTDIR
  WriteRegStr HKCU "Software\Medelfor\SurrealDocs" "WasInstalled" "1"
  WriteRegStr HKCU "Software\Medelfor\SurrealDocs" "Version" "${DIST_VERSION}"
  WriteRegStr HKCU "Software\Medelfor\SurrealDocs" "Cache" "$CACHE_DIR"

  WriteUninstaller "$INSTDIR\uninst.exe"
SectionEnd

Section "NodeJS" SecNodeJS
  SetOutPath "$INSTDIR"

  nsExec::ExecToStack '"node" "--version"'
  Pop $0
  Pop $1

  Var /GLOBAL do_install
  StrCpy $3 $1 1
  ${If} $3 != "v" ; no node is installed
    StrCpy $do_install "yes"
  ${Else}
    StrCpy $2 $1 "" 1
    ${VersionCompare} $2 "${NodeJSRequired}" $R0
    ${If} $R0 == 2 ; installed nodejs is older than required
      StrCpy $do_install "yes"
    ${Else}
      MessageBox MB_OK|MB_ICONINFORMATION "NodeJS requirement is already satisfied. No new NodeJS version will be installed" /SD IDOK
    ${EndIf}
  ${EndIf}

  ${If} $do_install == "yes"
    inetc::get "https://nodejs.org/dist/v${NodeJSNew}/node-v${NodeJSNew}-x64.msi" "$INSTDIR\node-v${NodeJSNew}-x64.msi"
    Pop $0
      StrCmp $0 "OK" dlok
      MessageBox MB_OK|MB_ICONEXCLAMATION "Unable to download NodeJS. Please download and install NodeJS \
        (at least v${NodeJSRequired}) manually. Click OK to exit the installer" /SD IDOK
      Quit
    dlok:

    ExecWait '"msiexec" /i "$INSTDIR\node-v${NodeJSNew}-x64.msi"' $0
      StrCmp $0 0 nInstallSucc 0
      StrCmp $0 1602 nInstallSucc 0
      MessageBox MB_OK|MB_ICONEXCLAMATION "Installation failed, because NodeJS\
      installer exited with exit code $0. Please download and install NodeJS \
        (at least v${NodeJSRequired}) manually. Click OK to exit the installer" /SD IDOK
      Quit
  ${EndIf}
  nInstallSucc:
SectionEnd

Section "-Puppeteer" SecPup
  SetOutPath "$CACHE_DIR"

  File "package.json"
  File "package-lock.json"

  nsExec::ExecToStack '"$SYSDIR\cmd.exe" /c npm i --prefix=$CACHE_DIR\ $CACHE_DIR\'
  Pop $0
  StrCmp $0 0 nInstallSucc 0
  StrCmp $0 1602 nInstallSucc 0
  MessageBox MB_OK|MB_ICONEXCLAMATION "Failed to install Puppeteer. Please install Puppeteer \
    manually. Click OK to exit the installer" /SD IDOK
  Quit
  nInstallSucc:

  Var /GLOBAL outVar

  ${IsLink} "$INSTDIR\node_modules" $outVar
  ${If} $outVar == 0
    RMDIR /r "$INSTDIR\node_modules"
  ${EndIf}

  ${CreateJunction} "$INSTDIR\node_modules" "$CACHE_DIR\node_modules" $outVar
  ${If} $outVar == 0
    MessageBox MB_OK|MB_ICONEXCLAMATION "Failed to create junction. Click OK to exit the installer" /SD IDOK
    Quit
  ${EndIf}
SectionEnd

  LangString DESC_SecCore ${LANG_ENGLISH} "Core of Surreal Docs"
  LangString DESC_SecNodeJS ${LANG_ENGLISH} "NodeJS v${NodeJSNew}. Do not select it \
    if you already have Node version higher than ${NodeJSRequired}. At least \
    NodeJS ${NodeJSRequired} is required for Surreal Docs to work"

  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCore} $(DESC_SecCore)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecNodeJS} $(DESC_SecNodeJS)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Uninstall"

  Delete "$INSTDIR\uninst.exe"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\Medelfor\SurrealDocs"

SectionEnd
