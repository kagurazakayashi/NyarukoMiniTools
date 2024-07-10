#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#define CACHE_MAX 1073741824 // 1GB 缓冲区

// 全局变量
char* line_buffer = NULL;
char* byte_buffer = NULL;
wchar_t* char_buffer = NULL;

// 函数：初始化全局缓冲区
void init_buffers(long bytes_per_file, long chars_per_file) {
    if (line_buffer == NULL) {
        line_buffer = (char*)malloc(CACHE_MAX);
        if (line_buffer == NULL) {
            perror("Error allocating memory for line buffer");
            exit(EXIT_FAILURE);
        }
    }
    if (byte_buffer == NULL) {
        byte_buffer = (char*)malloc((size_t)bytes_per_file);
        if (byte_buffer == NULL) {
            perror("Error allocating memory for byte buffer");
            exit(EXIT_FAILURE);
        }
    }
    if (char_buffer == NULL) {
        char_buffer = (wchar_t*)malloc((size_t)chars_per_file * sizeof(wchar_t));
        if (char_buffer == NULL) {
            perror("Error allocating memory for char buffer");
            exit(EXIT_FAILURE);
        }
    }
}

// 函数：释放全局缓冲区
void free_buffers() {
    if (line_buffer) free(line_buffer);
    if (byte_buffer) free(byte_buffer);
    if (char_buffer) free(char_buffer);
}

// 函数：计算文件总行数
long count_lines(const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "r") != 0) {
        perror("Error opening file to count lines");
        exit(EXIT_FAILURE);
    }

    long line_count = 0;

    while (fgets(line_buffer, CACHE_MAX, file)) {
        line_count++;
    }

    fclose(file);
    printf("line_count = %ld", line_count);
    return line_count;
}

// 函数：拆分文件（按行）
void split_file_by_lines(const char* input_file, int lines_per_file) {
    FILE* input;
    if (fopen_s(&input, input_file, "r") != 0) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    char output_file_name[256];
    int file_count = 1;
    int line_count = 0;
    //long total_lines = count_lines(input_file);
    long current_line = 0;
    char nowProgress[7] = { '\0' };
    char nowProgressTemp[7] = { '\0' };

    // 获取输入文件的基本名称和扩展名
    char base_name[256];
    char* dot = strrchr(input_file, '.');
    if (dot == NULL) {
        strcpy_s(base_name, sizeof(base_name), input_file);
        strcpy_s(output_file_name, sizeof(output_file_name), input_file);
    }
    else {
        strncpy_s(base_name, sizeof(base_name), input_file, dot - input_file);
        base_name[dot - input_file] = '\0';
    }

    sprintf_s(output_file_name, sizeof(output_file_name), "%s_%d%s", base_name, file_count, dot ? dot : "");
    printf("%s\n", output_file_name);

    FILE* output;
    if (fopen_s(&output, output_file_name, "w") != 0) {
        perror("Error opening output file");
        fclose(input);
        exit(EXIT_FAILURE);
    }

    while (fgets(line_buffer, CACHE_MAX, input)) {
        // 检查行长度是否超过 CACHE_MAX
        if (strlen(line_buffer) == CACHE_MAX - 1 && line_buffer[CACHE_MAX - 2] != '\n' && line_buffer[CACHE_MAX - 2] != '\r') {
            fprintf(stderr, "Warning: Line exceeds maximum length of %d characters\n", CACHE_MAX - 1);
            // 跳过剩余的超长行
            int ch;
            while ((ch = fgetc(input)) != '\n' && ch != EOF);
        }

        // 处理 \r\n 换行符
        char* newline = strchr(line_buffer, '\r');
        if (newline) {
            *newline = '\n';
            *(newline + 1) = '\0';
        }

        if (line_count == lines_per_file) {
            fclose(output);
            file_count++;
            sprintf_s(output_file_name, sizeof(output_file_name), "%s_%d%s", base_name, file_count, dot ? dot : "");
            printf("%s\n", output_file_name);
            if (fopen_s(&output, output_file_name, "w") != 0) {
                perror("Error opening new output file");
                fclose(input);
                exit(EXIT_FAILURE);
            }
            line_count = 0;
        }
        fputs(line_buffer, output);
        line_count++;
        current_line++;


        // 打印百分比进度
        //sprintf_s(nowProgressTemp, 7, "%.2f", (double)current_line / total_lines * 100);
        //if (nowProgressTemp != nowProgress) {
            //strcpy_s(nowProgress, 7, nowProgressTemp);
            //printf("\rProgress: %d / %d ( %.2f %% )", current_line, total_lines, (double)current_line / total_lines * 100);
            //fflush(stdout);
        //}
    }

    printf("\n"); // 确保在进度显示后换行
    fclose(output);
    fclose(input);
}

// 函数：拆分文件（按字节）
void split_file_by_bytes(const char* input_file, long bytes_per_file) {
    FILE* input;
    if (fopen_s(&input, input_file, "rb") != 0) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    char output_file_name[256];
    int file_count = 1;
    size_t bytes_read;

    // 获取输入文件的基本名称和扩展名
    char base_name[256];
    char* dot = strrchr(input_file, '.');
    if (dot == NULL) {
        strcpy_s(base_name, sizeof(base_name), input_file);
        strcpy_s(output_file_name, sizeof(output_file_name), input_file);
    }
    else {
        strncpy_s(base_name, sizeof(base_name), input_file, dot - input_file);
        base_name[dot - input_file] = '\0';
    }

    sprintf_s(output_file_name, sizeof(output_file_name), "%s_%d%s", base_name, file_count, dot ? dot : "");
    printf("%s\n", output_file_name);

    FILE* output;
    if (fopen_s(&output, output_file_name, "wb") != 0) {
        perror("Error opening output file");
        fclose(input);
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = fread(byte_buffer, 1, (size_t)bytes_per_file, input)) > 0) {
        fwrite(byte_buffer, 1, bytes_read, output);
        if ((long)bytes_read < bytes_per_file) {
            break; // End of file reached
        }
        fclose(output);
        file_count++;
        sprintf_s(output_file_name, sizeof(output_file_name), "%s_%d%s", base_name, file_count, dot ? dot : "");
        printf("%s\n", output_file_name);
        if (fopen_s(&output, output_file_name, "wb") != 0) {
            perror("Error opening new output file");
            fclose(input);
            exit(EXIT_FAILURE);
        }
    }

    fclose(output);
    fclose(input);
}

// 函数：拆分文件（按字符）
void split_file_by_chars(const char* input_file, long chars_per_file) {
    setlocale(LC_ALL, "");
    FILE* input;
    if (fopen_s(&input, input_file, "r") != 0) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    char output_file_name[256];
    int file_count = 1;
    size_t chars_read;

    // 获取输入文件的基本名称和扩展名
    char base_name[256];
    char* dot = strrchr(input_file, '.');
    if (dot == NULL) {
        strcpy_s(base_name, sizeof(base_name), input_file);
        strcpy_s(output_file_name, sizeof(output_file_name), input_file);
    }
    else {
        strncpy_s(base_name, sizeof(base_name), input_file, dot - input_file);
        base_name[dot - input_file] = '\0';
    }

    sprintf_s(output_file_name, sizeof(output_file_name), "%s_%d%s", base_name, file_count, dot ? dot : "");
    printf("%s\n", output_file_name);

    FILE* output;
    if (fopen_s(&output, output_file_name, "w") != 0) {
        perror("Error opening output file");
        fclose(input);
        exit(EXIT_FAILURE);
    }

    while ((chars_read = fread(char_buffer, sizeof(wchar_t), (size_t)chars_per_file, input)) > 0) {
        fwrite(char_buffer, sizeof(wchar_t), chars_read, output);
        if ((long)chars_read < chars_per_file) {
            break; // End of file reached
        }
        fclose(output);
        file_count++;
        sprintf_s(output_file_name, sizeof(output_file_name), "%s_%d%s", base_name, file_count, dot ? dot : "");
        printf("%s\n", output_file_name);
        if (fopen_s(&output, output_file_name, "w") != 0) {
            perror("Error opening new output file");
            fclose(input);
            exit(EXIT_FAILURE);
        }
    }

    fclose(output);
    fclose(input);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <number>[L|B|C]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* input_file = argv[1];
    char* endptr;
    long num = strtol(argv[2], &endptr, 10);
    char mode = endptr[0];

    if (num <= 0) {
        fprintf(stderr, "The number must be a positive integer.\n");
        exit(EXIT_FAILURE);
    }

    // 初始化缓冲区
    init_buffers(CACHE_MAX, num);

    switch (mode) {
    case 'L':
    case 'l':
        split_file_by_lines(input_file, (int)num);
        break;
    case 'B':
    case 'b':
    case '\0': // 默认按字节处理
        split_file_by_bytes(input_file, num);
        break;
    case 'C':
    case 'c':
        split_file_by_chars(input_file, num);
        break;
    default:
        fprintf(stderr, "Invalid mode. Use L for lines, B for bytes, C for characters.\n");
        free_buffers();
        exit(EXIT_FAILURE);
    }

    printf("File split successfully.\n");

    // 释放缓冲区
    free_buffers();

    return 0;
}
