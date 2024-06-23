#ifndef SCRCTL_H
#define SCRCTL_H

/**
 * @brief 監控電源模式
 *
 * 此函式用於調整顯示器的電源模式。
 *
 * @param powerMode 指定電源模式。可以為下列值之一：
 * -1：顯示器關閉
 * 1：顯示器進入省電模式
 * 2：顯示器恢復全功率模式
 */
void monitorPower(int powerMode);

#endif // SCRCTL_H
