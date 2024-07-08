#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

#define BUFFER_SIZE 1024
#define TAB_WIDTH 4
#define HISTORY_SIZE 10
#define MAX_CHAR_SIZE 4 // 假设最大字符大小为4字节

volatile int keepRunning = 1;
wchar_t commandHistory[HISTORY_SIZE][BUFFER_SIZE];
int historyIndex = 0;
int historyCount = 0;

BOOL WINAPI signalHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        wprintf(L"CTRL+C pressed, exiting...\n");
        keepRunning = 0;
        return TRUE;
    }
    return FALSE;
}

void executeCommand(const wchar_t* command, wchar_t* output, DWORD outputSize) {
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    HANDLE hRead, hWrite;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        _snwprintf_s(output, outputSize, _TRUNCATE, L"Error creating pipe\n");
        return;
    }

    STARTUPINFO si;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdOutput = hWrite;
    si.hStdError = hWrite;
    si.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    if (!CreateProcess(NULL, (LPWSTR)command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        _snwprintf_s(output, outputSize, _TRUNCATE, L"Error creating process\n");
        CloseHandle(hWrite);
        CloseHandle(hRead);
        return;
    }

    CloseHandle(hWrite);

    DWORD bytesRead;
    wchar_t buffer[BUFFER_SIZE];
    while (ReadFile(hRead, buffer, sizeof(buffer) - sizeof(wchar_t), &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead / sizeof(wchar_t)] = '\0';
        wcsncat_s(output, outputSize, buffer, _TRUNCATE);
    }

    CloseHandle(hRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void addCommandToHistory(const wchar_t* command) {
    if (historyCount < HISTORY_SIZE) {
        wcsncpy_s(commandHistory[historyCount++], BUFFER_SIZE, command, _TRUNCATE);
    }
    else {
        for (int i = 1; i < HISTORY_SIZE; ++i) {
            wcsncpy_s(commandHistory[i - 1], BUFFER_SIZE, commandHistory[i], _TRUNCATE);
        }
        wcsncpy_s(commandHistory[HISTORY_SIZE - 1], BUFFER_SIZE, command, _TRUNCATE);
    }
    historyIndex = historyCount;
}

void printConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        wprintf(L"Console size: %d columns, %d rows\n", columns, rows);
    }
    else {
        wprintf(L"Error getting console size\n");
    }
}

int main() {
    _wsetlocale(LC_ALL, L"");

    if (!SetConsoleCtrlHandler(signalHandler, TRUE)) {
        wprintf(L"Error setting up signal handler\n");
        return 1;
    }

    printConsoleSize();

    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };

    hSerial = CreateFile(
        L"\\\\.\\COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        wprintf(L"Error opening serial port\n");
        return 1;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        wprintf(L"Error getting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        wprintf(L"Error setting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        wprintf(L"Error setting serial port timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    wchar_t buffer[BUFFER_SIZE] = { 0 };
    DWORD bytesRead;
    wchar_t output[BUFFER_SIZE] = { 0 };
    int bufferIndex = 0;
    int historyNavigateIndex = -1;

    while (keepRunning) {
        if (!ReadFile(hSerial, buffer + bufferIndex, sizeof(wchar_t), &bytesRead, NULL)) {
            wprintf(L"Error reading from serial port\n");
            break;
        }
        if (bytesRead > 0) {
            wchar_t c = buffer[bufferIndex];
            if (c == '\b' || c == 127) {
                if (bufferIndex > 0) {
                    bufferIndex--;
                    DWORD bytesWritten;
                    wchar_t backspace[] = L"\b \b";
                    WriteFile(hSerial, backspace, sizeof(backspace) - sizeof(wchar_t), &bytesWritten, NULL);
                }
            }
            else if (c == '\r' || c == '\n') {
                buffer[bufferIndex] = '\0';
                wprintf(L"Received command: %s\n", buffer);

                if (wcsncmp(buffer, L"exit", 4) == 0) {
                    break;
                }

                memset(output, 0, sizeof(output));
                addCommandToHistory(buffer);
                executeCommand(buffer, output, sizeof(output));
                DWORD bytesWritten;
                WriteFile(hSerial, L"\r\n", 2 * sizeof(wchar_t), &bytesWritten, NULL);
                WriteFile(hSerial, output, wcslen(output) * sizeof(wchar_t), &bytesWritten, NULL);

                bufferIndex = 0;
                historyNavigateIndex = -1;
            }
            else if (c == '\t') {
                DWORD bytesWritten;
                for (int i = 0; i < TAB_WIDTH; ++i) {
                    WriteFile(hSerial, L" ", sizeof(wchar_t), &bytesWritten, NULL);
                }
                bufferIndex++;
                if (bufferIndex >= BUFFER_SIZE) {
                    bufferIndex = BUFFER_SIZE - 1;
                }
            }
            else if (c == '\x1b' && bufferIndex + 2 < BUFFER_SIZE) { // Escape sequence
                if (!ReadFile(hSerial, buffer + bufferIndex + 1, 2 * sizeof(wchar_t), &bytesRead, NULL)) {
                    wprintf(L"Error reading from serial port\n");
                    break;
                }
                if (bytesRead >= 2 * sizeof(wchar_t) && buffer[bufferIndex + 1] == L'[') {
                    if (buffer[bufferIndex + 2] == L'A') { // 上箭头
                        if (historyNavigateIndex == -1) {
                            historyNavigateIndex = historyIndex;
                        }
                        if (historyNavigateIndex > 0) {
                            historyNavigateIndex--;
                            bufferIndex = wcslen(commandHistory[historyNavigateIndex]);
                            wcsncpy_s(buffer, BUFFER_SIZE, commandHistory[historyNavigateIndex], bufferIndex);
                            buffer[bufferIndex] = '\0';
                            DWORD bytesWritten;
                            WriteFile(hSerial, L"\r", sizeof(wchar_t), &bytesWritten, NULL);
                            for (int i = 0; i < BUFFER_SIZE; ++i) WriteFile(hSerial, L" ", sizeof(wchar_t), &bytesWritten, NULL); // Clear line
                            WriteFile(hSerial, L"\r", sizeof(wchar_t), &bytesWritten, NULL);
                            WriteFile(hSerial, buffer, bufferIndex * sizeof(wchar_t), &bytesWritten, NULL);
                        }
                    }
                }
            }
            else {
                int charLen = mblen((const char*)&c, MAX_CHAR_SIZE);
                if (charLen > 1) {
                    wchar_t multiByteChar[MAX_CHAR_SIZE];
                    if (!ReadFile(hSerial, buffer + bufferIndex + 1, (charLen - 1) * sizeof(wchar_t), &bytesRead, NULL)) {
                        wprintf(L"Error reading from serial port\n");
                        break;
                    }
                    wcsncpy_s(multiByteChar, sizeof(multiByteChar) / sizeof(wchar_t), buffer + bufferIndex, charLen);
                    multiByteChar[charLen] = '\0';

                    DWORD bytesWritten;
                    WriteFile(hSerial, multiByteChar, charLen * sizeof(wchar_t), &bytesWritten, NULL);
                    bufferIndex += charLen;

                    if (bufferIndex >= BUFFER_SIZE) {
                        bufferIndex = BUFFER_SIZE - 1;
                    }
                }
                else {
                    DWORD bytesWritten;
                    WriteFile(hSerial, buffer + bufferIndex, sizeof(wchar_t), &bytesWritten, NULL);
                    bufferIndex++;
                    if (bufferIndex >= BUFFER_SIZE) {
                        bufferIndex = BUFFER_SIZE - 1;
                    }
                }
            }
        }
    }

    CloseHandle(hSerial);
    return 0;
}
