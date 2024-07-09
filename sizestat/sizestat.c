#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>

// #if defined(_WIN32) || defined(_WIN64)
#define FILE_SEPARATOR "\\"
// #else
//     #define FILE_SEPARATOR "/"
// #endif

//      UNITSIZE_FAT   32KB  64KB
#define UNITSIZE_FAT   "32768-65535,65536-131071"
//      UNITSIZE_FAT32 512 1024 2048 4096 8192 16KB
#define UNITSIZE_FAT32 "512-1023,1024-2047,2048-4095,4096-8191,8192-16383,16384-32767"
//      UNITSIZE_EXFAT 64KB  128KB  256KB  512KB  1024KB  2048KB  4096KB  8192KB  10384KB  32768KB
#define UNITSIZE_EXFAT "65536-131071,131072-262143,262144-524287,524288-1048575,1048576-2097151,2097152-4194303,4194304-8388607,8388608-16777215,16777216-33554431,33554432-67108863"
//      UNITSIZE_NTFS  512 1024 2048 4096 8192 16KB  32KB  64KB  128KB  256KB  512KB  1024KB  2048KB
#define UNITSIZE_NTFS  "512-1023,1024-2047,2048-4095,4096-8191,8192-16383,16384-32767,32768-65535,65536-131071,131072-262143,262144-524287,524288-1048575,1048576-2097151,2097152-4194303"

#define MAX_INTERVALS 16 // 指定的檔案大小區間的最大數量

typedef struct {
    int min_size;
    int max_size;
    int count;
} Interval;

volatile sig_atomic_t stop;

void handle_sigint(int sig) {
    stop = 1;
}

void ensure_trailing_slash(char* path) {
    size_t len = strlen(path);
    if (len == 0) return;
    // unify_file_separator(path);
    char last_char = path[len - 1];
    if (last_char != '\\' && last_char != '/') {
        strcat(path, FILE_SEPARATOR);
    }
}

void parse_intervals(const char* intervals_str, Interval* intervals, int* num_intervals) {
    char* intervals_copy = strdup(intervals_str);
    char* token = strtok(intervals_copy, ",");
    while (token != NULL && *num_intervals < MAX_INTERVALS) {
        sscanf(token, "%d-%d", &intervals[*num_intervals].min_size, &intervals[*num_intervals].max_size);
        intervals[*num_intervals].count = 0;
        (*num_intervals)++;
        token = strtok(NULL, ",");
    }
    free(intervals_copy);
}

void scan_file(const char* path, int show_errors, Interval* intervals, int num_intervals, int* successful_files, int* failed_files, long* all_size) {
    struct stat file_stat;

    if (stat(path, &file_stat) == -1) {
        if (show_errors) perror("stat");
        (*failed_files)++;
        return;
    }

    (*successful_files)++;

    for (int i = 0; i < num_intervals; i++) {
        int size = file_stat.st_size;
        (*all_size) += size;
        if (size >= intervals[i].min_size && size <= intervals[i].max_size) {
            intervals[i].count++;
            break;
        }
    }
}

void scan_directory(const char* path, int include_subdirs, int show_errors, Interval* intervals, int num_intervals, int* successful_dirs, int* failed_dirs, int* successful_files, int* failed_files, long* all_size) {
    DIR* dir;
    struct dirent* entry;
    struct stat file_stat;
    char full_path[1024];

    if ((dir = opendir(path)) == NULL) {
        if (show_errors) perror("opendir");
        (*failed_dirs)++;
        return;
    }

    (*successful_dirs)++;

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        snprintf(full_path, sizeof(full_path), "%s%s", path, entry->d_name);
        if (stat(full_path, &file_stat) == -1) {
            if (show_errors) {
                if (show_errors) printf("scanning error: %s\n", full_path);
                perror("stat");
            }
            (*failed_files)++;
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            if (include_subdirs) {
                ensure_trailing_slash(full_path);
                if (show_errors) printf("scanning directory: %s\n", full_path);
                scan_directory(full_path, include_subdirs, show_errors, intervals, num_intervals, successful_dirs, failed_dirs, successful_files, failed_files, all_size);
            }
        }
        else {
            if (show_errors) printf("scanning file: %s\n", full_path);
            scan_file(full_path, show_errors, intervals, num_intervals, successful_files, failed_files, all_size);
        }
    }

    closedir(dir);
}

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

void display_intervals(Interval* intervals, int num_intervals, int successful_dirs, int failed_dirs, int successful_files, int failed_files, long all_size) {
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
    printf("Total size: %s bytes (%ld)\n", all_size, format_size(all_size));
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 5) {
        fprintf(stderr, "Usage: %s <directory_path> [-r] [-i] [intervals]\n", argv[0]);
        return 1;
    }

    signal(SIGINT, handle_sigint);

    char directory_path[1024];
    strncpy(directory_path, argv[1], sizeof(directory_path) - 1);
    directory_path[sizeof(directory_path) - 1] = '\0';
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

    int successful_dirs = 0, failed_dirs = 0, successful_files = 0, failed_files = 0;
    long all_size = 0;
    scan_directory(directory_path, include_subdirs, show_errors, intervals, num_intervals, &successful_dirs, &failed_dirs, &successful_files, &failed_files, &all_size);

    printf("\nFinal Statistics:\n");
    display_intervals(intervals, num_intervals, successful_dirs, failed_dirs, successful_files, failed_files, all_size);

    return 0;
}