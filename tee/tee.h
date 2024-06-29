#ifndef SLEEP_H
#define SLEEP_H

#include <stdio.h>

/**
 * @brief 將資料寫入多個文件。
 * 
 *
 * 此函式將提供的資料寫入到一組文件中。
 *
 * @param data      指向要寫入的資料的字串指標。
 * @param files     文件指標的陣列，表示要寫入的文件。
 * @param num_files 文件數量，表示文件指標陣列中的文件數量。
 */
void write_to_files(const char* data, FILE** files, int num_files);

/**
 * @brief 打開多個檔案並返回檔案指標的陣列
 *
 * 此函式接收命令列引數，從指定索引開始打開檔案並以指定模式開啟。
 *
 * @param argc 命令列引數的數量
 * @param argv 命令列引數的陣列
 * @param file_start_index 檔案名稱開始的索引
 * @param mode 開啟檔案的模式，例如 "r"、"w" 等
 * @param num_files 返回開啟的檔案數量
 * @return 返回 FILE 指標的陣列，如果失敗返回 NULL
 */
FILE** open_files(int argc, char* argv[], int file_start_index, const char* mode, int* num_files);

/**
 * @brief 關閉檔案並釋放檔案指標陣列記憶體
 *
 * 這個函式會逐一關閉傳入的檔案指標陣列中的所有檔案，然後釋放該陣列的記憶體。
 *
 * @param files 檔案指標陣列的指標
 * @param num_files 檔案指標陣列的大小（檔案數量）
 */
void close_files(FILE** files, int num_files);

#endif // SLEEP_H