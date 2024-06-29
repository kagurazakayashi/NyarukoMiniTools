#ifndef PWGEN_H
#define PWGEN_H

/**
 * @brief 判斷字元模式
 *
 * 這個函式根據傳入的字元返回一個短整數值，
 * 以表示該字元的模式。具體的模式如下：
 *  - 大寫字母 'N' 或小寫字母 'n' 返回 3
 *  - 大寫字母 'S' 或小寫字母 's' 返回 4
 *  - 其他大寫字母返回 1
 *  - 其他小寫字母返回 2
 *  - 非字母字符返回 0
 *
 * @param nowChar 指向要判斷的字元的指標
 * @return short 表示字元模式的數值
 */
short charMode(char* nowChar);

/**
 * @brief 生成選擇字元
 *
 * 此函式根據輸入的字元串生成選擇的字元集合。
 * 字元串中的每個字元都會根據其模式添加相應範圍的字元到選擇集合中。
 *
 * @param selArgv 輸入的字元串。
 */
void genSelect(char* selArgv);

/**
 * @brief 初始化隨機種子
 *
 * 此函式使用當前時間作為隨機數種子來初始化隨機數生成器。
 *
 * @return int 返回1表示初始化成功
 */
int initRandomSeed();

/**
 * @brief 取得介於 min 和 max 之間的隨機數字。
 *
 * 這個函式會產生一個介於 min 和 max 之間的隨機數字（包括 min 和 max）。
 * 如果 min 大於 max，會自動交換這兩個值。
 *
 * @param min 最小值
 * @param max 最大值
 * @return int 介於 min 和 max 之間的隨機數字
 */
int getRandomNumberSoftware(int min, int max);

/**
 * @brief 從硬體隨機數產生器取得隨機數
 *
 * 此函式會從硬體隨機數產生器中取得隨機數，並限制其範圍在給定的最小值與最大值之間。
 * 若硬體隨機數產生器不可用，則返回INT_MIN。
 *
 * @param min 最小值（包含）
 * @param max 最大值（包含）
 * @return 介於min和max之間的隨機數，若硬體隨機數產生器不可用則返回INT_MIN
 */
int getRandomNumberHardware(int min, int max);

/**
 * @brief 檢查硬體隨機數生成器是否可用
 *
 * 這個函式調用硬體隨機數生成器，並檢查生成的隨機數是否在有效範圍內。
 * 如果硬體隨機數生成器不可用，則會顯示警告訊息並返回0；否則，返回1。
 *
 * @return int 返回1表示硬體隨機數生成器可用，返回0表示不可用。
 */
int chkRandomNumberHardware();

#endif // PWGEN_H
