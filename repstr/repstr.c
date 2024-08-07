﻿/*
Copyright (c) 2021 KagurazakaYashi
repstr is licensed under Mulan PSL v2.
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
#include <ctype.h>
short ignoreCase = 0;

/**
 * @brief 忽略大小寫的子字串搜尋
 *
 * 此函式在給定的字串 haystack 中搜尋忽略大小寫的子字串 needle。
 *
 * @param haystack 原始字串，在此字串中搜尋子字串。
 * @param needle 要搜尋的子字串。
 * @return char* 返回指向 haystack 中第一次出現的子字串 needle 的指標，如果未找到則返回 NULL。
 */
char* strcasestr(const char* haystack, const char* needle) {
	// 如果 needle 是空字串，直接返回 haystack
	if (!*needle) return (char*)haystack;

	// 遍歷 haystack 字串中的每個字元
	for (; *haystack; ++haystack) {
		// 如果當前字元與 needle 的第一個字元（忽略大小寫）相等
		if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
			const char* h, * n;

			// 從當前字元開始比較 haystack 和 needle
			for (h = haystack, n = needle; *h && *n; ++h, ++n) {
				// 如果字元不相等（忽略大小寫），則跳出內部迴圈
				if (tolower((unsigned char)*h) != tolower((unsigned char)*n)) {
					break;
				}
			}

			// 如果成功比較完 needle，表示找到匹配子字串
			if (!*n) {
				return (char*)haystack;
			}
		}
	}

	// 如果未找到匹配子字串，返回 NULL
	return NULL;
}


/**
 * @brief 字串取代函式
 *
 * 這個函式用來在給定的字串中將所有出現的子字串替換為另一個字串。
 *
 * @param str 原始字串
 * @param sub 需要被替換的子字串
 * @param rep 用來替換的字串
 * @param ignoreCase 是否忽略大小寫（非零值表示忽略大小寫，零表示區分大小寫）
 * @return 成功時返回 0
 */
int strReplace(char* str, char* sub, char* rep, int ignoreCase) {
	char* nowStr = str;      // 當前處理的位置
	char* tmpStr = NULL;     // 暫時存放位置
	char* subStr = NULL;     // 子字串出現的位置
	char* dstStr = NULL;     // 目標位置
	char* src = NULL;        // 來源位置
	size_t strLen = 0, subLen = 0, repLen = 0; // 字串長度變數
	if (str != 0) strLen = strlen(str);       // 計算原始字串長度
	if (sub != 0) subLen = strlen(sub);       // 計運子元串長度
	if (rep != 0) repLen = strlen(rep);       // 計算替換字串長度
	if (!nowStr) return 0;                    // 如果原始字串為空，直接返回

	while (*nowStr != '\0') {                 // 遍歷整個字串
		tmpStr = str + strLen;                // 設定暫時位置到字串結尾
		if (sub == 0) continue;               // 如果子字串為空，繼續下一次循迴圈
		// 根據 ignoreCase 引數選擇合適的查找詢式
		if (ignoreCase) {
			subStr = strcasestr(nowStr, sub); // 忽略大小寫查找詢
		}
		else {
			subStr = strstr(nowStr, sub);     // 區分大小寫查找詢
		}
		if (subStr == NULL) {                 // 如果找不到子字串，跳出循迴圈
			break;
		}
		src = subStr + subLen;                // 設定來源位置為子字串後面
		dstStr = subStr + repLen;             // 設定目標位置為替換字串後面
		memmove(dstStr, src, tmpStr - src);   // 使用 memmove 以避免內記憶體問題
		memcpy(subStr, rep, repLen);          // 將替換字串複製到子字串位置
		strLen = strLen + repLen - subLen;    // 更新字串長度
		nowStr = subStr + repLen;             // 繼續處理替換後的位置
	}
	str[strLen] = '\0'; // 在字串最後添加新增符
	return 0;           // 返回 0 表示成功
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
 *
 * 這個函式用來處理命令列引數，並調用呼叫trReplace 函式進行字串取代。
 *
 * @param argc 命令列引數個數
 * @param argv 命令列引列數組陣列* @return 成功時返回 0
 */
int main(int argc, char* argv[])
{
	char* aStr = argv[1];
	if (argc == 1 || strcmp(aStr, "/?") == 0 || argcmp(aStr, 'H') == 1 || strcmp(aStr, "--help") == 0) {
		printf("Usage: REPSTR [MODE] [String] [Find what] [Replace with]\n");
		printf("   String: English only.\n");
		printf("MODE supported:\n");
		printf("   /c: ignore case\n");
		printf("   /?: display this help and exit\n");
		printf("   /V: output version information and exit\n");
		printf("Demo: repstr info\\txt\\readme.txt \\ /\n");
		return 0;
	}
	else if (argcmp(aStr, 'V') == 1 || strcmp(aStr, "--version") == 0)
	{
		printf("repstr 1.0.1\n");
		printf("Written by Kagurazaka Yashi. https://github.com/kagurazakayashi/NyarukoMiniTools\n");
		printf("License Mulan PSL v2: http://license.coscl.org.cn/MulanPSL2\n");
		printf("This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.\n");
		return 0;
	}
	else if (argc < 4 || aStr == NULL || argv[2] == NULL || argv[3] == NULL) {
		printf("E");
		return 1;
	}
	char* str = aStr;
	// printf("%s : %s -> %s\n", str, argv[2], argv[3]);
	strReplace(str, argv[2], argv[3], argc > 4 && strcmp(argv[4], "/c") == 0);
	printf("%s", str);
	return 0;
}
