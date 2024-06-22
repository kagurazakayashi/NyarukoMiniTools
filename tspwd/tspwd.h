#ifndef TSPWD_H
#define TSPWD_H

#include <time.h>

/**
 * @brief 計算輸入字串的 SHA-256 哈希值並以十六進位字串格式輸出
 *
 * @param string        輸入的字串
 * @param outputBuffer  儲存輸出哈希值的緩衝區，必須有 65 字元長度
 *
 * 使用 OpenSSL 函式庫進行 SHA-256 哈希運算。包含初始化、更新和最終運算步驟。
 * 若任何步驟失敗，會呼叫 handleErrors() 進行錯誤處理。
 * 最終的哈希值會轉換為十六進位字串格式並存入 outputBuffer。
 */
void sha256(char* string, char outputBuffer[65]);

/**
 * @brief 產生哈希值
 * @param format 指定時間格式的字串
 * @param salt 使用的鹽值
 * @param hashOutput 用來儲存生成的SHA-256哈希值
 */
void generate_hash(const char* format, const char* salt, char* hashOutput);

#endif // TSPWD_H
