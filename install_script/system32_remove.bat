for %%f in (*.exe) do (
    DEL /F /Q %SystemRoot%\system32\%%f
)
PAUSE