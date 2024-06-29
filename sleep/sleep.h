#ifndef SLEEP_H
#define SLEEP_H

/**
 * @brief 使當前執行緒進入休眠狀態。
 *
 * 這個函式會讓當前執行緒暫停執行指定的時間。
 *
 * @param sec 要休眠的秒數。
 * @param display 是否顯示倒數計時。
 */
void sleepNow(int sec, int display);

/**
 * @brief 將時間字串轉換為毫秒數
 *
 * 此函式接收一個表示數字的字串和一個表示時間單位的字符，並將其轉換為毫秒數。
 *
 * @param num 表示數字的字串
 * @param unit 表示時間單位的字符，可以是 'm' (分鐘), 'h' (小時), 或 'd' (天)
 * @return double 對應的毫秒數
 */
double timeStrToSec(char* num, char unit);

#endif // SLEEP_H
