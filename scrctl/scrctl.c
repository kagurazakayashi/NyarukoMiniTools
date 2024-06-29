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
 * @brief 比較給定字串中的每個字元是否與指定字元匹配
 *
 * 此函式會檢查給定字串中的每個字元，並比較它是否與指定的字元k匹配。
 * 如果匹配，則返回1；否則返回0。
 *
 * @param str 欲檢查的字串
 * @param k 欲匹配的字元（必須為大寫 char）
 * @return int 如果找到匹配的字元返回1，否則返回0
 */
int argcmp(const char *str, char k) {
    // 迴圈遍歷字串中的每個字元
    while (*str) {
        // 取得字串中當前字元的下一個字元
        char c = *(str + 1);
        // 如果字元是小寫字母，將其轉換為大寫
        if (c >= 'a' && c <= 'z') {
            c -= ('a' - 'A');
        }
        // 檢查當前字元是否為 '/' 或 '-' ，且下一個字元是否等於 k
        if ((*str == '/' || *str == '-') && c == k) {
            return 1; // 如果匹配，返回1
        }
        // 移動到字串中的下一個字元
        str++;
    }
    // 如果未找到匹配的字元，返回0
    return 0;
}


/**
 * @brief 主函式
 *
 * 此函式用於處理命令列引數。
 *
 * @param argc 命令列引數的數量
 * @param argv 命令列引數的陣列
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
        if (strcmp(aStr, "/?") == 0 || argcmp(aStr, 'H') == 1 || strcmp(aStr, "--help") == 0)
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
        if (argcmp(aStr, 'V') == 1 || strcmp(aStr, "--version") == 0)
        {
            printf("scrctl 1.0.1\n");
            printf("Written by Kagurazaka Yashi. https://github.com/kagurazakayashi/NyarukoMiniTools\n");
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
