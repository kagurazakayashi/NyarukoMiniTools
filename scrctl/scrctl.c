/*
Copyright (c) 2020 KagurazakaYashi
scrctl is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
*/
#include <stdio.h>
#include <windows.h>
int main(int argc, char* argv[])
{
    int powerMode = 2;
    if (argc >= 2) {
        char* aStr = argv[1];
        if (strcmp(aStr, "/?") == 0 || strcmp(aStr, "--help") == 0)
        {
            printf("Usage: scrctl [MODE]\n");
            printf("MODE:");
            printf("\n   off  or  /0  Power Off (default)");
            printf("\n    on  or  /1  Power On");
            printf("\n   eco  or  /2  Low Power Mode");
            printf("\n  lock  or  /3  Power Off and Lock");
            printf("\n    /?  display this help and exit");
            printf("\n    /V  output version information and exit");
            return 0;
        }
        if (strcmp(aStr, "/V") == 0 || strcmp(aStr, "--version") == 0)
        {
            printf("scrctl 1.0.0\n");
            printf("Copyright (C) KagurazakaYashi\n");
            printf("License Mulan PSL v2: <http://license.coscl.org.cn/MulanPSL2>.\n");
            printf("This is free software: you are free to change and redistribute it.\n");
            printf("There is NO WARRANTY, to the extent permitted by law.\n");
            printf("Written by Kagurazaka Yashi.");
            return 0;
        }
        if (strcmp(aStr, "/1") == 0 || strcmp(aStr, "on") == 0)
        {
            powerMode = -1;
        }
        else if (strcmp(aStr, "/2") == 0 || strcmp(aStr, "eco") == 0)
        {
            powerMode = 1;
        }
        else if (strcmp(aStr, "/3") == 0 || strcmp(aStr, "lock") == 0)
        {
            system("rundll32.exe user32.dll, LockWorkStation");
        }
    }
    PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, powerMode);
    return 0;
}