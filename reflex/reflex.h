#ifndef REFLEX_H
#define REFLEX_H

/**
 * 處理接收到的訊號
 * @param sig 接收到的訊號程式
 */
void sigHandle(int sig);

/**
 * 休眠指定的毫秒數
 * @param dwMillisrconds 休眠時間，單位為毫秒
 */
void sleepNow(DWORD dwMillisrconds);

/**
 * 持續讀取使用者輸入的字串並顯示
 */
void ref();

/**
 * 持續顯示當前時間，每秒更新一次
 */
void ltime();

#endif // REFLEX_H
