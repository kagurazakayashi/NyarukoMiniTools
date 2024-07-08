!define PRODUCT_NAME "NyarukoMiniTools"

!define BIT "x86"
!define RDIR "Release"

; !define BIT "x64"
; !define RDIR "x64\Release"

; !define BIT "arm32"
; !define RDIR "ARM\Release"

; !define BIT "arm64"
; !define RDIR "ARM64\Release"

!define PRODUCT_VERSION "1.2.0"
!define PRODUCT_PUBLISHER "KagurazakaYashi"
!define PRODUCT_WEB_SITE "https://github.com/kagurazakayashi/NyarukoMiniTools"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\datetime.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor /FINAL /SOLID lzma

Name "${PRODUCT_NAME} ${BIT} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}_${BIT}_${PRODUCT_VERSION}_Installer.exe"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\English.nlf"
LoadLanguageFile "${NSISDIR}\Contrib\Language files\SimpChinese.nlf"
InstallDir "$PROGRAMFILES\NyarukoMiniTools"
Icon "icon.ico"
UninstallIcon "icon.ico"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ComponentText "$(^Name)"
DirText "$(^Name)"
LicenseText "$(^Name)"
LicenseData "..\LICENSE"
ShowInstDetails show
ShowUnInstDetails show

Section "Document" SEC0A
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\README.md"
  File "NyarukoMiniTools.url"
  File "..\LICENSE"
SectionEnd

Section "Debug PDBs" SEC0B
  File "..\${RDIR}\tspwd.pdb"
  File "..\${RDIR}\tee.pdb"
  File "..\${RDIR}\sleep.pdb"
  File "..\${RDIR}\scrctl.pdb"
  File "..\${RDIR}\repstr.pdb"
  File "..\${RDIR}\reflex.pdb"
  File "..\${RDIR}\pwgen.pdb"
  File "..\${RDIR}\null.pdb"
  File "..\${RDIR}\datetime.pdb"
  File "..\${RDIR}\datetime.pdb"
  File "..\${RDIR}\path.exe.config"
SectionEnd

Section "datetime" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\${RDIR}\datetime.exe"
SectionEnd

Section "null" SEC02
  File "..\${RDIR}\null.exe"
SectionEnd

Section "pwgen" SEC03
  File "..\${RDIR}\pwgen.exe"
SectionEnd

Section "reflex" SEC04
  File "..\${RDIR}\reflex.exe"
SectionEnd

Section "repstr" SEC05
  File "..\${RDIR}\repstr.exe"
SectionEnd

Section "scrctl" SEC06
  File "..\${RDIR}\scrctl.exe"
  CreateShortCut "$DESKTOP\Turn off the screen.lnk" "$INSTDIR\scrctl.exe"
  CreateDirectory "$SMPROGRAMS\NyarukoMiniTools"
  CreateShortCut "$SMPROGRAMS\NyarukoMiniTools\Turn off the screen.lnk" "$INSTDIR\scrctl.exe"
SectionEnd

Section "sleep" SEC07
  File "..\${RDIR}\sleep.exe"
SectionEnd

Section "tee" SEC08
  File "..\${RDIR}\tee.exe"
SectionEnd

Section "tspwd" SEC09
  File "..\${RDIR}\tspwd.exe"
SectionEnd

Section "chkascii" SEC10
  File "..\${RDIR}\chkascii.exe"
SectionEnd

Section "path" SEC11
  File "..\${RDIR}\path.exe"
  File "add_path.bat"
  File "del_path.bat"
  File "turn_off_screen_after_5s.bat"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateDirectory "$SMPROGRAMS\NyarukoMiniTools"
  CreateShortCut "$DESKTOP\NyarukoMiniTools.lnk" "$INSTDIR"
  CreateShortCut "$SMPROGRAMS\NyarukoMiniTools\NyarukoMiniTools.lnk" "$INSTDIR"
  CreateShortCut "$SMPROGRAMS\NyarukoMiniTools\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\NyarukoMiniTools\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\datetime.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\datetime.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\datetime.pdb"
  Delete "$INSTDIR\null.pdb"
  Delete "$INSTDIR\pwgen.pdb"
  Delete "$INSTDIR\reflex.pdb"
  Delete "$INSTDIR\repstr.pdb"
  Delete "$INSTDIR\scrctl.pdb"
  Delete "$INSTDIR\sleep.pdb"
  Delete "$INSTDIR\tee.pdb"
  Delete "$INSTDIR\tspwd.pdb"
  Delete "$INSTDIR\path.exe.config"
  Delete "$INSTDIR\tspwd.exe"
  Delete "$INSTDIR\tee.exe"
  Delete "$INSTDIR\sleep.exe"
  Delete "$INSTDIR\scrctl.exe"
  Delete "$INSTDIR\repstr.exe"
  Delete "$INSTDIR\reflex.exe"
  Delete "$INSTDIR\pwgen.exe"
  Delete "$INSTDIR\null.exe"
  Delete "$INSTDIR\datetime.exe"
  Delete "$INSTDIR\chkascii.exe"
  Delete "$INSTDIR\path.exe"
  Delete "$INSTDIR\add_path.bat"
  Delete "$INSTDIR\del_path.bat"
  Delete "$INSTDIR\turn_off_screen_after_5s.bat"
  Delete "$INSTDIR\README.md"
  Delete "$INSTDIR\LICENSE"

  Delete "$SMPROGRAMS\NyarukoMiniTools\Uninstall.lnk"
  Delete "$SMPROGRAMS\NyarukoMiniTools\Website.lnk"
  Delete "$DESKTOP\Turn off the screen.lnk"
  Delete "$SMPROGRAMS\NyarukoMiniTools\Turn off the screen.lnk"

  RMDir "$SMPROGRAMS\NyarukoMiniTools"
  RMDir "$INSTDIR"
  RMDir ""

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

Function .onInit
FunctionEnd

Function .onInstSuccess
  ExecShell "open" "$INSTDIR"
  MessageBox MB_ICONQUESTION|MB_YESNO "Add the installation directory to the system PATH environment variable?  添加安装目录到系统 PATH 环境变量？" IDYES +2
  Return
  ExecWait '"$INSTDIR\path.exe" /A "$INSTDIR"'
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) Successfully removed.  已成功卸载。"
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Remove $(^Name) ?  卸载 $(^Name) ?" IDYES +2
  Abort
  ExecWait '"$INSTDIR\path.exe" /D "$INSTDIR"'
FunctionEnd
