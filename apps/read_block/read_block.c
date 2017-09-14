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
	int result,ret;
	char base_mac[256] = {0};
	char tmp[256] = {0};
	char cmd[1024] = {0};
	char *licence_path = NULL;
	char *ver_path = NULL;
	prop_data properties[KEYS_MAX];
	int prop_count = 0;
	
	while ((opt =  getopt(argc, argv, "l:c:")) != -1)
		{
			switch (opt) 
				{
					case 'l'://¶Álicense
						licence_path = optarg;
						if(access(licence_path,F_OK) != 0)
							return 255;
						if(access("/ac/config/.m",F_OK) != 0)
							return 255;
						prop_count = load_prop(SEP_EQUAL, "/ac/config/.m", properties);
						get_prop("BASE_MAC", base_mac, properties, prop_count);
						free_prop(properties, prop_count);
						sprintf(cmd,"/ac/sbin/crypt -q -K %s -f %s -o /tmp/license.tmp",base_mac,licence_path);	
						system(cmd);
						ret = system("cd /tmp;tar -xzf /tmp/license.tmp");
						system("rm -f /tmp/license.tmp");
						if(ret == -1)
							return 255;
						prop_count = load_prop(SEP_EQUAL, "/tmp/license.conf", properties);
						get_prop("LICENCE", tmp, properties, prop_count);
						free_prop(properties, prop_count);
						system("rm -f /tmp/license.conf");
						return atoi(tmp);
						break;
					case 'c'://Ð£Ñé°æ±¾
						ver_path = optarg;
						if(access(ver_path,F_OK) != 0)
							return 255;
						sprintf(cmd,"/ac/sbin/crypt -q -K guoren_ac -f %s -o /tmp/ac.img",ver_path);	
						system(cmd);	
						ret = system("cd /tmp;tar -xzf /tmp/ac.img");
						system("rm -f /tmp/*img");
						if(ret == 0)
							return 0;
						else
							return 255;
					 	break;
					default:
						return 255;
						break;
				}
		}
	return  255;
}




