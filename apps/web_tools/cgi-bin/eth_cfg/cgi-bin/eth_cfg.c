#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <webman.h>
#include <string.h>
#include "config.h"



#define ETH_CONF "/opt/config/if_eth"
#define up_port 1
#define down_port 2

/*
* Main function,
* prints request results into stdout
*/
int main(int arg, char **argv)
{
	prop_data properties[KEYS_MAX];

	
	
	int prop_count=0;


	stringlist *my_list=NULL;

	char error_page[1024]={0};
	int ret = -1;
	
	/* Output MIME header */
	printf( "Content-type: text/html\n\n" );


	sprintf(error_page, "%s/%s", HTMLS_PATH, ERROR_PAGE);
	ret = http_timeout_update(TIMEOUT_FILE_PATH, getenv("REMOTE_ADDR"));
	
	if (ret == -1)
	{
		add_str(&my_list, "{ERROR_INFO}", "ÉÐÎ´µÇÂ¼£¬ÇëÖØÐÂµÇÂ¼");
		print_html(error_page, &my_list);
		free_list(&my_list);
		return 0;
	}
	else if (ret == 0)
	{
		add_str(&my_list, "{ERROR_INFO}","Á¬½Ó³¬Ê±£¬ÇëÖØÐÂµÇÂ¼");
		print_html(error_page, &my_list);
		free_list(&my_list);
		return 0;
	}

	/* save configuration */
	int i;
	
	prop_count=process_queue(properties);
	char tmp_value4[4] = {0};
	char tmp_value16[16] = {0};
	char tmp_value32[32] = {0};
	char tmp_path[32] = {0};
	char tmp_vlan_list[1024] = {0};

	
	if (prop_count > 0) 
	{
		for(i = 0; i<= 7; i++)
		{
			sprintf(tmp_value16,"CHANGE%d",i);
			get_prop(tmp_value16, tmp_value32, properties, prop_count);
			if(strcmp(tmp_value32,"0") == 0)
				continue;
			memset(tmp_path,0,sizeof(tmp_path));
			sprintf(tmp_path,ETH_CONF"%d.conf",i);
			save_global_conf(SEP_EQUAL,tmp_path,"","CHANGE_FLAG", tmp_value32);

			sprintf(tmp_value16,"IF_TYPE%d",i);
			get_prop(tmp_value16, tmp_value4, properties, prop_count);		
			if(i != 0 || i != 1)
			{
				save_global_conf(SEP_EQUAL,tmp_path,"","IF_TYPE", tmp_value4);
			}
			
			if(atoi(tmp_value4) == up_port || (atoi(tmp_value4) == down_port && i == 0))
			{
				sprintf(tmp_value16,"ip_addr%d",i);
				get_prop(tmp_value16, tmp_value32, properties, prop_count);
				save_global_conf(SEP_EQUAL,tmp_path,"","IP_ADDR", tmp_value32);


				sprintf(tmp_value16,"mask%d",i);
				get_prop(tmp_value16, tmp_value32, properties, prop_count);
				save_global_conf(SEP_EQUAL,tmp_path,"","MASK", tmp_value32);

				#if 0
				sprintf(tmp_value16,"gateway%d",i);
				get_prop(tmp_value16, tmp_value32, properties, prop_count);
				save_global_conf(SEP_EQUAL,tmp_path,"","GATEWAY", tmp_value32);
				#endif
			}
			
			memset(tmp_vlan_list,0,sizeof(tmp_vlan_list));
			sprintf(tmp_value16,"vlan%d",i);
			get_prop(tmp_value16, tmp_vlan_list, properties, prop_count);
			save_global_conf(SEP_EQUAL,tmp_path,"","ACCESS_VLAN", tmp_vlan_list);
		
		}

#if 0
		if(system("/usr/www/script/firewall_scr") != 0)
		{
			printf("firewall_scr error!");
			return -1;
		}
#endif
		
	}

	free_prop(properties,prop_count);
	// load configuration 
	for(i = 0; i <= 7; i++)
	{
		memset(tmp_path,0,sizeof(tmp_path));
		sprintf(tmp_path,ETH_CONF"%d.conf",i);
		prop_count=load_prop(SEP_EQUAL,tmp_path,properties);

		memset(tmp_value32,0,sizeof(tmp_value32));
		get_prop("IF_TYPE", tmp_value32, properties, prop_count);
		memset(tmp_value16,0,16);
		sprintf(tmp_value16,"{IF_TYPE%d}",i);
		if(strlen(tmp_value32) == 0)
		{
			add_str(&my_list,tmp_value16,"2");
		}
		else
		add_str(&my_list,tmp_value16, tmp_value32);

	

		memset(tmp_value32,0,sizeof(tmp_value32));
		get_prop("IP_ADDR", tmp_value32, properties, prop_count);
		memset(tmp_value16,0,16);
		sprintf(tmp_value16,"{ip_addr%d}",i);
		if(strlen(tmp_value32) == 0)
		{
			add_str(&my_list,tmp_value16,"");
		}
		else
			add_str(&my_list,tmp_value16, tmp_value32);

		memset(tmp_value32,0,sizeof(tmp_value32));
		get_prop("MASK", tmp_value32, properties, prop_count);
		memset(tmp_value16,0,16);
		sprintf(tmp_value16,"{mask%d}",i);
		if(strlen(tmp_value32) == 0)
		{
			add_str(&my_list,tmp_value16,"");
		}
		else
			add_str(&my_list,tmp_value16, tmp_value32);
		#if 0
		memset(tmp_value32,0,sizeof(tmp_value32));
		get_prop("GATEWAY", tmp_value32, properties, prop_count);
		memset(tmp_value16,0,16);
		sprintf(tmp_value16,"{gateway%d}",i);
		if(strlen(tmp_value32) == 0)
		{
			add_str(&my_list,tmp_value16,"");
		}
		else
			add_str(&my_list,tmp_value16, tmp_value32);
		#endif
		memset(tmp_vlan_list,0,sizeof(tmp_vlan_list));
		get_prop("ACCESS_VLAN", tmp_vlan_list, properties, prop_count);
		memset(tmp_value16,0,16);
		sprintf(tmp_value16,"{vlan%d}",i);
		if(strlen(tmp_vlan_list) == 0)
		{
			add_str(&my_list,tmp_value16,"");
		}
		else
			add_str(&my_list,tmp_value16, tmp_vlan_list);
		
		free_prop(properties,prop_count);
	}

	print_html(ETH_CFG_FILE,&my_list);
	free_list(&my_list);
	return 0;
}


