for %%f in (*.exe) do (
    COPY %%f %SystemRoot%\system32\%%f
)
PAUSE