/*
 Copyright (c) 2020 KagurazakaYashi
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

 /* 相容性處理：在非 MSVC 編譯器上，將 sscanf_s 對映為 sscanf */
#ifndef _MSC_VER
#define sscanf_s sscanf
#endif

/* ---------- 工具函式宣告 ---------- */
static void print_usage(const char* prog);

/**
 * @brief 解析形如 "A,B" 的座標字串（解析座標）
 * @param s 輸入字串，如 "370,125"
 * @param x 輸出 X
 * @param y 輸出 Y
 * @return 成功返回 0，失敗返回 -1
 */
static int parse_coord(const char* s, int* x, int* y) {
    // 嘗試解析 "整數,整數" 格式
    if (sscanf_s(s, "%d,%d", x, y) == 2) {
        return 0;
    }
    return -1;
}

/**
 * @brief 解析形如 "WxH" 的尺寸字串（解析寬高）
 * @param s 輸入字串，如 "36x18" 或 "36X18"
 * @param w 輸出 寬
 * @param h 輸出 高
 * @return 成功返回 0，失敗返回 -1
 */
static int parse_size(const char* s, int* w, int* h) {
    // 先嚐試小寫 x，再嘗試大寫 X
    if (sscanf_s(s, "%dx%d", w, h) == 2) {
        return 0;
    }
    if (sscanf_s(s, "%dX%d", w, h) == 2) {
        return 0;
    }
    return -1;
}

/* ---------- 模式 1：左上角 + 右下角 ---------- */
/**
 * @brief 模式一：輸入左上角(x1,y1)與右下角(x2,y2)，計算寬高與畫素並按固定格式輸出。
 *        （支援形如 "./rect_pixels 370,125 406,143"）
 *
 * @param arg_tl 字串引數1：左上角座標，如 "370,125"
 * @param arg_br 字串引數2：右下角座標，如 "406,143"
 * @return 成功返回 0；解析失敗或資料非法返回非 0
 */
static int mode_two_points(const char* arg_tl, const char* arg_br) {
    int x1, y1, x2, y2;

    // 解析兩個座標引數
    if (parse_coord(arg_tl, &x1, &y1) != 0) {
        fprintf(stderr, "[X] Invalid coordinate for top-left: '%s'\n", arg_tl);
        return 1;
    }
    if (parse_coord(arg_br, &x2, &y2) != 0) {
        fprintf(stderr, "[X] Invalid coordinate for bottom-right: '%s'\n", arg_br);
        return 1;
    }

    // 計算寬高（取絕對值以容錯），畫素為寬*高
    int width = abs(x2 - x1);
    int height = abs(y2 - y1);
    int pixels = width * height;

    // 列印結果，格式符合你的要求
    printf("%d , %d  ~  %d , %d\n", x1, y1, x2, y2);
    printf("%d x %d = %d\n", width, height, pixels);

    return 0;
}

/* ---------- 模式 2：左上角 + 寬高 ---------- */
/**
 * @brief 模式二：輸入左上角(x1,y1)與尺寸(width x height)，計算右下角並輸出寬高與畫素。
 *        （支援形如 "./rect_pixels 370,125 36x18"）
 *
 * @param arg_tl 字串引數1：左上角座標，如 "370,125"
 * @param arg_wh 字串引數2：尺寸，如 "36x18" 或 "36X18"
 * @return 成功返回 0；解析失敗或資料非法返回非 0
 */
static int mode_tl_plus_size(const char* arg_tl, const char* arg_wh) {
    int x1, y1, width, height;

    // 解析左上角座標
    if (parse_coord(arg_tl, &x1, &y1) != 0) {
        fprintf(stderr, "[X] Invalid coordinate for top-left: '%s'\n", arg_tl);
        return 1;
    }

    // 解析寬高（支援 x 或 X）
    if (parse_size(arg_wh, &width, &height) != 0) {
        fprintf(stderr, "[X] Invalid size: '%s' (expected WxH)\n", arg_wh);
        return 1;
    }

    // 寬高必須為正數
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "[X] Width and height must be positive: got %d x %d\n", width, height);
        return 1;
    }

    // 計算右下角座標
    int x2 = x1 + width;
    int y2 = y1 + height;

    // 畫素為寬*高
    int pixels = width * height;

    // 列印結果，格式符合你的要求
    printf("%d , %d  ~  %d , %d\n", x1, y1, x2, y2);
    printf("%d x %d = %d\n", width, height, pixels);

    return 0;
}

/* ---------- 用法提示 ---------- */
/**
 * @brief 列印用法（只在錯誤時顯示，英文提示）
 */
static void print_usage(const char* prog) {
    printf("Usage:\n");
    printf("  Mode 1 (two points): %s x1,y1 x2,y2\n", prog);
    printf("  Mode 2 (size):       %s x1,y1 WIDTHxHEIGHT\n", prog);
    printf("  %s /?  display this help and exit\n", prog);
    printf("  %s /V  output version information and exit\n", prog);
    printf("Examples:\n");
    printf("  %s 370,125 406,143\n", prog);
    printf("  %s 370,125 36x18\n", prog);
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

/* ---------- 程式入口 ---------- */
int main(int argc, char* argv[]) {
    char* aStr = argv[1];
    if (strcmp(aStr, "/?") == 0 || argcmp(aStr, 'H') == 1 || strcmp(aStr, "--help") == 0)
    {
        print_usage(argv[0]);
        return 0;
    }
    if (argcmp(aStr, 'V') == 1 || strcmp(aStr, "--version") == 0)
    {
        printf("rectcalc 1.0.0\n");
        printf("Written by Kagurazaka Yashi. https://github.com/kagurazakayashi/NyarukoMiniTools\n");
        printf("License Mulan PSL v2: http://license.coscl.org.cn/MulanPSL2\n");
        printf("This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.\n");
        return 0;
    }

    // 需要兩個引數（不包含程式名），否則列印用法
    if (argc != 3) {
        print_usage(argv[0]);
        return 1;
    }

    const char* arg1 = argv[1];
    const char* arg2 = argv[2];

    // 根據第二個引數判斷模式
    if (strchr(arg2, ',')) {
        // 模式 1 —— 兩點座標
        return mode_two_points(arg1, arg2);
    }
    else if (strchr(arg2, 'x') || strchr(arg2, 'X')) {
        // 模式 2 —— 左上角 + 尺寸
        return mode_tl_plus_size(arg1, arg2);
    }
    else {
        // 無法判斷，提示用法
        fprintf(stderr, "Unrecognized second argument: '%s'\n", arg2);
        print_usage(argv[0]);
        return 1;
    }
}
