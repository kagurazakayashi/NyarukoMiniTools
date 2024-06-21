#ifndef REPSTR_H
#define REPSTR_H

/**
 * @brief 忽略大小寫的子字串搜尋函式
 *
 * 此函式在給定的字串 haystack 中搜尋忽略大小寫的子字串 needle。
 *
 * @param haystack 原始字串，在此字串中搜尋子字串。
 * @param needle 要搜尋的子字串。
 * @return char* 返回指向 haystack 中第一次出現的子字串 needle 的指標，如果未找到則返回 NULL。
 */
char* strcasestr(const char* haystack, const char* needle);

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
int strReplace(char* str, char* sub, char* rep, int ignoreCase);

#endif // REPSTR_H
