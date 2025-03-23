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
        printf("CTRL+C pressed, exiting...\n");
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
        printf("Console size: %d columns, %d rows\n", columns, rows);
    }
    else {
        printf("Error getting console size\n");
    }
}

void printUsage() {
    wprintf(L"Usage: program_name [-n port_name] [-b baud_rate] [-s byte_size] [-t stop_bits] [-p parity] [-o timeout]\n");
}

int stricmp(const char *a, const char *b) {
    while (*a && *b && tolower((unsigned char)*a) == tolower((unsigned char)*b)) {
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

void adjustPortName(char **portName) {
    if (strncmp(*portName, "COM", 3) == 0) {
        char *adjustedName = malloc(strlen("\\\\.\\") + strlen(*portName) + 1);
        strcpy(adjustedName, "\\\\.\\");
        strcat(adjustedName, *portName);
        *portName = adjustedName;
    } else if (strncmp(*portName, "tty", 3) == 0) {
        char *adjustedName = malloc(strlen("/dev/") + strlen(*portName) + 1);
        strcpy(adjustedName, "/dev/");
        strcat(adjustedName, *portName);
        *portName = adjustedName;
    } else if (strncmp(*portName, "USB", 3) == 0) {
        char *adjustedName = malloc(strlen("/dev/tty") + strlen(*portName) + 1);
        strcpy(adjustedName, "/dev/tty");
        strcat(adjustedName, *portName);
        *portName = adjustedName;
    }
}

int parseArguments(int argc, char *argv[], char **portName, int *baudRate, int *byteSize, int *stopBits, int *parity, int *timeout) {
    for (int i = 1; i < argc; i++) {
        if ((argv[i][0] == '-' || argv[i][0] == '/') && i + 1 < argc) {
            if (stricmp(argv[i] + 1, "n") == 0) {
                *portName = argv[i + 1];
            } else if (stricmp(argv[i] + 1, "b") == 0) {
                *baudRate = atoi(argv[i + 1]);
            } else if (stricmp(argv[i] + 1, "s") == 0) {
                *byteSize = atoi(argv[i + 1]);
            } else if (stricmp(argv[i] + 1, "t") == 0) {
                *stopBits = atoi(argv[i + 1]);
            } else if (stricmp(argv[i] + 1, "p") == 0) {
                *parity = atoi(argv[i + 1]);
            } else if (stricmp(argv[i] + 1, "o") == 0) {
                *timeout = atoi(argv[i + 1]);
            } else {
                printUsage();
                return 1;
            }
            i++;
        } else {
            printUsage();
            return 1;
        }
    }
    return 0;
}

HANDLE configureSerialPort(char *portName, int baudRate, int byteSize, int stopBits, int parity, int timeout) {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    hSerial = CreateFileA(
        portName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        wprintf(L"Error opening serial port\n");
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        wprintf(L"Error getting serial port state\n");
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = byteSize;
    dcbSerialParams.StopBits = stopBits;
    dcbSerialParams.Parity = parity;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        wprintf(L"Error setting serial port state\n");
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    timeouts.ReadIntervalTimeout = timeout;
    timeouts.ReadTotalTimeoutConstant = timeout;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = timeout;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        wprintf(L"Error setting serial port timeouts\n");
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    return hSerial;
}

void processSerialData(HANDLE hSerial) {
    wchar_t buffer[BUFFER_SIZE] = {0};
    DWORD bytesRead;
    wchar_t output[BUFFER_SIZE] = {0};
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
            } else if (c == '\r' || c == '\n') {
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
            } else if (c == '\t') {
                DWORD bytesWritten;
                for (int i = 0; i < TAB_WIDTH; ++i) {
                    WriteFile(hSerial, L" ", sizeof(wchar_t), &bytesWritten, NULL);
                }
                bufferIndex++;
                if (bufferIndex >= BUFFER_SIZE) {
                    bufferIndex = BUFFER_SIZE - 1;
                }
            } else if (c == '\x1b' && bufferIndex + 2 < BUFFER_SIZE) {
                if (!ReadFile(hSerial, buffer + bufferIndex + 1, 2 * sizeof(wchar_t), &bytesRead, NULL)) {
                    wprintf(L"Error reading from serial port\n");
                    break;
                }
                if (bytesRead >= 2 * sizeof(wchar_t) && buffer[bufferIndex + 1] == L'[') {
                    if (buffer[bufferIndex + 2] == L'A') {
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
                            for (int i = 0; i < BUFFER_SIZE; ++i) WriteFile(hSerial, L" ", sizeof(wchar_t), &bytesWritten, NULL);
                            WriteFile(hSerial, L"\r", sizeof(wchar_t), &bytesWritten, NULL);
                            WriteFile(hSerial, buffer, bufferIndex * sizeof(wchar_t), &bytesWritten, NULL);
                        }
                    }
                }
            } else {
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
                } else {
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
}

int main(int argc, char *argv[]) {
    _wsetlocale(LC_ALL, L"");

    if (!SetConsoleCtrlHandler(signalHandler, TRUE)) {
        wprintf(L"Error setting up signal handler\n");
        return 1;
    }

    printConsoleSize();

    char *portName = "COM1";
    int baudRate = CBR_9600;
    int byteSize = 8;
    int stopBits = ONESTOPBIT;
    int parity = NOPARITY;
    int timeout = 50;

    if (parseArguments(argc, argv, &portName, &baudRate, &byteSize, &stopBits, &parity, &timeout)) {
        return 1;
    }

    adjustPortName(&portName);

    HANDLE hSerial = configureSerialPort(portName, baudRate, byteSize, stopBits, parity, timeout);
    if (hSerial == INVALID_HANDLE_VALUE) {
        return 1;
    }

    processSerialData(hSerial);

    CloseHandle(hSerial);
    return 0;
}
