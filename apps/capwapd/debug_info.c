

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

#include "includes.h"


void dump_data (unsigned char *p, int k)
{
	int i;
	if (debug == 0)
		return;

	for (i = 0; i < k; i++)
	{
		print_info("%02x ", *p++);
		if ((i + 1) % 16 == 0) 
			print_info("\n");
	}
	print_info("\n \n");
}

void print_timestamp(void)
{
		struct os_time tv;
		os_get_time(&tv);
		printf("%ld.%06u: ", (long) tv.sec, (unsigned int) tv.usec);
}

void print_info(const char *fmt, ...)
{
	FILE *fd,*fd2;
	va_list ap;
	va_list aq;
	
	if (debug == 0)
		return;
	
	fd = fopen("/dev/ttyS0","w+");  
	fd2 = fopen("/dev/pts/0","w+");            
	va_start(ap, fmt);
	va_copy(aq,ap);/*两次调用vfprintf，要对ap内容拷贝一份*/
	if(fd != NULL)
	{
		vfprintf(fd,fmt, ap);
		fclose(fd);
	}

	if(fd2 != NULL)
	{
		vfprintf(fd2,fmt, aq);		
		fclose(fd2);
	}
	va_end(ap);
	va_end(aq);
	
}
