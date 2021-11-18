#include <stdio.h>
#include <string.h>
#include <ctype.h>
short ignoreCase = 0;

int strReplace(char* str, char* sub, char* rep) {
    char* nowStr = str;
    char* tmpStr = NULL;
    char* subStr = NULL;
    char* dstStr = NULL;
    char* src = NULL;
    size_t strLen = 0, subLen = 0, repLen = 0;
    if (str != 0) strLen = strlen(str);
    if (sub != 0) subLen = strlen(sub);
    if (rep != 0) repLen = strlen(rep);
    if (!nowStr) return 0;
    while (*nowStr != '\0') {
        tmpStr = str + strLen;
        if (sub == 0) continue;
        subStr = strstr(str, sub);
        if (subStr == NULL) {
            break;
        }
        src = subStr + subLen;
        dstStr = subStr + repLen;
        memcpy(dstStr, src, tmpStr - src); //MOVE MEM
        memcpy(subStr, rep, repLen);
        strLen = strLen + repLen - subLen;
        nowStr = subStr + repLen;
    }
    str[strLen] = '\0'; //END
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc == 1 || strcmp(argv[1], "/?") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: repstr [MODE] [String] [Find what] [Replace with]\n");
        printf("   String: English only.\n");
        printf("MODE supported:\n");
        printf("   /c: ignore case\n");
        printf("   /?: display this help and exit\n");
        printf("   /V: output version information and exit\n");
        printf("Demo: repstr info\\txt\\readme.txt \\ /\n");
        return 0;
    }
    else if (strcmp(argv[1], "/V") == 0 || strcmp(argv[1], "--version") == 0)
    {
        printf("repstr 1.0.0\n");
        printf("Copyright (C) KagurazakaYashi\n");
        printf("License Mulan PSL v2: <http://license.coscl.org.cn/MulanPSL2>.\n");
        printf("This is free software: you are free to change and redistribute it.\n");
        printf("There is NO WARRANTY, to the extent permitted by law.\n");
        printf("Written by Kagurazaka Yashi.");
        return 0;
    }
    else if (argc < 4 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        printf("E");
        return 1;
    }
    char* str = argv[1];
    printf("%s : %s -> %s\n", str, argv[2], argv[3]);
    if (argc > 4 && strcmp(argv[4], "/c") == 0) {
        strReplace(str, argv[2], tolower(argv[3]));
        strReplace(str, argv[2], toupper(argv[3]));
    }
    else
    {
        strReplace(str, argv[2], argv[3]);
    }
    printf("%s", str);
    return 0;
}