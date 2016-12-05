; Installer for scoreboard
;
; Copyright 2014 scoreboard <scoreboard@naver.com>
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 3, or (at your option)
; any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.

!define APPNAME "scoreboard"
!define AUTHOR "scoreboard"
!define DESCRIPTION "Score your games"
!define VERSIONMAJOR 0
!define VERSIONMINOR 2
!define VERSIONMICRO 1

; Require admin rights on NT6+ (When UAC is turned on)
RequestExecutionLevel admin

InstallDir "$PROGRAMFILES\${APPNAME}"

LicenseData "License_GPL.txt"

; This will be in the installer/uninstaller's title bar
Name "${APPNAME}"
Icon "${APPNAME}.ico"
OutFile "${APPNAME}_${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONMICRO}_win32-setup.exe"

!include LogicLib.nsh

; Just three pages - license agreement, install location, and installation
page license
page directory
Page instfiles

!macro VerifyUserIsAdmin
UserInfo::GetAccountType
pop $0
${If} $0 != "admin" ; Require admin rights on NT4+
  messageBox mb_iconstop "Administrator rights required!"
  setErrorLevel 740 ; ERROR_ELEVATION_REQUIRED
  quit
${EndIf}
!macroend
 
function .onInit
  setShellVarContext all
  !insertmacro VerifyUserIsAdmin
functionEnd

section "install"
  ; Install directory
  setOutPath $INSTDIR

  ; Files
  file "${APPNAME}.exe"
  file "wxbase30u_vc90.dll"
  file "wxmsw30u_core_vc90.dll"
  file "wxmsw30u_adv_vc90.dll"
  file "SDL2.dll"
  file "${APPNAME}.ico"
  file "License_GPL.rtf"
  file "License_GPL.txt"

  ; Uninstaller
  writeUninstaller "$INSTDIR\uninstall.exe"

  ; Start Menu
  createDirectory "$SMPROGRAMS\${APPNAME}"
  createShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\${APPNAME}.exe" "" "$INSTDIR\${APPNAME}.exe" 0
 
  ; Registry information for add/remove programs
  writeRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
  writeRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
  writeRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
  writeRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "InstallLocation" "$\"$INSTDIR$\""
  writeRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayIcon" "$\"$INSTDIR\${APPNAME}.ico$\""
  writeRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "Publisher" "${AUTHOR}"
  writeRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayVersion" "${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONMICRO}"
  writeRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionMajor" ${VERSIONMAJOR}
  writeRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionMinor" ${VERSIONMINOR}
  # There is no option for modifying or repairing the install
  writeRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoModify" 1
  writeRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoRepair" 1
sectionEnd

; Uninstaller
function un.onInit
  setShellVarContext all
 
  #Verify the uninstaller - last chance to back out
  messageBox MB_OKCANCEL "Permanantly remove ${APPNAME}?" IDOK next
  abort
  next:
    !insertmacro VerifyUserIsAdmin
functionEnd

section "uninstall"
  ; Remove Start Menu launcher
  delete "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk"
  rmDir "$SMPROGRAMS\${APPNAME}"
 
  ; Remove files
  delete "$INSTDIR\${APPNAME}.exe"
  delete "$INSTDIR\wxbase30u_vc90.dll"
  delete "$INSTDIR\wxmsw30u_core_vc90.dll"
  delete "$INSTDIR\wxmsw30u_adv_vc90.dll"
  delete "$INSTDIR\SDL2.dll"
  delete "$INSTDIR\${APPNAME}.ico"
  delete "$INSTDIR\License_GPL.rtf"
  delete "$INSTDIR\License_GPL.txt"
  delete "$INSTDIR\uninstall.exe"

  ; Try to remove the install directory
  rmDir $INSTDIR
 
  ; Delete Uninstaller And Unistall Registry Entries
  deleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"  
sectionEnd
