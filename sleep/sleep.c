/*
Copyright (c) 2020 KagurazakaYashi
sleep-like is licensed under Mulan PSL v2.
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
#include <stdlib.h>
#if defined(__linux__) || defined(macintosh)
#include <time.h>
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @brief 使當前執行緒進入休眠狀態。
 *
 * 這個函式會讓當前執行緒暫停執行指定的時間。
 *
 * @param sec 要休眠的秒數。
 * @param display 是否顯示倒數計時。
 */
void sleepNow(int sec, int display)
{
	for (int i = 0; i < sec; i++)
	{
		if (display)
		{
			printf("%d ", sec - i);
		}
		//在 Linux 和 macOS 系統上，使用 usleep 函式來達成，usleep 需要將毫秒數轉換為微秒數。
#if defined(__linux__) || defined(macintosh)
		usleep(1000000); // 1秒 = 1000*1000=1000000 微秒
#endif
		// 在 Windows 系統上，使用 Sleep 函式來達成，Sleep 函式直接接受毫秒數。
#ifdef _WIN32
		Sleep(1000); // 1秒 = 1000 毫秒
#endif
		if (display && i >= sec - 1)
		{
			printf("0\n");
		}
	}
}


/**
 * @brief 使系統休眠指定的秒數，並可選擇性地顯示倒計時。
 *
 * 此函式根據不同的作業系統使用不同的方法來實現精確的休眠。
 * 在Linux和Mac系統上，使用nanosleep函式以高精度的方式進行休眠。
 * 在Windows系統上，使用QueryPerformanceCounter和QueryPerformanceFrequency函式來實現高精度的休眠。
 *
 * @param sec 等待的秒數。
 * @param display 若為非零，則顯示倒計時。
 * @param cpuSleep (Windows)一個短暫的睡眠，可以讓出CPU時間片給其他任務，從而減少CPU消耗。推薦1即可顯著降低CPU消耗。
 */
void sleepHighPrecision(int sec, int display, int cpuSleep)
{
	for (int i = 0; i < sec; i++)
	{
		if (display)
		{
			printf("%d ", sec - i); // 顯示剩餘秒數
		}

#if defined(__linux__) || defined(macintosh)
		// 在Linux和Mac系統上使用nanosleep進行精確休眠
		struct timespec ts;
		ts.tv_sec = 1;    // 設定秒數
		ts.tv_nsec = 0;   // 設定納秒數
		nanosleep(&ts, NULL); // 休眠1秒
#endif

#ifdef _WIN32
		// 在Windows系統上使用高精度計時器進行精確休眠
		LARGE_INTEGER frequency; // 計數器頻率
		LARGE_INTEGER start, end; // 開始和結束的計數值

		QueryPerformanceFrequency(&frequency); // 獲取計數器頻率
		QueryPerformanceCounter(&start); // 獲取當前計數值

		while (1)
		{
			QueryPerformanceCounter(&end); // 獲取當前計數值
			// 判斷是否達到1秒
			if ((end.QuadPart - start.QuadPart) >= frequency.QuadPart)
				break;
			// 稍微休眠一小段時間，減少忙等待
			Sleep(1);
		}
#endif

		if (display && i >= sec - 1)
		{
			printf("0\n"); // 顯示倒計時結束
		}
	}
}


/**
 * @brief 將時間字串轉換為毫秒數
 *
 * 此函式接收一個表示數字的字串和一個表示時間單位的字元，並將其轉換為毫秒數。
 *
 * @param num 表示數字的字串
 * @param unit 表示時間單位的字元，可以是 'm' (分鐘), 'h' (小時), 或 'd' (天)
 * @return double 對應的毫秒數
 */
int timeStrToSec(char* num, char unit)
{
	int total = 0; // 初始化總毫秒數為0
	int numi = atoi(num); // 將數字字串轉換為浮點數

	// 如果單位字元為大寫字母，則將其轉換為小寫字母
	if (unit >= 65 && unit <= 90)
	{
		unit = unit + 32;
	}

	// 根據單位字元，將時間轉換為秒數
	if (unit == 'm' || unit == 'h' || unit == 'd')
	{
		if (unit <= 'm')
		{
			numi *= 60; // 分鐘轉換為秒
		}
		if (unit <= 'h')
		{
			numi *= 60; // 小時轉換為秒
		}
		if (unit <= 'd')
		{
			numi *= 24; // 天轉換為秒
		}
	}

	total += numi; // 將轉換後的毫秒數累加到總數
	return total; // 返回總毫秒數
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
int argcmp(const char* str, char k) {
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
 * @brief 程式入口函式。
 *
 * 這個函式會解析命令列引數，並根據引數的值來決定休眠的時間。
 *
 * @param argc 命令列引數的數量。
 * @param argv 命令列引數的陣列。
 *
 * @return 程式的結束狀態。
 */
int main(int argc, char* argv[])
{
	int total = 0;
	int i = 1;
	int display = 0;
	int highPrecision = 0;
	if (argc < 2)
	{
		sleepNow(10, display);
		return 0;
	}
	char* aStr = argv[1];
	if (strcmp(aStr, "/?") == 0 || argcmp(aStr, 'H') == 1 || strcmp(aStr, "--help") == 0)
	{
		printf("Usage: SLEEP [OPTION] NUMBER[SUFFIX]\n");
		printf("Pause for NUMBER seconds. SUFFIX may be 's' for seconds (the default),\n");
		printf("'m' for minutes, 'h' for hours or 'd' for days.\n");
		printf("DEFAULT: 1s\n");
		printf("Here NUMBER may be an arbitrary floating point number.\n");
		printf("Given two or more arguments, pause for the amount of time specified by the sum of their values.\n");
		printf("    /D  display the remaining time (seconds) every second.\n");
		printf("    /H  Use high-precision timer (more CPU intensive). Not recommended to use with /D\n");
		printf("    /?  display this help and exit.\n");
		printf("    /V  output version information and exit.\n");
		return 0;
	}
	else if (argcmp(aStr, 'V') == 1 || strcmp(aStr, "--version") == 0)
	{
		printf("sleep-like 1.1.0\n");
		printf("Written by Kagurazaka Yashi. https://github.com/kagurazakayashi/NyarukoMiniTools\n");
		printf("License Mulan PSL v2: http://license.coscl.org.cn/MulanPSL2\n");
		printf("This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.\n");
		return 0;
	}
	if (argcmp(aStr, 'D') == 1 || strcmp(aStr, "--display") == 0)
	{
		display++;
		i++;
	}
	if (argcmp(aStr, 'H') == 1 || strcmp(aStr, "--high-precision") == 0)
	{
		highPrecision++;
		i++;
	}
	for (; i < argc; i++)
	{
		char* inNum = argv[i];
		int inLen = (int)strlen(inNum);
		char num[10] = { '\0' };
		char unit = 's';
		for (int j = 0; j < inLen; j++)
		{
			char nowchar = inNum[j];
			if ((nowchar >= 48 && nowchar <= 57) || nowchar == 46)
			{
				num[j] = nowchar;
			}
			else
			{
				unit = nowchar;
			}
		}
		if (num[0] == 0)
		{
			fprintf(stderr, "SLEEP: invalid time interval '%s'%s", inNum, "\nTry 'SLEEP /?' for more information.");
			return 1;
		}
		total = timeStrToSec(num, unit);
	}
	if (highPrecision >= 1)
	{
		sleepHighPrecision(total, display, highPrecision);
	}
	else
	{
		sleepNow(total, display);
	}
	return 0;
}
