#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

#define BUFFER_SIZE 1024
#define TAB_WIDTH 4
#define HISTORY_SIZE 10

volatile int keepRunning = 1;
char commandHistory[HISTORY_SIZE][BUFFER_SIZE];
int historyIndex = 0;
int historyCount = 0;

BOOL WINAPI signalHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        printf("CTRL+C pressed, exiting...\n");
        keepRunning = 0;
        return TRUE;
    }
    return FALSE;
}

void executeCommand(const char* command, char* output, DWORD outputSize) {
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    HANDLE hRead, hWrite;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        snprintf(output, outputSize, "Error creating pipe\n");
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

    if (!CreateProcess(NULL, (LPSTR)command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        snprintf(output, outputSize, "Error creating process\n");
        CloseHandle(hWrite);
        CloseHandle(hRead);
        return;
    }

    CloseHandle(hWrite);

    DWORD bytesRead;
    char buffer[BUFFER_SIZE];
    while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        strncat(output, buffer, outputSize - strlen(output) - 1);
    }

    CloseHandle(hRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void addCommandToHistory(const char* command) {
    if (historyCount < HISTORY_SIZE) {
        strcpy(commandHistory[historyCount++], command);
    }
    else {
        for (int i = 1; i < HISTORY_SIZE; ++i) {
            strcpy(commandHistory[i - 1], commandHistory[i]);
        }
        strcpy(commandHistory[HISTORY_SIZE - 1], command);
    }
    historyIndex = historyCount;
}

void printConsoleSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        printf("Console size: %d columns, %d rows\n", columns, rows);
    }
    else {
        printf("Error getting console size\n");
    }
}

int main() {
    setlocale(LC_ALL, "");

    if (!SetConsoleCtrlHandler(signalHandler, TRUE)) {
        printf("Error setting up signal handler\n");
        return 1;
    }

    printConsoleSize();

    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };

    hSerial = CreateFile(
        "\\\\.\\COM8", GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error opening serial port\n");
        return 1;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("Error getting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("Error setting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("Error setting serial port timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    char buffer[BUFFER_SIZE] = { 0 };
    DWORD bytesRead;
    char output[BUFFER_SIZE] = { 0 };
    int bufferIndex = 0;
    int historyNavigateIndex = -1;

    while (keepRunning) {
        if (ReadFile(hSerial, buffer + bufferIndex, 1, &bytesRead, NULL) && bytesRead > 0) {
            char c = buffer[bufferIndex];
            if (c == '\b' || c == 127) {
                if (bufferIndex > 0) {
                    bufferIndex--;
                    DWORD bytesWritten;
                    char backspace[] = "\b \b";
                    WriteFile(hSerial, backspace, sizeof(backspace) - 1, &bytesWritten, NULL);
                }
            }
            else if (c == '\r' || c == '\n') {
                buffer[bufferIndex] = '\0';
                printf("Received command: %s\n", buffer);

                if (strncmp(buffer, "exit", 4) == 0) {
                    break;
                }

                memset(output, 0, sizeof(output));
                addCommandToHistory(buffer);
                executeCommand(buffer, output, sizeof(output));
                DWORD bytesWritten;
                WriteFile(hSerial, "\r\n", 2, &bytesWritten, NULL);
                WriteFile(hSerial, output, strlen(output), &bytesWritten, NULL);

                bufferIndex = 0;
                historyNavigateIndex = -1;
            }
            else if (c == '\t') {
                DWORD bytesWritten;
                for (int i = 0; i < TAB_WIDTH; ++i) {
                    WriteFile(hSerial, " ", 1, &bytesWritten, NULL);
                }
                bufferIndex++;
                if (bufferIndex >= BUFFER_SIZE) {
                    bufferIndex = BUFFER_SIZE - 1;
                }
            }
            else if (c == '\x1b') { // Escape sequence
                ReadFile(hSerial, buffer + bufferIndex + 1, 2, &bytesRead, NULL);
                if (buffer[bufferIndex + 1] == '[') {
                    if (buffer[bufferIndex + 2] == 'A') { // 上箭头
                        if (historyNavigateIndex == -1) {
                            historyNavigateIndex = historyIndex;
                        }
                        if (historyNavigateIndex > 0) {
                            historyNavigateIndex--;
                            bufferIndex = strlen(commandHistory[historyNavigateIndex]);
                            strncpy(buffer, commandHistory[historyNavigateIndex], bufferIndex);
                            buffer[bufferIndex] = '\0';
                            DWORD bytesWritten;
                            WriteFile(hSerial, "\r", 1, &bytesWritten, NULL);
                            for (int i = 0; i < BUFFER_SIZE; ++i) WriteFile(hSerial, " ", 1, &bytesWritten, NULL); // Clear line
                            WriteFile(hSerial, "\r", 1, &bytesWritten, NULL);
                            WriteFile(hSerial, buffer, bufferIndex, &bytesWritten, NULL);
                        }
                    }
                }
            }
            else {
                int charLen = mblen(&c, MB_CUR_MAX);
                if (charLen > 1) {
                    char multiByteChar[MB_CUR_MAX];
                    ReadFile(hSerial, buffer + bufferIndex + 1, charLen - 1, &bytesRead, NULL);
                    strncpy(multiByteChar, buffer + bufferIndex, charLen);
                    multiByteChar[charLen] = '\0';

                    DWORD bytesWritten;
                    WriteFile(hSerial, multiByteChar, charLen, &bytesWritten, NULL);
                    bufferIndex += charLen;

                    if (bufferIndex >= BUFFER_SIZE) {
                        bufferIndex = BUFFER_SIZE - 1;
                    }
                }
                else {
                    DWORD bytesWritten;
                    WriteFile(hSerial, buffer + bufferIndex, 1, &bytesWritten, NULL);
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
