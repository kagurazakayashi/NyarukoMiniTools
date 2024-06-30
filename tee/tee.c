/*
Copyright (c) 2024 KagurazakaYashi
tee-like is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 將資料寫入多個文件
 *
 * 此函式將提供的資料寫入到一組文件中。
 *
 * @param data      指向要寫入的資料的字串指標。
 * @param files     文件指標的陣列，表示要寫入的文件。
 * @param num_files 文件數量，表示文件指標陣列中的文件數量。
 */
void write_to_files(const char* data, FILE** files, int num_files) {
    // 迴圈遍歷所有文件
    for (int i = 0; i < num_files; i++) {
        // 將資料寫入當前文件
        fprintf(files[i], "%s", data);
    }
}


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
FILE** open_files(int argc, char* argv[], int file_start_index, const char* mode, int* num_files) {
    // 計算需要打開的檔案數量
    *num_files = argc - file_start_index;
    // 為 FILE 指標陣列分配記憶體
    FILE** files = malloc(*num_files * sizeof(FILE*));
    if (files == NULL) {
        // 如果分配失敗，打印錯誤信息並返回 NULL
        perror("malloc");
        return NULL;
    }

    // 迴圈打開每個檔案
    for (int i = file_start_index; i < argc; i++) {
        // 打開檔案，並將指標存入 files 陣列
        if (fopen_s(&files[i - file_start_index], argv[i], mode) != 0) {
            // 如果打開失敗，打印錯誤信息
            perror(argv[i]);
            // 關閉已成功打開的所有檔案
            for (int j = 0; j < i - file_start_index; j++) {
                if (files[j] != 0)
                {
                    fclose(files[j]);
                }
            }
            // 釋放已分配的記憶體
            free(files);
            return NULL;
        }
    }
    // 返回 FILE 指標的陣列
    return files;
}


/**
 * @brief 關閉檔案並釋放檔案指標陣列記憶體
 * 
 * 這個函式會逐一關閉傳入的檔案指標陣列中的所有檔案，然後釋放該陣列的記憶體。
 *
 * @param files 檔案指標陣列的指標
 * @param num_files 檔案指標陣列的大小（檔案數量）
 */
void close_files(FILE** files, int num_files) {
    // 逐一關閉所有檔案
    for (int i = 0; i < num_files; i++) {
        fclose(files[i]);
    }
    // 釋放檔案指標陣列的記憶體
    free(files);
}


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
int argcmp(const char *str, char k) {
    // 迴圈遍歷字串中的每個字元
    while (*str) {
        // 取得字串中當前字元的下一個字元
        char c = *(str + 1);
        // 如果字元是小寫字母，將其轉換為大寫
        if (c >= 'a' && c <= 'z') {
            c -= ('a' - 'A');
        }
        // 檢查當前字元是否為 '/' 或 '-' ，且下一個字元是否等於 k
        if ((*str == '/' || *str == '-') && c == k) {
            return 1; // 如果匹配，返回1
        }
        // 移動到字串中的下一個字元
        str++;
    }
    // 如果未找到匹配的字元，返回0
    return 0;
}


/**
 * @brief 主函式
 *
 * 這個函式處理命令列引數，並將標準輸入的資料寫入到指定的檔案中。
 *
 * @param argc 命令列引數的數量
 * @param argv 命令列引數的陣列
 * @return 程式的結束狀態碼
 */
int main(int argc, char* argv[]) {
    int append_mode = 0;
    int file_start_index = 1;
    char* aStr = argv[1];
    if (argc < 2 || strcmp(aStr, "/?") == 0 || argcmp(aStr, 'H') == 1 || strcmp(aStr, "--help") == 0)
    {
        printf("Usage: TEE [MODE] [logfile]\n");
        printf("MODE supported:\n");
        printf("    /A  append to the given FILEs, do not overwrite.\n");
        printf("    /?  display this help and exit.\n");
        printf("    /V  output version information and exit.\n");
        return 0;
    }
    else if (argcmp(aStr, 'V') == 1 || strcmp(aStr, "--version") == 0)
    {
        printf("tee-like 1.0.0\n");
        printf("Written by Kagurazaka Yashi. https://github.com/kagurazakayashi/NyarukoMiniTools\n");
        printf("License Mulan PSL v2: http://license.coscl.org.cn/MulanPSL2\n");
        printf("This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.\n");
        return 0;
    }

    if (argcmp(aStr, 'A') == 1 || strcmp(aStr, "--add") == 0) {
        append_mode = 1;
        file_start_index = 2;
    }

    if (argc < file_start_index + 1) {
        fprintf(stderr, "Usage: %s [/a] [logfile]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_files;
    const char* mode = append_mode ? "a" : "w";
    FILE** files = open_files(argc, argv, file_start_index, mode, &num_files);
    if (files == NULL) {
        return EXIT_FAILURE;
    }

    char buffer[BUFSIZ];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        fputs(buffer, stdout);
        write_to_files(buffer, files, num_files);
    }

    close_files(files, num_files);
    return EXIT_SUCCESS;
}
