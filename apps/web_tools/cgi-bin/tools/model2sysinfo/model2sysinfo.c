#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <webman.h>

extern struct  product_info product_info_ac[];
extern struct html_link html_link_logo[];
int main()
{
	prop_data properties[KEYS_MAX] = {0};
	prop_data properties2[KEYS_MAX] = {0};
	int prop_count=0;
	int prop_count2=0;
	char testsac_id[8] = {0};
	char tmp_value[8] = {0};
	char domain[32] = {0};
	char *fp = NULL;
	char mac_string[32] = {0};
	char ap_serial[16] = {0};
	char *mac_fp;
	
	prop_count=load_prop(SEP_EQUAL, AC_SYSINFO_CONF,properties);
	get_prop("TESTSAC_ID",testsac_id , properties, prop_count);
	if(strlen(testsac_id) == 0)
		sprintf(testsac_id,"%s","101");
	free_prop(properties,prop_count);
	int i;
	for(i=0;product_info_ac[i].testsac_id != 0;i++)
	{
		if(strcmp(testsac_id,product_info_ac[i].testsac_id) ==0)
		{	
			save_global_conf(SEP_EQUAL, AC_SYSINFO_CONF, "", "COMPANY", product_info_ac[i].factory);
			save_global_conf(SEP_EQUAL, AC_SYSINFO_CONF, "", "ENTERPRIZE_ID", product_info_ac[i].enterprise_id);
			save_global_conf(SEP_EQUAL, AC_SYSINFO_CONF, "", "AC_MODEL", product_info_ac[i].model_name);
			save_global_conf(SEP_EQUAL, AC_SYSINFO_CONF, "", "DESCRIPTION", product_info_ac[i].description);

		}
	}
	#if 0//add by xialiang,20100629,增加设备序列号
	memset(properties,0,sizeof(properties));
	load_prop(SEP_EQUAL, TEMP_MAC_CONF,properties);
	get_prop("MACADDR",mac_string , properties, prop_count);
	mac_fp = mac_string;
	for(i = 0; i < 6 && mac_fp != NULL; i++)
	{
		strncat(ap_serial,mac_fp,2);
		mac_fp += 3;
	}
	save_global_conf(SEP_EQUAL, AC_SYSINFO_CONF, "", "HARDWARE_SERIL",ap_serial);
	free_prop(properties,prop_count);
	#endif
 	return 0;
}

