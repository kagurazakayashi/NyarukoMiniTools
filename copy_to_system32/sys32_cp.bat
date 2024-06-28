set nowdir=%~dp0
%nowdir:~0,2%
cd %nowdir%
COPY /B /V /Y datetime.exe %SystemRoot%\system32\
COPY /B /V /Y pwgen.exe %SystemRoot%\system32\
COPY /B /V /Y null.exe %SystemRoot%\system32\
COPY /B /V /Y reflex.exe %SystemRoot%\system32\
COPY /B /V /Y repstr.exe %SystemRoot%\system32\
COPY /B /V /Y scrctl.exe %SystemRoot%\system32\
COPY /B /V /Y tspwd.exe %SystemRoot%\system32\
set nowdir=
PAUSE