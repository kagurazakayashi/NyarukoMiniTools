/*
Copyright (c) 2024 KagurazakaYashi
tspwd is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
		 http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
*/
#include <stdio.h> // 匯入標準輸入輸出頭檔案
#include <stdlib.h> // 匯入標準函式庫頭檔案，提供記憶體分配、程式控制等功能
#include <time.h> // 匯入時間處理相關函式頭檔案
#include <string.h> // 匯入字串處理函式庫頭檔案
#include "sha256.h" // 匯入 SHA-256 演演法實作頭檔案

/**
 * @brief 處理錯誤函式
 *
 * 此函式用於處理錯誤，將錯誤訊息輸出到標準錯誤並立即終止程式運執。
 */
void handleErrors(void) {
	fprintf(stderr, "ERROR.\n"); // 將錯誤訊息輸出到標準錯誤
	abort(); // 立即終止程式運執
}

/**
 * @brief 產生哈雜湊
 * @param format 指定時間格式的字串
 * @param salt 使用的鹽值
 * @param hashOutput 用來儲存生成的SHA-256哈雜湊
 */
void generate_hash(const char* format, const char* salt, char* hashOutput) {
	time_t now;
	struct tm now_tm;
	char timeString[20];
	char finalString[255];

	time(&now);
#ifdef _WIN32
	localtime_s(&now_tm, &now);
#else
	localtime_r(&now, &now_tm);
#endif

	strftime(timeString, sizeof(timeString), format, &now_tm);
	snprintf(finalString, sizeof(finalString), "%s%s", timeString, salt);

	sha256(finalString, hashOutput);
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
 * @brief 主函式
 * @param argc 命令列引數數量
 * @param argv 命令列引數陣列
 * @return 程式執行結果
 */
int main(int argc, char* argv[]) {
	char hashOutput[65];
	const char* salt = "";
	const char* format = "%Y%m%d%H%M%S";
	char* aStr = argv[1];
	if (argc >= 2) {
		if (strcmp(aStr, "/?") == 0 || argcmp(aStr, 'H') == 1 || strcmp(aStr, "--help") == 0) {
			printf("Get the SHA256 of the current timestamp.\n");
			printf("Usage: TSPWD [timestamp precision] [salt]\n");
			printf("    timestamp precision: `y` / `m` / `d` / `h` / `m` / `s`(default)\n");
			printf("    salt: any string of English letters/numbers, less than 200 characters (default is empty).\n");
			printf("    /?  display this help and exit\n");
			printf("    /V  output version information and exit\n");
			return 0;
		}
		else if (argcmp(aStr, 'V') == 1 || strcmp(aStr, "--version") == 0) {
			printf("TSPwd 1.0.1\n");
			printf("Written by Kagurazaka Yashi. https://github.com/kagurazakayashi/NyarukoMiniTools\n");
			printf("License Mulan PSL v2: http://license.coscl.org.cn/MulanPSL2\n");
			printf("This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.\n");
			return 0;
		}

		// 根據命令列引數調整時間格式字串
		if (argcmp(aStr, 'y') == 1) {
			format = "%Y"; // 只顯示年份
		}
		else if (strcmp(aStr, "m") == 0) {
			format = "%Y%m"; // 顯示年份和月份
		}
		else if (argcmp(aStr, 'D') == 1) {
			format = "%Y%m%d"; // 顯示年月日
		}
		else if (argcmp(aStr, 'H') == 1) {
			format = "%Y%m%d%H"; // 顯示到小時
		}
		else if (strcmp(aStr, "M") == 0) {
			format = "%Y%m%d%H%M"; // 顯示到分鐘
		}
	}

	if (argc >= 3) {
		salt = argv[2]; // 如果提供了第三個引數，則將其設定為鹽值
	}

	generate_hash(format, salt, hashOutput);
	printf("%s", hashOutput); // 輸出密後的哈雜湊

	return 0;
}
