#ifndef TSPWD_H
#define TSPWD_H

/**
 * @brief 產生雜湊值
 * @param format 指定時間格式的字串
 * @param salt 使用的鹽值
 * @param hashOutput 用來儲存生成的SHA-256雜湊值
 */
void generate_hash(const char* format, const char* salt, char* hashOutput);

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
int argcmp(const char* str, char k);

#endif // TSPWD_H
