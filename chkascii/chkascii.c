/*
Copyright (c) 2022 KagurazakaYashi
datetime is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
		 http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 8000  // 定義緩衝區大小（使用者輸入模式的最大字元數量）

/**
 * @brief 處理字元並更新相關計數和位置。
 *
 * @param c 輸入的字元。
 * @param yi 用於統計合法字元的計數器指標。
 * @param ni 用於統計非法字元的計數器指標。
 * @param ai 用於統計字元總數的計數器指標。
 * @param row 當前行位置指標。
 * @param column 當前列位置指標。
 * @param verbose 是否輸出詳細資訊，1 為輸出，0 為不輸出。
 */
void process_character(int c, unsigned long* yi, unsigned long* ni, unsigned long* ai, unsigned long* row, unsigned long* column, short verbose) {
    (*column)++;  // 增加當前列位置

    if (c == '\n') {  // 如果字元是換行符
        *column = 1;  // 重置列位置為 1
        (*row)++;     // 增加行計數
    }

    if (c > 177 || c < 0) {  // 判斷字元是否為非法字元（超出合法範圍）
        (*ni)++;  // 增加非法字元計數
		printf("[!] Position %lu, Row %lu, Column %lu, Code %i, Char %c\n", *ai, *row, *column, c, (unsigned char)c);
    }
    else {  // 字元為合法字元
        if (verbose == 1) {  // 如果需要輸出詳細資訊
			printf("[-] Position %lu, Row %lu, Column %lu, Code %i, Char %c\n", *ai, *row, *column, c, (unsigned char)c);
        }
        (*yi)++;  // 增加合法字元計數
    }

    (*ai)++;  // 增加總字元數計數
}


/**
 * @brief 檢查檔案中的ASCII字元
 *
 * 該函式會遍歷給定檔案中的所有字元，統計ASCII字元和非ASCII字元的數量，並列印結果。
 *
 * @param fp 檔案指標，指向要檢查的檔案
 * @param verbose 是否輸出詳細資訊，如果為1則輸出詳細資訊
 * @param fn 檔名，用於列印輸出時顯示
 */
void check_ascii(FILE* fp, short verbose, const char* fn) {
    int c; // 用於儲存從檔案中讀取的字元
    unsigned long yi = 0, ni = 0, ai = 0, row = 1, column = 1; // 初始化計數器

    // 逐字元讀取檔案內容，直到檔案結束
    while ((c = fgetc(fp)) != EOF) {
        // 處理當前字元，更新統計計數器和行列資訊
        process_character(c, &yi, &ni, &ai, &row, &column, verbose);
    }

    // 列印統計結果
    printf("\n   Ascii = %lu\nNo Ascii = %lu\n     All = %lu\n    File = %s\n", yi, ni, ai, fn);
}


/**
 * @brief 檢查輸入的文字並統計ASCII字元和非ASCII字元的數量
 *
 * @param verbose 控制是否顯示詳細資訊，1表示顯示，0表示不顯示
 * @return 返回處理的字元總數
 */
int check_ascii_input(short verbose) {
    int c;  // 當前處理的字元
    unsigned long yi = 0, ni = 0, ai = 0, row = 1, column = 1;  // 統計變數，分別表示ASCII字元數、非ASCII字元數和總字元數
    char input[BUFFER_SIZE];  // 儲存輸入的文字
    const char* ptr = input;  // 指向輸入文字的指標

    // 提示使用者輸入文字
    printf("Please input text:\n");
    fgets(input, sizeof(input), stdin);  // 獲取使用者輸入的文字
    size_t len = strlen(input);  // 獲取輸入文字的長度
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';  // 去除輸入文字末尾的換行符
    }

    // 遍歷輸入文字中的每一個字元
    while ((c = *ptr++) != '\0') {
        // 處理字元並更新統計資料
        process_character(c, &yi, &ni, &ai, &row, &column, verbose);
    }

    // 輸出統計結果
    printf("\n   Ascii = %lu\nNo Ascii = %lu\n     All = %lu\n", yi, ni, ai);
    return ai;  // 返回處理的字元總數
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
 * @brief 由使用者主動輸入內容
 *
 * 該函式持續呼叫 check_ascii_input 函式，直到使用者輸入空值時退出迴圈。
 *
 * @param verbose 冗長模式標誌，如果為非零值，則函式會輸出更多的除錯資訊。
 */
void need_input(short verbose)
{
    // 無限迴圈，直到使用者輸入空值
    while (1)
    {
        // 呼叫 check_ascii_input 函式，如果返回 0，則退出迴圈
        if (check_ascii_input(verbose) == 0)
        {
            break;
        }
    }
}


/**
 * @brief 主函式
 *
 * @param argc 命令列引數的數量
 * @param argv 命令列引數的值
 * @return int 返回0表示成功
 */
int main(int argc, char* argv[])
{
	int i = 1;
	short verbose = 0;
	char* fn = NULL;

	if (argc < 2)
	{
		need_input(0);
		return 0;
	}
	char* aStr = argv[1];
	if (strcmp(aStr, "/?") == 0 || argcmp(aStr, 'H') == 1 || strcmp(aStr, "--help") == 0)
	{
		printf("Usage: CHKASCII [MODE] [filepath]\nMODE:\n");
		printf("    /D  display process (slow)\n");
		printf("    /?  display this help and exit.\n");
		printf("    /V  output version information and exit.\n");
		return 0;
	}
	else if (argcmp(aStr, 'V') == 1 || strcmp(aStr, "--version") == 0)
	{
		printf("chkASCII 1.0.0\n");
		printf("Written by Kagurazaka Yashi. https://github.com/kagurazakayashi/NyarukoMiniTools\n");
		printf("License Mulan PSL v2: http://license.coscl.org.cn/MulanPSL2\n");
		printf("This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.\n");
		return 0;
	}
	else if (argcmp(aStr, 'P') == 1 || strcmp(aStr, "--process") == 0)
	{
		verbose++;
		i++;
	}
	for (; i < argc; i++)
	{
		fn = argv[i];
	}
	if (fn == NULL)
	{
		need_input(verbose);
		return 0;
	}
	FILE* fp = fopen(fn, "r");
	if (fp == NULL) {
		printf("ERROR: %s\n", fn);
		exit(1);
	}
	else {
		check_ascii(fp, verbose, fn);
		fclose(fp);
	}

	return 0;
}
