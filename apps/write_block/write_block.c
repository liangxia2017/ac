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

long long main(int argc, char *argv[])
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
	
	if(access("/ac/config/ac_sysinfo.conf",F_OK) != 0)
		system("touch /ac/config/ac_sysinfo.conf");
	prop_count = load_prop(SEP_EQUAL, "/ac/config/ac_sysinfo.conf", properties);
	
	while ((opt =  getopt(argc, argv, "m:a:f:v:d:s:")) != -1)
	{
			switch (opt) 
			{
				case 'a':
					memset(tmp,0,sizeof(tmp));
					strcpy(tmp,optarg);
					save_global_conf(SEP_EQUAL, "/ac/config/ac_sysinfo.conf", "", "AC_MODEL", tmp);
					break;
				case 'f':
					memset(tmp,0,sizeof(tmp));
					strcpy(tmp,optarg);
					save_global_conf(SEP_EQUAL, "/ac/config/ac_sysinfo.conf", "", "FACTORY", tmp);
					break;
				case 'v':
					memset(tmp,0,sizeof(tmp));
					strcpy(tmp,optarg);
					save_global_conf(SEP_EQUAL, "/ac/config/ac_sysinfo.conf", "", "SOFT_VER", tmp);
					break;	
				case 'd':
					memset(tmp,0,sizeof(tmp));
					strcpy(tmp,optarg);
					save_global_conf(SEP_EQUAL, "/ac/config/ac_sysinfo.conf", "", "DB_VER", tmp);
					break;
				case 's':
					memset(tmp,0,sizeof(tmp));
					strcpy(tmp,optarg);
					save_global_conf(SEP_EQUAL, "/ac/config/ac_sysinfo.conf", "", "SN", tmp);
					break;
				case 'm':
					memset(tmp,0,sizeof(tmp));
					strcpy(tmp,optarg);
					if(access("/ac/config/.m",F_OK) != 0)
						system("touch /ac/config/.m");
					prop_count = load_prop(SEP_EQUAL, "/ac/config/.m", properties);
					save_global_conf(SEP_EQUAL, "/ac/config/.m", "", "BASE_MAC", tmp);
					free_prop(properties, prop_count);
					break;
					
				default:

					break;
			}
	}
	free_prop(properties, prop_count);
	return  0;
}




