#ifndef DATETIME_H
#define DATETIME_H

/**
 * @brief 將字元陣列中的元素依序往前移動，並在最後一個位置放入新的字元。
 *
 * @param cArr 指向字元陣列的指標
 * @param cLen 字元陣列的長度
 * @param cNew 要插入到陣列最後一個位置的新字元
 */
void shiftAndInsertChar(char* cArr, int cLen, char cNew);

/**
 * @brief 替換字串中特定字元組合為數字，根據指定的長度格式化數字。
 *
 * @param aStr 原始字串指標
 * @param aLen 原始字串長度
 * @param num 要替換的數字
 * @param rchar 要被替換的字元組合
 * @param cSize 要被替換字元組合的長度
 */
void replaceSubstringWithNumber(char* aStr, int aLen, int num, char* rchar, int cSize);

/**
 * @brief 根據提供的時間結構(struct tm)替換字串中的時間標記（如 "yyyy", "MM" 等）。
 *
 * @param aStr 指向要進行替換操作的字串。
 * @param time 包含日期和時間信息的時間結構。
 */
void replaceTimeMarkers(char* aStr, struct tm time);

#endif // DATETIME_H