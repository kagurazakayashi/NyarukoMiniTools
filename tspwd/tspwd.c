/*
Copyright (c) 2024 KagurazakaYashi
tspwd is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
		 http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
*/
#include <stdio.h> // 匯入標準輸入輸出頭檔案
#include <stdlib.h> // 匯入標準函式庫頭檔案，提供記憶體分配、程式控制等功能
#include <time.h> // 匯入時間處理相關函式頭檔案
#include <string.h> // 匯入字串處理函式庫頭檔案
/*
請根據 OpenSSL 具體安裝位置修改專案屬性，然後重新載入解決方案：
- 配置屬性 -> VC++ 目錄 -> 包含目錄
  - 例如 "C:\Program Files\OpenSSL-Win64\include";$(VC_IncludePath);$(WindowsSDK_IncludePath);
- 配置屬性 -> VC++ 目錄 -> 庫目錄
  - 例如 "C:\Program Files\OpenSSL-Win64\lib";$(VC_LibraryPath_x64);$(WindowsSDK_LibraryPath_x64);$(NETFXKitsDir)Lib\um\x64
- 配置屬性 -> C/C++ -> 常規 -> 附加包含目錄
  - 例如 "C:\Program Files\OpenSSL-Win64\include\openssl";"C:\Program Files\OpenSSL-Win64\include";%(AdditionalIncludeDirectories)
- 配置屬性 -> 連結器 -> 常規 -> 附加庫目錄
  - 例如 "C:\Program Files\OpenSSL-Win64";%(AdditionalLibraryDirectories)
- 配置屬性 -> 連結器 -> 輸入 -> 附加依賴項
  - 例如 "C:\Program Files\OpenSSL-Win64\lib\VC\x64\MD\libcrypto.lib";kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)`
- Download Win32/Win64 OpenSSL: https://slproweb.com/products/Win32OpenSSL.html
*/
//#include <openssl/sha.h> // 註解掉SHA加密函式庫頭檔案，改用EVP
#include <openssl/evp.h> // 匯入OpenSSL的加密函式庫頭檔案
#include <openssl/err.h> // 匯入OpenSSL的錯誤處理函式庫頭檔案

/**
 * @brief 處理錯誤函式
 *
 * 此函式用於處理錯誤，將錯誤訊息輸出到標準錯誤並立即終止程式運行。
 */
void handleErrors(void) {
	ERR_print_errors_fp(stderr); // 將錯誤訊息輸出到標準錯誤
	abort(); // 立即終止程式運行
}

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
void sha256(char *string, char outputBuffer[65]) {
	EVP_MD_CTX *mdctx; // 宣告訊息摘要內容結構指標
	unsigned char hash[EVP_MAX_MD_SIZE]; // 定義一個字元陣列來儲存哈希值
	unsigned int len; // 宣告一個無符號整數來儲存哈希值長度

	if ((mdctx = EVP_MD_CTX_new()) == NULL) // 初始化訊息摘要內容結構
		handleErrors(); // 如果初始化失敗，處理錯誤

	if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) // 初始化哈希運算，設定為SHA-256
		handleErrors(); // 如果初始化失敗，處理錯誤

	if (1 != EVP_DigestUpdate(mdctx, string, strlen(string))) // 將字串加入到哈希運算中
		handleErrors(); // 如果加入失敗，處理錯誤

	if (1 != EVP_DigestFinal_ex(mdctx, hash, &len)) // 完成哈希運算，獲取最終的哈希值
		handleErrors(); // 如果獲取哈希值失敗，處理錯誤

	EVP_MD_CTX_free(mdctx); // 釋放訊息摘要內容結構佔用的記憶體

	for (int i = 0; i < (int)len; i++) // 將哈希值轉換為十六進位字串
		snprintf(&outputBuffer[i * 2], 3, "%02x", (unsigned int)hash[i]); // 格式化輸出到輸出緩衝區
}

/**
 * @brief 產生哈希值
 * @param format 指定時間格式的字串
 * @param salt 使用的鹽值
 * @param hashOutput 用來儲存生成的SHA-256哈希值
 */
void generate_hash(const char* format, const char* salt, char* hashOutput) {
    time_t now;
    struct tm now_tm;
    char timeString[20];
    char finalString[255];

    time(&now);
#ifdef _WIN32
    localtime_s(&now_tm, &now);
#else
    localtime_r(&now, &now_tm);
#endif

    strftime(timeString, sizeof(timeString), format, &now_tm);
    snprintf(finalString, sizeof(finalString), "%s%s", timeString, salt);

    sha256(finalString, hashOutput);
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
 * @param argc 命令列引數數量
 * @param argv 命令列引數陣列
 * @return 程式執行結果
 */
int main(int argc, char* argv[]) {
    char hashOutput[65];
    const char* salt = "";
    const char* format = "%Y%m%d%H%M%S";
    char* aStr = argv[1];
    if (argc >= 2) {
        if (strcmp(aStr, "/?") == 0 || argcmp(aStr, 'H') == 1 || strcmp(aStr, "--help") == 0) {
            printf("Get the SHA256 of the current timestamp.\n");
            printf("Usage: TSPWD [timestamp precision] [salt]\n");
            printf("    timestamp precision: `y` / `m` / `d` / `h` / `m` / `s`(default)\n");
            printf("    salt: any string of English letters/numbers, less than 200 characters (default is empty).\n");
            printf("    /?  display this help and exit\n");
            printf("    /V  output version information and exit\n");
            return 0;
        }
        else if (argcmp(aStr, 'V') == 1 || strcmp(aStr, "--version") == 0) {
            printf("TSPwd 1.0.1\n");
            printf("Written by Kagurazaka Yashi. https://github.com/kagurazakayashi/NyarukoMiniTools\n");
            printf("License Mulan PSL v2: http://license.coscl.org.cn/MulanPSL2\n");
            printf("This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.\n");
            return 0;
        }

        // 根據命令列引數調整時間格式字串
        if (argcmp(aStr, 'y') == 1) {
            format = "%Y"; // 只顯示年份
        }
        else if (strcmp(aStr, "m") == 0) {
            format = "%Y%m"; // 顯示年份和月份
        }
        else if (argcmp(aStr, 'D') == 1) {
            format = "%Y%m%d"; // 顯示年月日
        }
        else if (argcmp(aStr, 'H') == 1) {
            format = "%Y%m%d%H"; // 顯示到小時
        }
        else if (strcmp(aStr, "M") == 0) {
            format = "%Y%m%d%H%M"; // 顯示到分鐘
        }
    }

    if (argc >= 3) {
        salt = argv[2]; // 如果提供了第三個引數，則將其設定為鹽值
    }

    generate_hash(format, salt, hashOutput);
    printf("%s", hashOutput); // 輸出密後的哈希值

    return 0;
}
