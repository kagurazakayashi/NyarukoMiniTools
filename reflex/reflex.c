/*
Copyright (c) 2021 KagurazakaYashi
reflex is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
*/
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

/**
 * 處理接收到的訊號
 * @param sig 接收到的訊號代碼
 */
void sigHandle(int sig) {
    switch (sig) {
    case SIGINT:
        printf("sigHandle: %d, SIGINT\n", sig); // 處理 SIGINT 訊號
        break;
    default:
        printf("sigHandle: %d\n", sig); // 處理其他訊號
        break;
    }
    exit(sig); // 結束程序並回傳訊號代碼
}


/**
 * 休眠指定的毫秒數
 * @param dwMillisrconds 休眠時間，單位為毫秒
 */
void sleepNow(int dwMillisrconds)
{
#if defined(__linux__) || defined(macintosh)
    usleep(dwMillisrconds * 1000); // 在 Linux 或 macOS 上休眠
#endif
#ifdef _WIN32
    Sleep(dwMillisrconds); // 在 Windows 上休眠
#endif
}


/**
 * 持續讀取使用者輸入的字串並顯示
 */
void ref()
{
    int i = 0;
    char str[1001] = { '\0' };
    while (1)
    {
        int scanfcode = scanf("%1000s", &str); // 讀取最多 1000 個字符的字串
        fflush(stdin); // 清空輸入緩衝區
        if (scanfcode != 1) {
            continue; // 如果讀取失敗，繼續下一次讀取
        }
        printf("%s\n", &str); // 顯示讀取到的字串
        fflush(stdout); // 清空輸出緩衝區
        for (i = 0; i < 1001; i++)
        {
            str[i] = '\0'; // 清空字串
        }
    }
}


/**
 * 持續顯示當前時間，每秒更新一次
 */
void ltime()
{
    int oldsec = -1;
    while (1)
    {
        time_t timep;
        struct tm p;
        time(&timep); // 獲取當前時間
#ifdef _WIN32
        localtime_s(&p, &timep); // 在 Windows 上使用 localtime_s
#else
        struct tm* p_localtime;
        p_localtime = localtime(&timep); // 轉換為本地時間
        p = *p_localtime; // 複製結構體內容
#endif
        if (oldsec == p.tm_sec) {
            continue; // 如果秒數未變化，繼續下一次迴圈
        }
        printf("%4d-%02d-%02d %02d:%02d:%02d\n", 1900 + p.tm_year, 1 + p.tm_mon, p.tm_mday, p.tm_hour, p.tm_min, p.tm_sec); // 顯示當前時間
        oldsec = p.tm_sec; // 更新舊的秒數
        fflush(stdout); // 清空輸出緩衝區
        sleepNow(100); // 休眠 100 毫秒
    }
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
 * 主函式
 * @param argc 命令列引數的數量
 * @param argv 命令列引數的陣列
 * @return 程式的結束狀態碼
 */
int main(int argc, char* argv[])
{
    char* aStr = argv[1];
	signal(SIGINT, sigHandle);
	if (argc >= 2) {
		if (strcmp(aStr, "/?") == 0 || argcmp(aStr, 'H') == 1 || strcmp(aStr, "--help") == 0) {
			printf("Usage: REFLEX [MODE]\n");
			printf("MODE supported:\n");
			printf("    (null) : Input as is, output as is. (lenth <= 1000)\n");
			printf("    /T : output the current time every second.\n");
			printf("    /? : display this help and exit.\n");
			printf("    /V : output version information and exit.\n");
			printf("    (other) : output parameters.\n");
			return 0;
		}
		else if (argcmp(aStr, 'V') == 1 || strcmp(aStr, "--version") == 0)
		{
			printf("reflex 1.1.0\n");
			printf("Written by Kagurazaka Yashi. https://github.com/kagurazakayashi/NyarukoMiniTools\n");
			printf("License Mulan PSL v2: http://license.coscl.org.cn/MulanPSL2\n");
			printf("This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.\n");
			return 0;
		}
		else if (argcmp(aStr, 'T') == 1 || strcmp(aStr, "--time") == 0)
		{
			ltime();
		}
		else
		{
			for (int i = 1; i < argc; i++)
			{
				printf("%s\n", argv[i]);
			}
			return 0;
		}
	}
	ref();
	return 0;
}
