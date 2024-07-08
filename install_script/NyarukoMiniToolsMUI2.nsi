; 安装程序初始定义常量
!define PRODUCT_NAME "NyarukoMiniTools"
; "x86" "x64"
!define BIT "x86"
; "Release" "x64\Release"
!define RDIR "Release"
!define PRODUCT_VERSION "1.1.0"
!define PRODUCT_PUBLISHER "KagurazakaYashi"
!define PRODUCT_WEB_SITE "https://github.com/kagurazakayashi/NyarukoMiniTools"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\datetime.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; 界面定义
!include "MUI2.nsh"

; MUI 预定义常量
; !define MUI_ABORTWARNING
; !define MUI_ICON "1.ico"
; !define MUI_UNICON "1.ico"

; 语言选择窗口常量设置
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; 欢迎页面
!insertmacro MUI_PAGE_WELCOME
; 设置要安装什么
!insertmacro MUI_PAGE_COMPONENTS
; 安装目录选择页面
!insertmacro MUI_PAGE_DIRECTORY
; 安装过程页面
!insertmacro MUI_PAGE_INSTFILES
; 安装完成页面
!define MUI_FINISHPAGE_RUN "$INSTDIR"
!insertmacro MUI_PAGE_FINISH
; 安装卸载过程页面
!insertmacro MUI_UNPAGE_INSTFILES
; 安装界面包含的语言设置
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "SimpChinese"
; 安装预释放文件
!insertmacro MUI_RESERVEFILE_LANGDLL
; !insertmacro MUI_RESERVEFILE_INSTALLOPTIONS
; 界面定义结束

Name "${PRODUCT_NAME} ${BIT} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}_${BIT}_${PRODUCT_VERSION}_Installer.exe"
InstallDir "$PROGRAMFILES\NyarukoMiniTools"
Icon "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
UninstallIcon "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
LicenseData "..\LICENSE"
ShowInstDetails show
ShowUnInstDetails show

Section "Document" SEC00
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\README.md"
  File "..\LICENSE"
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

Section "Debug Database" SEC10
  File "..\${RDIR}\tspwd.pdb"
  File "..\${RDIR}\tee.pdb"
  File "..\${RDIR}\sleep.pdb"
  File "..\${RDIR}\scrctl.pdb"
  File "..\${RDIR}\repstr.pdb"
  File "..\${RDIR}\reflex.pdb"
  File "..\${RDIR}\pwgen.pdb"
  File "..\${RDIR}\null.pdb"
  File "..\${RDIR}\datetime.pdb"
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
  Delete "$INSTDIR\tspwd.exe"
  Delete "$INSTDIR\tee.exe"
  Delete "$INSTDIR\sleep.exe"
  Delete "$INSTDIR\scrctl.exe"
  Delete "$INSTDIR\repstr.exe"
  Delete "$INSTDIR\reflex.exe"
  Delete "$INSTDIR\pwgen.exe"
  Delete "$INSTDIR\null.exe"
  Delete "$INSTDIR\datetime.exe"
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

; 所有 Function 区段必须放置在 Section 区段之后编写

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Function .onInstSuccess
  ExecShell "open" "$INSTDIR"
FunctionEnd

; 卸载成功函数
Function un.onUninstSuccess
  MessageBox MB_ICONINFORMATION "Uninstallation successful."
FunctionEnd

; 卸载程序初始化函数
Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO "Are you sure you want to uninstall?" IDYES +2
  Abort
FunctionEnd
