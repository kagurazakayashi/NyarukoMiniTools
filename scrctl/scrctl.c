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


/**
 * @brief 監控電源模式
 *
 * 此函式用於調整顯示器的電源模式。
 *
 * @param powerMode 指定電源模式。可以為下列值之一：
 * -1：顯示器關閉
 * 1：顯示器進入省電模式
 * 2：顯示器恢復全功率模式
 */
void monitorPower(int powerMode)
{
#ifdef _WIN32
	PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, powerMode);
#endif
}


/**
 * @brief 主函式
 *
 * 此函式用於處理命令列參數。
 *
 * @param argc 命令列參數的數量
 * @param argv 命令列參數的陣列
 * @return 程式的結束狀態碼
 */
int main(int argc, char* argv[])
{
#ifndef _WIN32
    printf("This program only supports Windows.\n");
	return 1;
#endif
    int powerMode = 2;
    if (argc >= 2) {
        char* aStr = argv[1];
        if (strcmp(aStr, "/?") == 0 || strcmp(aStr, "--help") == 0)
        {
            printf("Usage: SCRCTL [MODE]\n");
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
            printf("scrctl 1.0.1\n");
            printf("Written by Kagurazaka Yashi. https://github.com/kagurazakayashi/NyarukoMiniTools");
            printf("License Mulan PSL v2: http://license.coscl.org.cn/MulanPSL2\n");
            printf("This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.\n");
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
#ifdef _WIN32
            system("rundll32.exe user32.dll, LockWorkStation");
#endif
        }
    }
    monitorPower(powerMode);
    return 0;
}
