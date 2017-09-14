#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>
/* Separators */
#define SEP_EQUAL '='
#define SEP_SPACE ' '
#define SEP_TAB (char)0x9
#define SEP_Q_MARK	'"'
#define KEYS_MAX 2048

typedef struct prop_data
{
	char *key;
	char *value;
}prop_data;

int main(int argc, char *argv[])
{
	int opt;
	int num;
	char base_mac[256] = {0};
	char licence_num[32] = {0};
	char tmp[256] = {0};
	char cmd[1024] = {0};
	
	prop_data properties[KEYS_MAX];
	int prop_count = 0;
	
	while ((opt =  getopt(argc, argv, "m:n:")) != -1)
	{
			switch (opt) 
			{
				case 'm':
					strcpy(base_mac,optarg);
					break;
				case 'n':
					strcpy(licence_num,optarg);
					num = atoi(optarg);
					break;
				default:
					break;
			}
	}
	
	if((num<16 || num > 4064) && (num%16 == 0) )
	{
		printf("make licence fail beacause of invalid -n\n");
		return 0;
	}
	if(strlen(base_mac) == 0)
	{
		printf("make licence fail beacause of empty -b\n");
		return 0;
	}
	system("touch /tmp/license.conf");
	sprintf(cmd,"echo LICENCE=%d > /tmp/license.conf",num/16);
	system(cmd);
	system("cd /tmp/;tar -czf license.tmp license.conf");
	sprintf(cmd,"rm -f /home/license.bin;/ac/sbin/crypt -q -K %s -f /tmp/license.tmp -o /home/license.bin",base_mac);
	system(cmd);
	system("rm -f /tmp/license*");
	return  0;
}




