#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <windows.h>

void sigHandle(int sig) {
	switch (sig) {
	case SIGINT:
		printf("sigHandle: %d, SIGINT\n", sig);
		break;
	default:
		printf("sigHandle: %d\n", sig);
		break;
	}
	exit(sig);
}

void sleepNow(DWORD dwMillisrconds)
{
#if defined(__linux__) || defined(macintosh)
	usleep(dwMillisrconds * 1000);
#endif
#ifdef _WIN32
	Sleep(dwMillisrconds);
#endif
}

void ref()
{
	int i = 0;
	char str[1001] = { '\0' };
	while (1)
	{
		int scanfcode = scanf("%1000s", &str);
		if (scanfcode != 1) {
			continue;
		}
		for (i = 0; i < 1001; i++)
		{
			str[i] = '\0';
		}
	}
}

void ltime()
{
	int oldsec = -1;
	while (1)
	{
		time_t timep;
		struct tm* p;
		time(&timep);
		p = localtime(&timep);
		if (oldsec == p->tm_sec) {
			continue;
		}
		printf("%4d-%02d-%02d %02d:%02d:%02d\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
		oldsec = p->tm_sec;
		sleepNow(100);
	}
}

int main(int argc, char* argv[])
{
	signal(SIGINT, sigHandle);
	if (argc >= 2) {
		if (strcmp(argv[1], "/?") == 0 || strcmp(argv[1], "--help") == 0) {
			printf("Usage: reflection [MODE]\n");
			printf("MODE supported:\n");
			printf("    (null) : Input as is, output as is. (lenth <= 1000)\n");
			printf("    /T : output the current time every second.\n");
			printf("    /? : display this help and exit.\n");
			printf("    /V : output version information and exit.\n");
			printf("    (other) : output parameters.\n");
			return 0;
		}
		else if (strcmp(argv[1], "/V") == 0 || strcmp(argv[1], "--version") == 0)
		{
			printf("reflection 1.0.0\n");
			printf("Copyright (C) KagurazakaYashi\n");
			printf("License Mulan PSL v2: <http://license.coscl.org.cn/MulanPSL2>.\n");
			printf("This is free software: you are free to change and redistribute it.\n");
			printf("There is NO WARRANTY, to the extent permitted by law.\n");
			printf("Written by Kagurazaka Yashi.");
			return 0;
		}
		else if (strcmp(argv[1], "/T") == 0 || strcmp(argv[1], "--time") == 0)
		{
			ltime();
		}
		else
		{
			for (int i = 1; i < argc; i++)
			{
				printf("%s\n", argv[i]);
			}
			return 0;
		}
	}
	ref();
	return 0;
}
