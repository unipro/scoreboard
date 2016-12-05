copy /Y "Release\scoreboard.exe" "..\nsis\"
copy /Y "Release\wxbase30u_vc90.dll" "..\nsis\"
copy /Y "Release\wxmsw30u_core_vc90.dll" "..\nsis\"
copy /Y "Release\wxmsw30u_adv_vc90.dll" "..\nsis\"
copy /Y "Release\SDL2.dll" "..\nsis\"
copy /Y "..\wx\scoreboard.ico" "..\nsis\"
cd "..\nsis"
"c:\Program Files (x86)\NSIS\makensis.exe" setup.nsi
@pause
