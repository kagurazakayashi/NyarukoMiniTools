/*
Copyright (c) 2021 KagurazakaYashi
sleep is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details.
*/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>
#if defined(__linux__) || defined(macintosh)
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

char allSelect[94] = { 0 };
unsigned int seedi = 0;

short charMode(char* nowChar)
{
    int nowCharI = (char)nowChar;
    if (nowCharI == 78 || nowCharI == 110) { //Nn
        return 3;
    }
    if (nowCharI == 83 || nowCharI == 115) { //Ss
        return 4;
    }
    if (nowCharI >= 65 && nowCharI <= 90) { //A
        return 1;
    }
    if (nowCharI >= 97 && nowCharI <= 122) { //a
        return 2;
    }
    return 0;
}

void genSelect(char* selArgv) {
    int j, k, allSelectI = 0;
    for (j = 0; j < (int)strlen(selArgv); j++)
    {
        short nowMode = charMode((char*)selArgv[j]);
        if (nowMode == 1) {
            for (k = 65; k <= 90; k++) {
                allSelect[allSelectI++] = k;
            }
        }
        else if (nowMode == 2) {
            for (k = 97; k <= 122; k++) {
                allSelect[allSelectI++] = k;
            }
        }
        else if (nowMode == 3) {
            for (k = 48; k <= 57; k++) {
                allSelect[allSelectI++] = k;
            }
        }
        else if (nowMode == 4) {
            for (k = 33; k <= 47; k++) {
                allSelect[allSelectI++] = k;
            }
            for (k = 58; k <= 64; k++) {
                allSelect[allSelectI++] = k;
            }
            for (k = 91; k <= 96; k++) {
                allSelect[allSelectI++] = k;
            }
            for (k = 123; k <= 126; k++) {
                allSelect[allSelectI++] = k;
            }
        }
    }
}

int genRand(int min, int max) {
    unsigned int ms;
    int sleepTime = rand() % (100 - 10 + 1) + 10;
    #if defined(__linux__) || defined(macintosh)
        struct timeval tv;
        gettimeofday(&tv, NULL);
        ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        usleep(sleepTime * 1000);
    #endif
    #ifdef _WIN32
        struct timeb tv;
        ftime(&tv);
        ms = (unsigned int)tv.time * 1000 + (unsigned int)tv.millitm;
        Sleep(sleepTime);
    #endif
    srand(ms);
    return rand() % (max - min + 1) + min;
}

int main(int argc, char* argv[])
{
    int i, j = 0;
    int aCount = 1;
    int aLength = 32;
    char aSelect[4] = { 'a','A','N' };
    short noArg1 = 0;
    for (i = 1; i < argc; i++)
    {
        char* nowArgv = argv[i];
        if (i == 1) { // mode
            if (strcmp(nowArgv, "/?") == 0 || strcmp(nowArgv, "--help") == 0) {
                printf("Usage: genpwd [MODE] [pw_length] [num_pw]\n");
                printf("         or   [pw_length] [num_pw]\n");
                printf("         or   [pw_length]\n");
                printf("MODE supported:\n");
                printf("    A: Upper case\n");
                printf("    a: Lower case\n");
                printf("    n: Number\n");
                printf("    s: Symbol\n");
                printf("   /?: display this help and exit\n");
                printf("   /V: output version information and exit\n");
                printf("Demo: genpwd Aan 32 1\n");
                return 0;
            }
            if (strcmp(nowArgv, "/V") == 0 || strcmp(nowArgv, "--version") == 0)
            {
                printf("genpwd 1.0.0\n");
                printf("Copyright (C) KagurazakaYashi\n");
                printf("License Mulan PSL v2: <http://license.coscl.org.cn/MulanPSL2>.\n");
                printf("This is free software: you are free to change and redistribute it.\n");
                printf("There is NO WARRANTY, to the extent permitted by law.\n");
                printf("Written by Kagurazaka Yashi.");
                return 0;
            }
            short modeChk = 0;
            for (j = 0; j < (int)strlen(nowArgv); j++)
            {
                modeChk += charMode((char*)nowArgv[j]);
            }
            if (modeChk == 0) {
                noArg1 = 1;
                aLength = atoi(nowArgv);
                genSelect(aSelect);
            }
            else {
                genSelect(nowArgv);
            }
        }
        else if (i == 2) {
            if (noArg1 == 1) {
                aCount = atoi(nowArgv);
            }
            else {
                aLength = atoi(nowArgv);
            }
        }
        else if (i == 3 && noArg1 == 0) {
            aCount = atoi(nowArgv);
        }
    }
    int selectCharLen = (int)strlen(allSelect);
    if (selectCharLen == 0) {
        genSelect(aSelect);
        selectCharLen = (int)strlen(allSelect);
    }
    //printf("Count=%d, Length=%d, Select=(%d)%s\n", aCount, aLength, selectCharLen, allSelect);
    for (i = 0; i < aCount; i++) {
        for (j = 0; j < aLength; j++) {
            if (selectCharLen == 0) exit(-1);
            int rCharIndex = genRand(0, selectCharLen - 1);
            char rChar = allSelect[rCharIndex];
            printf("%c", rChar);
        }
        printf("\n");
    }
    return 0;
}
