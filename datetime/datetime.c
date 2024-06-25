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
#include <time.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief 將字元陣列中的元素依序往前移動，並在最後一個位置放入新的字元。
 *
 * @param cArr 指向字元陣列的指標
 * @param cLen 字元陣列的長度
 * @param cNew 要插入到陣列最後一個位置的新字元
 */
void shiftAndInsertChar(char* cArr, int cLen, char cNew)
{
    // 遍歷字元陣列的每一個元素
    for (int i = 0; i < cLen; i++)
    {
        // 如果是最後一個元素
        if (i + 1 >= cLen)
        {
            // 將新的字元插入到最後一個位置
            cArr[i] = cNew;
            return;
        }
        // 將當前位置的字元替換成下一個位置的字元
        cArr[i] = cArr[i + 1];
    }
}


/**
 * @brief 替換字串中特定字元組合為數字，根據指定的長度格式化數字。
 *
 * @param aStr 原始字串指標
 * @param aLen 原始字串長度
 * @param num 要替換的數字
 * @param rchar 要被替換的字元組合
 * @param cSize 要被替換字元組合的長度
 */
void replaceSubstringWithNumber(char* aStr, int aLen, int num, char* rchar, int cSize)
{
    int i, j;
    int cSize0 = cSize - 1;
    char str[5] = { '\0' }; // 暫存字串，用於比較
    for (i = 0; i < aLen; i++)
    {
        char nc = aStr[i];
        shiftAndInsertChar(str, cSize, nc); // 移動字元進入暫存字串
        if (strcmp(str, rchar) == 0) { // 比較暫存字串與目標字元組合
            char dateChar[5] = { '\0' }; // 用於存儲格式化數字的字串
            if (cSize == 2) {
                sprintf_s(dateChar, 3, "%02d", num); // 格式化兩位數字
            }
            else if (cSize == 4) {
                sprintf_s(dateChar, 5, "%04d", num); // 格式化四位數字
            }
            for (j = 0; j < cSize; j++)
            {
                aStr[i - (cSize0 - j)] = dateChar[j]; // 將格式化數字插入原字串中
            }
            i += cSize0; // 跳過已替換的字元
        }
    }
}


/**
 * @brief 根據提供的時間結構(struct tm)替換字串中的時間標記（如 "yyyy", "MM" 等）。
 *
 * @param aStr 指向要進行替換操作的字串。
 * @param time 包含日期和時間信息的時間結構。
 */
void replaceTimeMarkers(char* aStr, struct tm time) {
    // 取得字串長度
    int aLen = (int)strlen(aStr);

    // 取得完整年份（如 2023）
    int nv = time.tm_year + 1900;
    // 將 "yyyy" 替換為完整年份
    replaceSubstringWithNumber(aStr, aLen, nv, "yyyy", 4);

    // 取得年份的後兩位（如 23）
    nv = (nv / 10 % 10) * 10 + (nv / 1 % 10);
    // 將 "yy" 替換為年份的後兩位
    replaceSubstringWithNumber(aStr, aLen, nv, "yy", 2);

    // 取得月份（範圍 1-12）
    nv = time.tm_mon + 1;
    // 將 "MM" 替換為月份
    replaceSubstringWithNumber(aStr, aLen, nv, "MM", 2);

    // 將 "dd" 替換為日期
    replaceSubstringWithNumber(aStr, aLen, time.tm_mday, "dd", 2);

    // 取得小時（24小時制）
    nv = time.tm_hour;
    // 將 "HH" 替換為小時（24小時制）
    replaceSubstringWithNumber(aStr, aLen, nv, "HH", 2);

    // 若小時超過 12 小時，轉為 12 小時制
    if (nv > 12) nv -= 12;
    // 將 "hh" 替換為小時（12小時制）
    replaceSubstringWithNumber(aStr, aLen, nv, "hh", 2);

    // 將 "mm" 替換為分鐘
    replaceSubstringWithNumber(aStr, aLen, time.tm_min, "mm", 2);

    // 將 "ss" 替換為秒數
    replaceSubstringWithNumber(aStr, aLen, time.tm_sec, "ss", 2);
}


/**
 * @brief 主函數
 *
 * @param argc 命令行參數的數量
 * @param argv 命令行參數的值
 * @return int 返回0表示成功
 */
int main(int argc, char* argv[])
{
    time_t now;
    time(&now);
    struct tm time;
    localtime_s(&time, &now);
    if (argc < 2)
    {
        printf("%4d-%02d-%02d %02d:%02d:%02d", 1900 + time.tm_year, 1 + time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
        return 0;
    }
    if (argc == 3 && strcmp(argv[2], "gmt") == 0) {
        gmtime_s(&time, &now);
    }
    char* aStr = argv[1];
    if (strcmp(aStr, "/?") == 0 || strcmp(aStr, "--help") == 0)
    {
        printf("Usage: datetime MODE [TIME]\n");
        printf("MODE:\n");
        printf("    Support: yyyy,yy,MM,dd,HH,hh,mm,ss\n");
        printf("  Unsupport: y,M,d,H,h,m,s,ddd,dddd,MMM,MMMM,gg ...\n");
        printf("TIME:\n  \"gmt\" or \"local\" (the default)\n");
        printf("DEMO:\n  datetime \"yyyy-MM-dd HH:mm:ss\"\n        ->  2021-02-01 20:21:11\n");
        printf("\ndatetime /?  display this help and exit");
        printf("\ndatetime /V  output version information and exit");
        return 0;
    }
    if (strcmp(aStr, "/V") == 0 || strcmp(aStr, "--version") == 0)
    {
        printf("datetime 1.1.0\n");
        printf("Copyright (C) KagurazakaYashi\n");
        printf("License Mulan PSL v2: <http://license.coscl.org.cn/MulanPSL2>.\n");
        printf("This is free software: you are free to change and redistribute it.\n");
        printf("There is NO WARRANTY, to the extent permitted by law.\n");
        printf("Written by Kagurazaka Yashi.");
        return 0;
    }
    replaceTimeMarkers(aStr, time);
    printf("%s", aStr);
    return 0;
}
