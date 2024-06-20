/*
Copyright (c) 2020 KagurazakaYashi
datetime is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
*/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
void mvchars(char* cArr, int cLen, char cNew)
{
    for (int i = 0; i < cLen; i++)
    {
        if (i + 1 >= cLen)
        {
            cArr[i] = cNew;
            return;
        }
        cArr[i] = cArr[i + 1];
    }
}
void rnum4(char* aStr, int aLen, int num, char* rchar, int cSize)
{
    int i, j;
    int cSize0 = cSize - 1;
    char str[5] = { '\0' };
    for (i = 0; i < aLen; i++)
    {
        char nc = aStr[i];
        mvchars(str, cSize, nc);
        if (strcmp(str, rchar) == 0) {
            char dateChar[5] = { '\0' };
            if (cSize == 2) {
                sprintf_s(dateChar, 3, "%02d", num);
            } else if (cSize == 4) {
                sprintf_s(dateChar, 5, "%04d", num);
            }
            for (j = 0; j < cSize; j++)
            {
                aStr[i - (cSize0 - j)] = dateChar[j];
            }
            i += cSize0;
        }
    }
}
int main(int argc, char* argv[])
{
    time_t now;
    time(&now);
    struct tm time;
    localtime_s(&time, &now);
    if (argc < 2)
    {
        printf("%4d-%02d-%02d %02d:%02d:%02d", 1900 + time.tm_year, 1 + time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
        return 0;
    }
    if (argc == 3 && strcmp(argv[2], "gmt") == 0) {
        gmtime_s(&time, &now);
    }
    char* aStr = argv[1];
    if (strcmp(aStr, "/?") == 0 || strcmp(aStr, "--help") == 0)
    {
        printf("Usage: datetime MODE [TIME]\n");
        printf("MODE:\n");
        printf("    Support: yyyy,yy,MM,dd,HH,hh,mm,ss\n");
        printf("  Unsupport: y,M,d,H,h,m,s,ddd,dddd,MMM,MMMM,gg ...\n");
        printf("TIME:\n  \"gmt\" or \"local\" (the default)\n");
        printf("DEMO:\n  datetime \"yyyy-MM-dd HH:mm:ss\"\n        ->  2021-02-01 20:21:11\n");
        printf("\ndatetime /?  display this help and exit");
        printf("\ndatetime /V  output version information and exit");
        return 0;
    }
    if (strcmp(aStr, "/V") == 0 || strcmp(aStr, "--version") == 0)
    {
        printf("datetime 1.0.0\n");
        printf("Copyright (C) KagurazakaYashi\n");
        printf("License Mulan PSL v2: <http://license.coscl.org.cn/MulanPSL2>.\n");
        printf("This is free software: you are free to change and redistribute it.\n");
        printf("There is NO WARRANTY, to the extent permitted by law.\n");
        printf("Written by Kagurazaka Yashi.");
        return 0;
    }
    int aLen = (int)strlen(aStr);
    int nv = time.tm_year + 1900;
    rnum4(aStr, aLen, nv, "yyyy", 4);
    nv = (nv / 10 % 10) * 10 + (nv / 1 % 10);
    rnum4(aStr, aLen, nv, "yy", 2);
    nv = time.tm_mon + 1;
    rnum4(aStr, aLen, nv, "MM", 2);
    rnum4(aStr, aLen, time.tm_mday, "dd", 2);
    nv = time.tm_hour;
    rnum4(aStr, aLen, nv, "HH", 2);
    if (nv > 12) nv -= 12;
    rnum4(aStr, aLen, nv, "hh", 2);
    rnum4(aStr, aLen, time.tm_min, "mm", 2);
    rnum4(aStr, aLen, time.tm_sec, "ss", 2);
    printf("%s", aStr);
    return 0;
}