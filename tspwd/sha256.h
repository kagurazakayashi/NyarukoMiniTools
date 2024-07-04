#ifndef SHA256_H
#define SHA256_H

/**
 * @brief 計算輸入字元的SHA-256雜湊值並轉換為十六進位字串
 *
 * @param string 輸入字元
 * @param outputBuffer 輸出十六進位字串，必須至少有65個位元組
 */
void sha256(char string, char outputBuffer[]);

#endif // SHA256_H