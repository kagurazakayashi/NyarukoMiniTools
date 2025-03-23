#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <tchar.h>
#define FILE_SEPARATOR "\\"
#else
#include <dirent.h>
#include <unistd.h>
#define FILE_SEPARATOR "/"
#endif

#define MAX_INTERVALS 16 // 指定的檔案大小區間的最大數量
#define MAX_PATHSIZE 4096 // 最大路徑長度 MAX_PATH

#define UNITINFO_FAT   "32KB        64KB"
#define UNITSIZE_FAT   "32768-65535,65536-131071"
#define UNITINFO_FAT32 "512      1024      2048      4096      8192       16KB"
#define UNITSIZE_FAT32 "512-1023,1024-2047,2048-4095,4096-8191,8192-16383,16384-32767"
#define UNITINFO_EXFAT "64KB         128KB         256KB         512KB          1024KB          2048KB          4096KB          8192KB           10384KB           32768KB"
#define UNITSIZE_EXFAT "65536-131071,131072-262143,262144-524287,524288-1048575,1048576-2097151,2097152-4194303,4194304-8388607,8388608-16777215,16777216-33554431,33554432-67108863"
#define UNITINFO_NTFS  "512      1024      2048      4096      8192       16KB        32KB        64KB         128KB         256KB         512KB          1024KB          2048KB" // (DEFAULT)
#define UNITSIZE_NTFS  "512-1023,1024-2047,2048-4095,4096-8191,8192-16383,16384-32767,32768-65535,65536-131071,131072-262143,262144-524287,524288-1048575,1048576-2097151,2097152-4194303"

typedef struct {
    int min_size;
    int max_size;
    int count;
} Interval;

volatile sig_atomic_t stop;

int successful_dirs = 0;
int failed_dirs = 0;
int successful_files = 0;
int failed_files = 0;
long all_size = 0;

char* ConvertWCharToChar(const WCHAR* wStr) {
    if (wStr == NULL) {
        return NULL;
    }

    int bufferSize = WideCharToMultiByte(CP_ACP, 0, wStr, -1, NULL, 0, NULL, NULL);
    if (bufferSize == 0) {
        return NULL;
    }

    char* mbStr = (char*)malloc(bufferSize * sizeof(char));
    if (mbStr == NULL) {
        return NULL;
    }

    int result = WideCharToMultiByte(CP_ACP, 0, wStr, -1, mbStr, bufferSize, NULL, NULL);
    if (result == 0) {
        free(mbStr);
        return NULL;
    }

    return mbStr;
}

WCHAR* ConvertCharToWChar(const char* mbStr) {
    if (mbStr == NULL) {
        return NULL;
    }

    int bufferSize = MultiByteToWideChar(CP_ACP, 0, mbStr, -1, NULL, 0);
    if (bufferSize == 0) {
        return NULL;
    }

    WCHAR* wStr = (WCHAR*)malloc(bufferSize * sizeof(WCHAR));
    if (wStr == NULL) {
        return NULL;
    }

    int result = MultiByteToWideChar(CP_ACP, 0, mbStr, -1, wStr, bufferSize);
    if (result == 0) {
        free(wStr);
        return NULL;
    }

    return wStr;
}

void handle_sigint(int sig) {
    stop = 1;
}

void ensure_trailing_slash(char* path) {
    if (path == NULL) return;
    
    size_t len = strlen(path);
    if (len == 0) return;
    char last_char = path[len - 1];
    if (last_char != '\\' && last_char != '/') {
#if defined(_WIN32) || defined(_WIN64)
        strcat_s(path, 1024, FILE_SEPARATOR);
#else
        strcat(path, FILE_SEPARATOR);
#endif
    }
}

void parse_intervals(const char* intervals_str, Interval* intervals, int* num_intervals) {
    char* intervals_copy = strdup(intervals_str);
    char* context = NULL;
#if defined(_WIN32) || defined(_WIN64)
    char* token = strtok_s(intervals_copy, ",", &context);
#else
    char* token = strtok(intervals_copy, ",");
#endif
    while (token != NULL && *num_intervals < MAX_INTERVALS) {
#if defined(_WIN32) || defined(_WIN64)
        sscanf_s(token, "%d-%d", &intervals[*num_intervals].min_size, &intervals[*num_intervals].max_size);
#else
        sscanf(token, "%d-%d", &intervals[*num_intervals].min_size, &intervals[*num_intervals].max_size);
#endif
        intervals[*num_intervals].count = 0;
        (*num_intervals)++;
#if defined(_WIN32) || defined(_WIN64)
        token = strtok_s(NULL, ",", &context);
#else
        token = strtok(NULL, ",");
#endif
    }
    free(intervals_copy);
}

void scan_file(const char* path, int show_errors, Interval* intervals, int num_intervals) {
    if (path == NULL) return;

    struct stat file_stat;

    if (stat(path, &file_stat) == -1) {
        if (show_errors) perror("stat");
        failed_files++;
        return;
    }

    successful_files++;

    for (int i = 0; i < num_intervals; i++) {
        int size = file_stat.st_size;
        all_size += size;
        if (size >= intervals[i].min_size && size <= intervals[i].max_size) {
            intervals[i].count++;
            break;
        }
    }
}

#if defined(_WIN32) || defined(_WIN64)
void scan_directory(const char* path, int include_subdirs, int show_errors, Interval* intervals, int num_intervals) {
    if (path == NULL) return;

    WIN32_FIND_DATA find_data;
    HANDLE hFind;
    char search_path[MAX_PATHSIZE];

    snprintf(search_path, sizeof(search_path), "%s*.*", path);

    if ((hFind = FindFirstFile(ConvertCharToWChar(search_path), &find_data)) == INVALID_HANDLE_VALUE) {
        if (show_errors) perror("FindFirstFile");
        failed_dirs++;
        return;
    }

    successful_dirs++;

    do {
        char* fileName = ConvertWCharToChar(find_data.cFileName);
        if (fileName == NULL) continue;
        
        if (strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0) {
            free(fileName);
            continue;
        }

        char full_path[MAX_PATHSIZE];
        if (path != NULL) {
            snprintf(full_path, sizeof(full_path), "%s%s", path, fileName);
        } else {
            fprintf(stderr, "Error: path is NULL.\n");
            free(fileName);
            continue;
        }

        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (include_subdirs) {
                ensure_trailing_slash(full_path);
                if (show_errors) printf("scanning directory: %s\n", full_path);
                scan_directory(full_path, include_subdirs, show_errors, intervals, num_intervals);
            }
        }
        else {
            if (show_errors) printf("scanning file: %s\n", full_path);
            scan_file(full_path, show_errors, intervals, num_intervals);
        }

        free(fileName);
    } while (FindNextFile(hFind, &find_data) != 0);

    FindClose(hFind);
    return;
}
#else
void scan_directory(const char* path, int include_subdirs, int show_errors, Interval* intervals, int num_intervals) {
    if (path == NULL) return;

    DIR* dir;
    struct dirent* entry;
    struct stat file_stat;
    char full_path[1024];

    if ((dir = opendir(path)) == NULL) {
        if (show_errors) perror("opendir");
        failed_dirs++;
        return;
    }

    successful_dirs++;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        snprintf(full_path, sizeof(full_path), "%s%s", path, entry->d_name);
        if (stat(full_path, &file_stat) == -1) {
            if (show_errors) {
                printf("scanning error: %s\n", full_path);
                perror("stat");
            }
            failed_files++;
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            if (include_subdirs) {
                ensure_trailing_slash(full_path);
                if (show_errors) printf("scanning directory: %s\n", full_path);
                scan_directory(full_path, include_subdirs, show_errors, intervals, num_intervals);
            }
        }
        else {
            if (show_errors) printf("scanning file: %s\n", full_path);
            scan_file(full_path, show_errors, intervals, num_intervals);
        }
    }

    closedir(dir);
}
#endif

const char* format_size(int size) {
    static char output[20];
    const char* units[] = { "B", "K", "M", "G", "T", "E" };
    int unit_index = 0;
    double size_d = (double)size;
    while (size_d >= 1024 && unit_index < 5) {
        size_d /= 1024;
        unit_index++;
    }
    snprintf(output, sizeof(output), "%.0f %s", size_d, units[unit_index]);
    return output;
}

void display_intervals(Interval* intervals, int num_intervals) {
    for (int i = 0; i < num_intervals; i++) {
        if (intervals[i].min_size == -1) {
            printf("Files with size < %d bytes (%s): %d\n", intervals[i].max_size + 1, format_size(intervals[i].max_size + 1), intervals[i].count);
        }
        else if (intervals[i].max_size == INT_MAX) {
            printf("Files with size > %d bytes (%s): %d\n", intervals[i].min_size - 1, format_size(intervals[i].min_size - 1), intervals[i].count);
        }
        else {
            printf("Files with size %d - %d bytes (%s - %s): %d\n", intervals[i].min_size, intervals[i].max_size, format_size(intervals[i].min_size), format_size(intervals[i].max_size), intervals[i].count);
        }
    }
    printf("Directories: All=%d, Successful=%d, Failed=%d\n", successful_dirs + failed_dirs, successful_dirs, failed_dirs);
    printf("Files: All=%d, Successful=%d, Failed=%d\n", successful_files + failed_files, successful_files, failed_files);
    printf("Total size: %s bytes (%ld)\n", format_size(all_size), all_size);
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
int argcmp(const char* str, char k) {
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
 * @brief 程式入口函式。
 *
 * 這個函式會解析命令列引數，並根據引數的值來決定休眠的時間。
 *
 * @param argc 命令列引數的數量。
 * @param argv 命令列引數的陣列。
 *
 * @return 程式的結束狀態。
 */
int main(int argc, char* argv[]) {
    char* aStr = argv[1];
	if (argcmp(aStr, 'V') == 1 || strcmp(aStr, "--version") == 0)
	{
		printf("sizestat 1.0.0\n");
		printf("Written by Kagurazaka Yashi. https://github.com/kagurazakayashi/NyarukoMiniTools\n");
		printf("License Mulan PSL v2: http://license.coscl.org.cn/MulanPSL2\n");
		printf("This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.\n");
		return 0;
	}
	else if (strcmp(aStr, "/?") == 0 || argcmp(aStr, 'H') == 1 || strcmp(aStr, "--help") == 0 || argc < 2 || argc > 5)
	{
		printf("Usage: %s <Directory Path> [/R] [/D] [Interval List]\n", argv[0]);
		printf("MODE supported:\n");
		printf("    /R  Scan Subfolders.\n");
		printf("    /D  Display detailed scan location information.\n");
		printf("    /?  display this help and exit.\n");
		printf("    /V  output version information and exit.\n");
		printf("Interval List:\n");
		printf("    Format:  <from bytes>-<to bytes>,<from bytes>-<to bytes>,...\n");
		printf("    Common values:\nCommon allocation unit sizes for file systems formatted according to Windows:\n");
        printf("  FAT: %s\n  FAT: %s\n", UNITINFO_FAT, UNITSIZE_FAT);
        printf("FAT32: %s\nFAT32: %s\n", UNITINFO_FAT32, UNITSIZE_FAT32);
        printf("EXFAT: %s\nEXFAT: %s\n", UNITINFO_EXFAT, UNITSIZE_EXFAT);
        printf(" NTFS: %s\n NTFS: %s\n", UNITINFO_NTFS, UNITSIZE_NTFS);
		return 0;
	}

    signal(SIGINT, handle_sigint);

    char directory_path[MAX_PATHSIZE];
#if defined(_WIN32) || defined(_WIN64)
    strncpy_s(directory_path, sizeof(directory_path), argv[1], _TRUNCATE);
#else
    strncpy(directory_path, argv[1], sizeof(directory_path) - 1);
    directory_path[sizeof(directory_path) - 1] = '\0';
#endif
    ensure_trailing_slash(directory_path);

    int include_subdirs = 0;
    int show_errors = 0;
    const char* intervals_str = UNITSIZE_NTFS;

    for (int i = 2; i < argc; i++) {
        if (argcmp(argv[i], "R") == 1 || strcmp(argv[i], "--subfolders") == 0) {
            include_subdirs = 1;
        }
        else if (argcmp(argv[i], "D") == 1 || strcmp(argv[i], "--detail") == 0) {
            show_errors = 1;
        }
        else {
            intervals_str = argv[i];
        }
    }

    Interval intervals[MAX_INTERVALS + 2]; // Extra intervals for smaller and larger files
    int num_intervals = 0;
    parse_intervals(intervals_str, intervals, &num_intervals);

    // Adding intervals for smaller and larger files
    intervals[num_intervals].min_size = -1;
    intervals[num_intervals].max_size = intervals[0].min_size - 1;
    intervals[num_intervals].count = 0;
    num_intervals++;

    intervals[num_intervals].min_size = intervals[num_intervals - 2].max_size + 1;
    intervals[num_intervals].max_size = INT_MAX;
    intervals[num_intervals].count = 0;
    num_intervals++;

    scan_directory(directory_path, include_subdirs, show_errors, intervals, num_intervals);

    printf("\nFinal Statistics:\n");
    display_intervals(intervals, num_intervals);

    return 0;
}
