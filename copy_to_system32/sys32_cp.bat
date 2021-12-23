set nowdir=%~dp0
%nowdir:~0,2%
cd %nowdir%
COPY /B /V /Y datetime.exe %SystemRoot%\system32\
COPY /B /V /Y genpwd.exe %SystemRoot%\system32\
COPY /B /V /Y null.exe %SystemRoot%\system32\
COPY /B /V /Y reflection.exe %SystemRoot%\system32\
COPY /B /V /Y repstr.exe %SystemRoot%\system32\
COPY /B /V /Y scrctl.exe %SystemRoot%\system32\
COPY /B /V /Y sleep.exe %SystemRoot%\system32\
set nowdir=
PAUSE