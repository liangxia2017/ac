#include "get_running_time.h"


/*获取系统运行时间,返回值为秒*/
long int get_running_time_in_seconds()
{
#if 0
	int updays, uphours, upminutes;
	struct sysinfo info;
	struct tm *current_time;
	time_t current_secs;
	time(&current_secs);
	current_time = localtime(&current_secs);
	sysinfo(&info);
	printf(" %02d:%02d:%02d up ",
	       current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
	updays = (int) info.uptime / (60 * 60 * 24);

	if (updays)
		printf("%d day%s, ", updays, (updays != 1) ? "s" : "");

	upminutes = (int) info.uptime / 60;
	uphours = (upminutes / 60) % 24;
	upminutes %= 60;

	if (uphours)
		printf("%2d:%02d, ", uphours, upminutes);
	else
		printf("%d min, ", upminutes);

	return 0;
#else
	struct sysinfo info;
	sysinfo(&info);
	return info.uptime;
#endif
}

void get_running_time_string(char * str)
{
	unsigned long u_int_ret = get_running_time_in_seconds();
	int day = 0, hour = 0, minu = 0, second = 0, tmp_time = 0;
	day = u_int_ret / 86400;
	tmp_time = u_int_ret % 86400;
	hour = tmp_time / 3600;
	tmp_time = tmp_time % 3600;
	minu = tmp_time / 60;
	second = tmp_time % 60;

	if (day)
		sprintf(str, "%d天 ", day);

	sprintf(str + strlen(str), "%d小时%2d分钟%2d秒", hour, minu, second);
}
