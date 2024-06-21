#include <stdio.h>
#include <string.h>
#include <ctype.h>
short ignoreCase = 0;

/**
 * @brief 忽略大小寫的子字串搜尋函式
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

    // 遍歷 haystack 字串中的每個字符
    for (; *haystack; ++haystack) {
        // 如果當前字符與 needle 的第一個字符（忽略大小寫）相等
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char* h, * n;

            // 從當前字符開始比較 haystack 和 needle
            for (h = haystack, n = needle; *h && *n; ++h, ++n) {
                // 如果字符不相等（忽略大小寫），則跳出內部循環
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
    if (sub != 0) subLen = strlen(sub);       // 計算子字串長度
    if (rep != 0) repLen = strlen(rep);       // 計算替換字串長度
    if (!nowStr) return 0;                    // 如果原始字串為空，直接返回

    while (*nowStr != '\0') {                 // 遍歷整個字串
        tmpStr = str + strLen;                // 設定暫時位置到字串結尾
        if (sub == 0) continue;               // 如果子字串為空，繼續下一次循環
        // 根據 ignoreCase 參數選擇合適的查找函式
        if (ignoreCase) {
            subStr = strcasestr(nowStr, sub); // 忽略大小寫查找
        }
        else {
            subStr = strstr(nowStr, sub);     // 區分大小寫查找
        }
        if (subStr == NULL) {                 // 如果找不到子字串，跳出循環
            break;
        }
        src = subStr + subLen;                // 設定來源位置為子字串後面
        dstStr = subStr + repLen;             // 設定目標位置為替換字串後面
        memmove(dstStr, src, tmpStr - src);   // 使用 memmove 以避免內存重疊問題
        memcpy(subStr, rep, repLen);          // 將替換字串複製到子字串位置
        strLen = strLen + repLen - subLen;    // 更新字串長度
        nowStr = subStr + repLen;             // 繼續處理替換後的位置
    }
    str[strLen] = '\0'; // 在字串最後添加結束符
    return 0;           // 返回 0 表示成功
}

/**
 * @brief 主函式
 *
 * 這個函式用來處理命令行參數，並調用 strReplace 函式進行字串取代。
 *
 * @param argc 命令行參數個數
 * @param argv 命令行參數數組
 * @return 成功時返回 0
 */
int main(int argc, char* argv[])
{
    if (argc == 1 || strcmp(argv[1], "/?") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: repstr [MODE] [String] [Find what] [Replace with]\n");
        printf("   String: English only.\n");
        printf("MODE supported:\n");
        printf("   /c: ignore case\n");
        printf("   /?: display this help and exit\n");
        printf("   /V: output version information and exit\n");
        printf("Demo: repstr info\\txt\\readme.txt \\ /\n");
        return 0;
    }
    else if (strcmp(argv[1], "/V") == 0 || strcmp(argv[1], "--version") == 0)
    {
        printf("repstr 1.0.0\n");
        printf("Copyright (C) KagurazakaYashi\n");
        printf("License Mulan PSL v2: <http://license.coscl.org.cn/MulanPSL2>.\n");
        printf("This is free software: you are free to change and redistribute it.\n");
        printf("There is NO WARRANTY, to the extent permitted by law.\n");
        printf("Written by Kagurazaka Yashi.");
        return 0;
    }
    else if (argc < 4 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        printf("E");
        return 1;
    }
    char* str = argv[1];
    // printf("%s : %s -> %s\n", str, argv[2], argv[3]);
    strReplace(str, argv[2], argv[3], argc > 4 && strcmp(argv[4], "/c") == 0);
    printf("%s", str);
    return 0;
}
