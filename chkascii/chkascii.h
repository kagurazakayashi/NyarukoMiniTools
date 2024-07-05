#include <cstdio>
#ifndef CHKASCII_H
#define CHKASCII_H

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
void process_character(int c, unsigned long* yi, unsigned long* ni, unsigned long* ai, unsigned long* row, unsigned long* column, short verbose);

/**
 * @brief 檢查檔案中的ASCII字元
 *
 * 該函式會遍歷給定檔案中的所有字元，統計ASCII字元和非ASCII字元的數量，並列印結果。
 *
 * @param fp 檔案指標，指向要檢查的檔案
 * @param verbose 是否輸出詳細資訊，如果為1則輸出詳細資訊
 * @param fn 檔名，用於列印輸出時顯示
 */
void check_ascii(FILE* fp, short verbose, const char* fn);

/**
 * @brief 檢查輸入的文字並統計ASCII字元和非ASCII字元的數量
 *
 * @param verbose 控制是否顯示詳細資訊，1表示顯示，0表示不顯示
 * @return 返回處理的字元總數
 */
int check_ascii_input(short verbose);

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

/**
 * @brief 由使用者主動輸入內容
 *
 * 該函式持續呼叫 check_ascii_input 函式，直到使用者輸入空值時退出迴圈。
 *
 * @param verbose 冗長模式標誌，如果為非零值，則函式會輸出更多的除錯資訊。
 */
void need_input(short verbose);

#endif // CHKASCII_H
