/*
Copyright (c) 2020 KagurazakaYashi
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
#include <stdlib.h>
#include <windows.h>
#define TRYHELP "\nTry 'SLEEP /?' for more information."
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "SLEEP: missing operand%s", TRYHELP);
        return 1;
    }
    char* argvstr = argv[1];
    if (strcmp(argvstr, "/?") == 0 || strcmp(argvstr, "--help") == 0)
    {
        printf("Usage: SLEEP NUMBER[SUFFIX]...\n   or: SLEEP OPTION\n");
        printf("Pause for NUMBER seconds. SUFFIX may be 's' for seconds (the default),\n");
        printf("'m' for minutes, 'h' for hours or 'd' for days.\n");
        printf("Here NUMBER may be an arbitrary floating point number.\n");
        printf("Given two or more arguments, pause for the amount of time specified by the sum of their values.\n");
        printf("\n    /?  display this help and exit");
        printf("\n    /V  output version information and exit");
        return 0;
    }
    else if (strcmp(argvstr, "/V") == 0 || strcmp(argvstr, "--version") == 0)
    {
        printf("Sleep in Windows 1.0.0\n");
        printf("Copyright (C) KagurazakaYashi\n");
        printf("License Mulan PSL v2: <http://license.coscl.org.cn/MulanPSL2>.\n");
        printf("This is free software: you are free to change and redistribute it.\n");
        printf("There is NO WARRANTY, to the extent permitted by law.\n");
        printf("Written by Kagurazaka Yashi.");
        return 0;
    }
    double total = 0;
    for (int i = 1; i < argc; i++)
    {
        char* inNum = argv[i];
        int inLen = (int)strlen(inNum);
        char num[10] = { '\0' };
        char unit = 's';
        for (int j = 0; j < inLen; j++)
        {
            char nowchar = inNum[j];
            if ((nowchar >= 48 && nowchar <= 57) || nowchar == 46)
            {
                num[j] = nowchar;
            }
            else
            {
                unit = nowchar;
            }
        }
        if (num[0] == '\0')
        {
            fprintf(stderr, "SLEEP: invalid time interval '%s'%s", inNum, TRYHELP);
            return 1;
        }
        double numi = atof(num);
        if (unit >= 65 && unit <= 90)
        {
            unit = unit + 32;
        }
        if (unit == 'm' || unit == 'h' || unit == 'd')
        {
            if (unit <= 'm')
            {
                numi *= 60;
            }
            if (unit <= 'h')
            {
                numi *= 60;
            }
            if (unit <= 'd')
            {
                numi *= 24;
            }
        }
        numi *= 1000;
        total += numi;
    }
    Sleep((DWORD)total);
    return 0;
}