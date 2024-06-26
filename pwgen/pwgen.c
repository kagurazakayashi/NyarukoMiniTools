﻿/*
Copyright (c) 2021 KagurazakaYashi
sleep is licensed under Mulan PSL v2.
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
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>
#if defined(__linux__) || defined(macintosh)
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

char allSelect[94] = { 0 };
unsigned int seedi = 0;

/**
 * @brief 判斷字元模式
 *
 * 這個函數根據傳入的字元返回一個短整數值，
 * 以表示該字元的模式。具體的模式如下：
 *  - 大寫字母 'N' 或小寫字母 'n' 返回 3
 *  - 大寫字母 'S' 或小寫字母 's' 返回 4
 *  - 其他大寫字母返回 1
 *  - 其他小寫字母返回 2
 *  - 非字母字符返回 0
 *
 * @param nowChar 指向要判斷的字元的指標
 * @return short 表示字元模式的數值
 */
short charMode(char* nowChar)
{
	// 將字元轉換為整數型態
	int nowCharI = (char)nowChar;

	// 判斷是否為字母 'N' 或 'n'
	if (nowCharI == 78 || nowCharI == 110) { // Nn
		return 3;
	}
	// 判斷是否為字母 'S' 或 's'
	if (nowCharI == 83 || nowCharI == 115) { // Ss
		return 4;
	}
	// 判斷是否為其他大寫字母
	if (nowCharI >= 65 && nowCharI <= 90) { // A-Z
		return 1;
	}
	// 判斷是否為其他小寫字母
	if (nowCharI >= 97 && nowCharI <= 122) { // a-z
		return 2;
	}
	// 非字母字符返回 0
	return 0;
}


/**
 * @brief 生成選擇字元
 *
 * 此函數根據輸入的字元串生成選擇的字元集合。
 * 字元串中的每個字元都會根據其模式添加相應範圍的字元到選擇集合中。
 *
 * @param selArgv 輸入的字元串。
 */
void genSelect(char* selArgv)
{
	int j, k, allSelectI = 0; // 初始化變數
	for (j = 0; j < (int)strlen(selArgv); j++) // 遍歷輸入字元串的每個字元
	{
		short nowMode = charMode((char*)selArgv[j]); // 獲取當前字元的模式
		if (nowMode == 1) { // 如果模式為1，添加大寫字母A-Z
			for (k = 65; k <= 90; k++) {
				allSelect[allSelectI++] = k;
			}
		}
		else if (nowMode == 2) { // 如果模式為2，添加小寫字母a-z
			for (k = 97; k <= 122; k++) {
				allSelect[allSelectI++] = k;
			}
		}
		else if (nowMode == 3) { // 如果模式為3，添加數字0-9
			for (k = 48; k <= 57; k++) {
				allSelect[allSelectI++] = k;
			}
		}
		else if (nowMode == 4) { // 如果模式為4，添加特殊字元
			for (k = 33; k <= 47; k++) {
				allSelect[allSelectI++] = k;
			}
			for (k = 58; k <= 64; k++) {
				allSelect[allSelectI++] = k;
			}
			for (k = 91; k <= 96; k++) {
				allSelect[allSelectI++] = k;
			}
			for (k = 123; k <= 126; k++) {
				allSelect[allSelectI++] = k;
			}
		}
	}
}


/**
 * @brief 初始化隨機種子
 *
 * 此函數使用當前時間作為隨機數種子來初始化隨機數生成器。
 *
 * @return int 返回1表示初始化成功
 */
int initRandomSeed() {
	// 使用當前時間作為種子初始化隨機數生成器
	srand((unsigned int)time(NULL));
	// 返回1表示初始化成功
	return 1;
}


/**
 * @brief 取得介於 min 和 max 之間的隨機數字。
 *
 * 這個函數會產生一個介於 min 和 max 之間的隨機數字（包括 min 和 max）。
 * 如果 min 大於 max，會自動交換這兩個值。
 *
 * @param min 最小值
 * @param max 最大值
 * @return int 介於 min 和 max 之間的隨機數字
 */
int getRandomNumberSoftware(int min, int max) {
	// 檢查 min 是否大於 max
	if (min > max)
	{
		// 如果是，交換 min 和 max 的值
		int temp = min;
		min = max;
		max = temp;
	}
	// 產生並返回介於 min 和 max 之間的隨機數字
	return (rand() % (max - min + 1)) + min;
}


/**
 * @brief 從硬體隨機數產生器取得隨機數
 *
 * 此函數會從硬體隨機數產生器中取得隨機數，並限制其範圍在給定的最小值與最大值之間。
 * 若硬體隨機數產生器不可用，則返回INT_MIN。
 *
 * @param min 最小值（包含）
 * @param max 最大值（包含）
 * @return 介於min和max之間的隨機數，若硬體隨機數產生器不可用則返回INT_MIN
 */
int getRandomNumberHardware(int min, int max) {
	unsigned int rand_num = 0;
	int range = max - min + 1;
	int hardware_rng_available = 1;

#ifdef _WIN32
	HCRYPTPROV hCryptProv;

	// 獲取加密服務提供者上下文
	if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0)) {
		// 生成隨機數
		if (!CryptGenRandom(hCryptProv, sizeof(rand_num), (BYTE*)&rand_num)) {
			hardware_rng_available = 0;
		}
		// 釋放加密服務提供者上下文
		CryptReleaseContext(hCryptProv, 0);
	}
	else {
		hardware_rng_available = 0;
	}

#else
	// 打開/dev/urandom設備檔以讀取隨機數
	int fd = open("/dev/urandom", O_RDONLY);
	if (fd != -1) {
		// 從設備檔讀取隨機數
		if (read(fd, &rand_num, sizeof(rand_num)) == -1) {
			hardware_rng_available = 0;
		}
		// 關閉設備檔
		close(fd);
	}
	else {
		hardware_rng_available = 0;
	}
#endif

	// 如果硬體隨機數不可用，則設定rand_num為INT_MIN
	if (!hardware_rng_available) {
		rand_num = INT_MIN;
	}

	// 將隨機數限制在給定範圍內
	rand_num = rand_num % range + min;
	return rand_num;
}


/**
 * @brief 檢查硬體隨機數生成器是否可用
 *
 * 這個函數調用硬體隨機數生成器，並檢查生成的隨機數是否在有效範圍內。
 * 如果硬體隨機數生成器不可用，則會顯示警告訊息並返回0；否則，返回1。
 *
 * @return int 返回1表示硬體隨機數生成器可用，返回0表示不可用。
 */
int chkRandomNumberHardware()
{
	// 呼叫硬體隨機數生成器，生成一個在0和1之間的隨機數
	int chkNum = getRandomNumberHardware(0, 1);

	// 檢查生成的隨機數是否小於等於INT_MIN，表示硬體隨機數生成器不可用
	if (chkNum <= INT_MIN)
	{
		// 輸出警告訊息，通知硬體隨機數生成器不可用
		printf("Warning: Hardware RNG not available.\n");
		// 返回0，表示硬體隨機數生成器不可用
		return 0;
	}

	// 返回1，表示硬體隨機數生成器可用
	return 1;
}


/**
 * @brief 主函式
 *
 * 這個函式是程式的入口，根據命令列參數生成指定長度的隨機密碼。
 *
 * @param argc 命令列參數的數量
 * @param argv 命令列參數的陣列
 * @return 程式的結束狀態碼
 */
int main(int argc, char* argv[])
{
	initRandomSeed();
	int i, j = 0;
	int aCount = 1;
	int aLength = 32;
	char aSelect[4] = { 'a','A','N' };
	short noArg1 = 0;
	short modeChk = 0;
	int selectCharLen = -1;
	int rCharIndex = INT_MIN;
	char rChar = 0;
	int hardware_rng = 1;
	int initializedSeed = 0;
	for (i = 1; i < argc; i++)
	{
		char* nowArgv = argv[i];
		if (i == 1) { // mode
			if (strcmp(nowArgv, "/?") == 0 || strcmp(nowArgv, "--help") == 0) {
				printf("Usage: PWGEN [MODE] [pw_length] [num_pw]\n");
				printf("         or   [pw_length] [num_pw]\n");
				printf("         or   [pw_length]\n");
				printf("MODE supported:\n");
				printf("    A: Upper case\n");
				printf("    a: Lower case\n");
				printf("    n: Number\n");
				printf("    s: Symbol\n");
				printf("   /?: display this help and exit\n");
				printf("   /V: output version information and exit\n");
				printf("Demo: PWGEN Aan 32 1\n");
				chkRandomNumberHardware();
				return 0;
			}
			if (strcmp(nowArgv, "/V") == 0 || strcmp(nowArgv, "--version") == 0)
			{
				printf("PWGEN 1.1.0\n");
				printf("Copyright (C) KagurazakaYashi\n");
				printf("License Mulan PSL v2: <http://license.coscl.org.cn/MulanPSL2>.\n");
				printf("This is free software: you are free to change and redistribute it.\n");
				printf("There is NO WARRANTY, to the extent permitted by law.\n");
				printf("Written by Kagurazaka Yashi.");
				chkRandomNumberHardware();
				return 0;
			}
			modeChk = 0;
			for (j = 0; j < (int)strlen(nowArgv); j++)
			{
				modeChk += charMode((char*)nowArgv[j]);
			}
			if (modeChk == 0) {
				noArg1 = 1;
				aLength = atoi(nowArgv);
				genSelect(aSelect);
			}
			else {
				genSelect(nowArgv);
			}
		}
		else if (i == 2) {
			if (noArg1 == 1) {
				aCount = atoi(nowArgv);
			}
			else {
				aLength = atoi(nowArgv);
			}
		}
		else if (i == 3 && noArg1 == 0) {
			aCount = atoi(nowArgv);
		}
	}
	selectCharLen = (int)strlen(allSelect);
	if (selectCharLen == 0) {
		genSelect(aSelect);
		selectCharLen = (int)strlen(allSelect);
	}
	// printf("Count=%d, Length=%d, Select=(%d)%s\n", aCount, aLength, selectCharLen, allSelect);
	for (i = 0; i < aCount; i++) {
		for (j = 0; j < aLength; j++) {
			if (selectCharLen == 0) exit(-1);
			rCharIndex = INT_MIN;
			if (hardware_rng >= 0)
			{
				rCharIndex = getRandomNumberHardware(0, selectCharLen - 1);
			}
			if (rCharIndex <= INT_MIN)
			{
				hardware_rng = 0;
				if (initializedSeed == 0)
				{
					initializedSeed = initRandomSeed();
				}
				rCharIndex = getRandomNumberSoftware(0, selectCharLen - 1);
			}
			rChar = allSelect[rCharIndex];
			printf("%c", rChar);
		}
		printf("\n");
	}
	if (hardware_rng == 0)
	{
		return 1;
	}
	return 0;
}