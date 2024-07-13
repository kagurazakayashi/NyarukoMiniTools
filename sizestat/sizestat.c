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

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 5) {
        fprintf(stderr, "Usage: %s <directory_path> [-r] [-i] [intervals]\n", argv[0]);
        return 1;
    }

    signal(SIGINT, handle_sigint);

    char directory_path[1024];
#if defined(_WIN32) || defined(_WIN64)
    strncpy_s(directory_path, sizeof(directory_path), argv[1], _TRUNCATE);
#else
    strncpy(directory_path, argv[1], sizeof(directory_path) - 1);
    directory_path[sizeof(directory_path) - 1] = '\0';
#endif
    ensure_trailing_slash(directory_path);

    int include_subdirs = 0;
    int show_errors = 0;
    const char* intervals_str = "512-1023,1024-2047";

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            include_subdirs = 1;
        }
        else if (strcmp(argv[i], "-i") == 0) {
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
