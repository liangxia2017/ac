#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <webman.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/syslog.h>

#include <errno.h>

extern struct  product_info product_info_ac[];
extern struct html_link html_link_logo[];

#define BIT(x) (1<<(x))
//static int save_default_ssid(const char * ssid);
//static void set_default_config(struct product_info_txt_t *info_txt,int serid);
//static void set_def_ntp();
//static void def_trap_switch();

//add by liuyaxiong,20081123
#if 0
int addnewpar_to_wirelessconf(char *name,char *argnew)		
{
	prop_data properties[KEYS_MAX];
	int prop_count=0;
	char bufold[256];
	char *pt;
	
	bufold[0]=0;
	prop_count=load_prop(SEP_EQUAL, WIRELESS_CONF,properties);
	pt = get_prop(name, bufold, properties, prop_count);
	free_prop(properties,prop_count);
	if(!pt)
	{
		save_global_conf(SEP_EQUAL,WIRELESS_CONF,"",name,argnew);
	}
	return 1;
}
#endif

//add by liuyaxiong,20081117 
#if 0
int addnewpar_to_basicconf(char *name,char *argnew)		
{
	prop_data properties[KEYS_MAX];
	int prop_count=0;
	char bufold[256];
	char *pt;
	
	bufold[0]=0;
	prop_count=load_prop(SEP_EQUAL, BASIC_CONF,properties);
	pt = get_prop(name, bufold, properties, prop_count);
	free_prop(properties,prop_count);
	if(!pt)
	{
		save_global_conf(SEP_EQUAL,BASIC_CONF,"",name,argnew);
	}
	return 1;
}
#endif

//add by xieshaojun 20090201,网通上行链路完整性测试，默认开启
#if 0
void def_eth_integrity_for_test()
{
	if (0 != access(ADVANCED_CONF, R_OK|W_OK))
	{
		FILE * fid = fopen(ADVANCED_CONF, "w");
		if (fid) fclose(fid);
	}
	/*prop_data properties[KEYS_MAX];
	int prop_count=0;
	char ap_parameter[256];
	prop_count=load_prop(SEP_EQUAL,ADVANCED_CONF,properties);
	memset(ap_parameter, 0, sizeof(ap_parameter));
	get_prop("ETH_INTEGRITY", ap_parameter, properties, prop_count);
	if (strcmp(ap_parameter, "1"))
		save_global_conf(SEP_EQUAL,ADVANCED_CONF,"","ETH_INTEGRITY","1");
	free_prop(properties,prop_count);*/

//add by liuyaxiong,20090304
#if 1
    //除网通外,默认为关闭
	save_global_conf(SEP_EQUAL,ADVANCED_CONF,"","ETH_INTEGRITY","0");
#else
	save_global_conf(SEP_EQUAL,ADVANCED_CONF,"","ETH_INTEGRITY","1");
	/*启用专用管理SSID*/
	save_global_conf(SEP_EQUAL,BASIC_CONF,"","ENABLE_MGMT_SSID", "1");
#endif

}
#endif

/*
* Main function,
* prints request results into stdout
*/
int main(int arg, char **argv)
{
	prop_data properties[KEYS_MAX];
	int prop_count=0;
	stringlist *my_list=NULL;

	char file_name[LENGTH]={0};
	char m_version[LENGTH]={0};
	char s_version[LENGTH]={0};
	char firmwareversion[LENGTH]={0};
	char hardversion[LENGTH]={0};

	/* Output MIME header */

//add by liuyaxiong,20081108
#if 0
do{
	if(arg<2)
	break;
	if(strcmp(argv[1],"getapmodel")==0)
	{
		char buffer[2048]={0};
		int fd=-1;
		struct product_info_txt_t txt_info;
		char ap_outdoor[100];
		char ap_fact[8]={0};
		char ap_type[8]={0};
		int ap_fa=0;


		prop_count=load_prop(SEP_EQUAL,AP_SYSINFO_CONF,properties);
		get_prop("AP_MANUFACTURER", ap_fact, properties, prop_count);
		get_prop("AP_TYPE", ap_type, properties, prop_count);
		free_prop(properties,prop_count);
		ap_fa=atoi(ap_fact);
		handle_product_info_pwlan(PATH, ap_fa, &txt_info);
		
		handle_product_info_doortype(ap_outdoor,&txt_info);
#if 1	//add by xieshaojun 20090413
		create_empty_file(MAX_TXPOWER_CONF);
		save_global_conf(SEP_EQUAL, MAX_TXPOWER_CONF, "", "wifi0", txt_info.wifi0_maxpower);
		save_global_conf(SEP_EQUAL, MAX_TXPOWER_CONF, "", "wifi1", txt_info.wifi1_maxpower);
#endif

#if 1	//add by xieshaojun 20100308
		if (0 == strcmp(txt_info.wifi1_maxpower, "0"))
			save_global_conf(SEP_EQUAL,WIRELESS_CONF,"","AWLAN0_RF","0");
#endif
		buffer[0] = 0;
		//sprintf(buffer+strlen(buffer),"############\n");
		
		sprintf(buffer+strlen(buffer),"#产品型号\n");
		sprintf(buffer+strlen(buffer),"apmodel=\"%s\"\n",txt_info.p_ser);

		sprintf(buffer+strlen(buffer),"#硬件版本\n");
		sprintf(buffer+strlen(buffer),"aphardwareversion=\"%s\"\n",hardversion);
		//sprintf(buffer+strlen(buffer),"####AP固件版本\n");
		sprintf(buffer+strlen(buffer),"apfirmwareversion=\"%s\"\n",firmwareversion);

		//sprintf(buffer+strlen(buffer),"####AP室内外\n");
		sprintf(buffer+strlen(buffer),"apdoortype=\"%s\"\n",ap_outdoor);

		sprintf(buffer+strlen(buffer),"apManufacturer=\"%s\"\n",txt_info.manufacturer);
		sprintf(buffer+strlen(buffer),"deviceCategory=\"%s\"\n",txt_info.deviceCategory);


		if(arg<3)
		{
			printf( "%s", buffer);
		}
		else
		{
			if ((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR))==-1)
			{
					fprintf(stderr,"open file failure!");
					return -1;
			}
			
			if ((write(fd,buffer,strlen(buffer))==-1) && (errno!=EINTR))
			{
				fprintf(stderr,"write to file failure!");
				return -1;
			}
			close(fd);
		}
	
		return 0;
	}
//add by liuyaxiong,20081117 
#if 1
	else
	if(strcmp(argv[1],"getdefaultapmode")==0)
	{
 		struct product_info_txt_t txt_info;
 		char ap_fact[8]={0};
		char ap_type[8]={0};
		int ap_fa=0;
		char ap_mode[8]={0,};
		char ap_iptype[8]={0,};
		char ap_acurl[128]={0,};
		char serbuf[16]={0};
		int ap_mode_i;
		int ap_iptype_i;
		int serid;
		
		prop_count=load_prop(SEP_EQUAL,AP_SYSINFO_CONF,properties);
 		get_prop("AP_MANUFACTURER", ap_fact, properties, prop_count);
		get_prop("AP_TYPE", ap_type, properties, prop_count);
		get_prop("DF_AP_MODE", ap_mode, properties, prop_count);
		get_prop("DF_ETHER_IP_TYPE", ap_iptype, properties, prop_count);
		get_prop("DF_ACURL", ap_acurl, properties, prop_count);
		get_prop("SERID", serbuf, properties, prop_count);
		free_prop(properties,prop_count);
		
 		ap_fa=atoi(ap_fact);
		ap_mode_i=atoi(ap_mode);
		ap_iptype_i=atoi(ap_iptype);
		serid = atoi(serbuf);

		sprintf(ap_mode,"%d",ap_mode_i);
		sprintf(ap_iptype,"%d",ap_iptype_i);

		addnewpar_to_basicconf("AP_MODE",ap_mode);
		addnewpar_to_basicconf("ETHER_IP_TYPE",ap_iptype);
		addnewpar_to_basicconf("AC_IPADDR",ap_acurl);

//add by xieshaojun 20090214
#if 1
		handle_product_info_pwlan(PATH, ap_fa, &txt_info);
		set_default_config(&txt_info,serid);
		set_def_ntp();
#endif


//add by xieshaojun 20090702
#if 1
		def_trap_switch();
#endif
		return 0;
	}
#endif
//add by liuyaxiong,20090118
#if 1
	else
	if(strcmp(argv[1],"setsnmpinfo")==0)
	{
		char buffer[2048]={0};
		int fd=-1;
		struct product_info_txt_t txt_info;
		char ap_fact[8]={0};
		char ap_type[8]={0};
		int ap_fa=0;


		prop_count=load_prop(SEP_EQUAL,AP_SYSINFO_CONF,properties);
		get_prop("AP_MANUFACTURER", ap_fact, properties, prop_count);
		get_prop("AP_TYPE", ap_type, properties, prop_count);
		free_prop(properties,prop_count);
		ap_fa=atoi(ap_fact);
		handle_product_info_pwlan(PATH, ap_fa, &txt_info);
		

		buffer[0] = 0;
		//sprintf(buffer+strlen(buffer),"############\n");
		
		sprintf(buffer+strlen(buffer),"SNMPOID1=\"%d\"\n",txt_info.snmpoid1);
		sprintf(buffer+strlen(buffer),"SNMPOID2=\"%d\"\n",txt_info.snmpoid2);
		sprintf(buffer+strlen(buffer),"SNMPREGSTR=\"%s\"\n",txt_info.snmpregstr);


		if(arg<3)
		{
			printf( "%s", buffer);
		}
		else
		{
			if ((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR))==-1)
			{
					fprintf(stderr,"open file failure!");
					return -1;
			}
			
			if ((write(fd,buffer,strlen(buffer))==-1) && (errno!=EINTR))
			{
				fprintf(stderr,"write to file failure!");
				return -1;
			}
			close(fd);
		}
	
		return 0;
	}
#endif
	break;
}while(0);
#endif

	printf( "Content-type: text/html\n\n" );
	#if 0
	//handle_product_info(PATH, &product_info, &my_list);
	struct product_info_txt_t txt_info;
	char ap_fact[8]={0};
	char ap_type[8]={0};
	int ap_fa=0;
	prop_count=load_prop(SEP_EQUAL,AP_SYSINFO_CONF,properties);
	get_prop("AP_MANUFACTURER", ap_fact, properties, prop_count);
	get_prop("AP_TYPE", ap_type, properties, prop_count);
	free_prop(properties,prop_count);
	ap_fa=atoi(ap_fact);
	add_str(&my_list,"{AP_FACT}",ap_fact);
	add_str(&my_list,"{AP_TYPE}",ap_type);
	#if 0
	handle_product_info_pwlan(PATH, ap_fa, &txt_info);
	add_str(&my_list,"{AP_TITLE}",txt_info.title);
	add_str(&my_list,"{LINK0_FACT}",txt_info.link0);
	add_str(&my_list,"{LINK_FACT}",txt_info.link);
	add_str(&my_list,"{LINK_TXT_FACT}",txt_info.link_txt);

	add_str(&my_list,"{PRODUCT_SERIES_FACT}",txt_info.p_ser);
	add_str(&my_list,"{PRODUCT_NAME_FACT}",txt_info.p_name);
	add_str(&my_list,"{PRODUCT_TYPE_FACT}",txt_info.p_type);
	add_str(&my_list,"{PRODUCT_MODE_FACT}",txt_info.p_mode);
	#endif
	//读取AS服务器的配置文件
	prop_count=load_prop(SEP_EQUAL,AP_SYSINFO_CONF,properties);
	get_prop("AP_M_SOFTVER", m_version, properties,prop_count);
	get_prop("AP_S_SOFTVER", s_version, properties,prop_count);
	get_prop("HARDWARE_M_VER", hardversion, properties,prop_count);
	get_prop("FIRMWARE_VER", firmwareversion, properties,prop_count);
    

	free_prop(properties,prop_count);
	#endif
	sprintf(file_name, "%s/%s", HTMLS_PATH,ABOUT_PAGE);

//edit by xieshaojun 20090214
#if 1
{
	#if 0
	char apmsoftver[64];

	char dev_mac[10];
    char mac2[100];

	memset(dev_mac,0,10);
	/*Get eth1 MAC addresss*/
	get_dev_addr(dev_mac, "wifi0");
	//printf("ssss=%02X%02X%02X%02X",dev_mac[0],dev_mac[1],dev_mac[2],dev_mac[3]);
	//sprintf(mac2,"%s","112233445566");
	sprintf(mac2,"%02X%02X%02X%02X%02X%02X",dev_mac[0],dev_mac[1],dev_mac[2],dev_mac[3],dev_mac[4],dev_mac[5]);
	add_str(&my_list, "{SHOW_D_VERSION}", mac2);
	//printf("mac=%s\n",mac2);
	#if 0//add by xialiang,20100618
	strcpy(apmsoftver, m_version);
	if (get_serid() <= 499) // SERID_GRENTECH_END
		sprintf(apmsoftver + strlen(apmsoftver), "-%s", s_version);
	add_str(&my_list, "{SHOW_VERSION}", apmsoftver);
	add_str(&my_list, "{SHOW_H_VERSION}", hardversion);
	add_str(&my_list, "{SHOW_F_VERSION}", firmwareversion);
	#endif
	#endif
	char dev_mac[16] = {0};
	
	#if 1//add by xialiang,20100618
	char testsac_id[8];
	prop_count=load_prop(SEP_EQUAL, AC_SYSINFO_CONF,properties);
	get_prop("TESTSAC_ID",testsac_id , properties, prop_count);
	if(strlen(testsac_id) == 0)
		sprintf(testsac_id,"%s","101");
	int i;
	for(i=0;product_info_ac[i].testsac_id != 0;i++)
	{
		if(strcmp(testsac_id,product_info_ac[i].testsac_id) ==0)
		{
			add_str(&my_list,"{SHOW_MODEL}",product_info_ac[i].model_name);
			add_str(&my_list,"{SHOW_DESCRIP}",product_info_ac[i].description);
			break;
		}
	}

	short int tmpid;
	tmpid = atoi(testsac_id)/100 - 1;//国人产品1xx，其它厂商2xx，3xx
	if(tmpid < 0)
		tmpid = 0;
	add_str(&my_list,"{LINK_FACT}",html_link_logo[0].linkurl);
	add_str(&my_list,"{LINK_TXT_FACT}",html_link_logo[0].linktxt);

#if 0
	get_prop("HARDWARE_M_VER", hardversion, properties,prop_count);
	add_str(&my_list, "{SHOW_H_VERSION}", hardversion);
	get_prop("AP_M_SOFTVER", m_version, properties,prop_count);
	add_str(&my_list, "{SHOW_VERSION}", m_version);
	get_prop("AP_S_SOFTVER", s_version, properties,prop_count);
	add_str(&my_list, "{SHOW_SUB_VERSION}", s_version);
	get_prop("FIRMWARE_VER", firmwareversion, properties,prop_count);
	add_str(&my_list, "{SHOW_F_VERSION}", firmwareversion);
	get_prop("HARDWARE_SERIL", dev_mac, properties,prop_count);
	add_str(&my_list, "{SHOW_D_VERSION}", dev_mac);
#endif
	free_prop(properties,prop_count);
	
	#endif
    char run_time[LENGTH];
    get_running_time_string(run_time);
    add_str(&my_list, "{SHOW_RUN_TIME}", run_time);
}	
#endif	

	print_html(file_name,&my_list);
	free_list(&my_list);
	return 0;
}


//add by xieshaojun 20090214
#if 0
int save_default_ssid(const char * ssid)
{
	return 0;
}

//为其他厂家设置默认配置参数
//serid大于500时，判断文件not_def.conf是否存在；
//如果文件不存在，写入默认配置参数
void set_default_config(struct product_info_txt_t *info_txt,int serid)
{
	if (0 == access(DEFAULT_NOT_SET_CONF, F_OK))
		return;

	if (serid > 499) // SERID_GRENTECH_END
	{
		FILE * fid = fopen(DEFAULT_NOT_SET_CONF, "w");
		if (fid) fclose(fid);
	}
	else
	{
		return;
	}

	//设置默认主机名，SSID，IP地址，用户名，密码
	if (strlen(info_txt->default_host_name) > 0)
		save_global_conf(SEP_EQUAL,BASIC_CONF,"","HOST_NAME", info_txt->default_host_name);
	if (strlen(info_txt->default_ssid) > 0)
		save_default_ssid(info_txt->default_ssid);
	if (strlen(info_txt->default_ip) > 0)
		save_global_conf(SEP_EQUAL,BASIC_CONF,"","LAN_IP", info_txt->default_ip);
	if (strlen(info_txt->default_user) > 0)
		save_global_conf(SEP_EQUAL,AS_CONF,"","user", info_txt->default_user);
	if (strlen(info_txt->default_passwd) > 0)
		save_global_conf(SEP_EQUAL,AS_CONF,"","passwd", info_txt->default_passwd);

//add by xieshaojun 20090201
#if 1
	def_eth_integrity_for_test();
#endif

}
#if 0
//默认NTP同步间隔为24小时
void set_def_ntp()
{
	prop_data properties[KEYS_MAX];
	int prop_count=0;
	char ap_parameter[256];
	prop_count=load_prop(SEP_EQUAL,NTP_CONF,properties);
	if( get_prop("NTP_INTERVAL", ap_parameter, properties, prop_count)==NULL)	
		save_global_conf(SEP_EQUAL,NTP_CONF,"","NTP_INTERVAL","86400");
	free_prop(properties,prop_count);
}
#endif
#endif

/*xieshaojun,20090702,fit ap默认打开Trap开关(心跳除外)，
同时默认开启启动时调整信道*/
#if 0
void def_trap_switch()
{
	prop_data properties[KEYS_MAX];
	int prop_count=0;
	char ap_parameter[64];

	create_empty_file(SEND_TRAP_CONF);
	prop_count=load_prop(SEP_EQUAL,BASIC_CONF,properties);
	memset(ap_parameter, 0, sizeof(ap_parameter));
	get_prop("AP_MODE", ap_parameter, properties, prop_count);
	free_prop(properties,prop_count);
	if (0 == strcmp(ap_parameter, "1"))
	{
		prop_count=load_prop(SEP_EQUAL,SEND_TRAP_CONF,properties);
		memset(ap_parameter, 0, sizeof(ap_parameter));
		get_prop("USER_ASSOC", ap_parameter, properties, prop_count);
		free_prop(properties,prop_count);
		if (0 == strlen(ap_parameter))
		{
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","NTP_TRAP","1");	//?
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","INVALID_CERT_TRAP","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","CHALLENGE_REPLAY_TRAP","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","MIC_JUGGLE_TRAP","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","SECURITY_GRADE_DEBASE_TRAP","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","MAC_CHANGED_DIRECT_TRAP","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","STA_INTER","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","USER_ASSOC","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","USER_DEASSOC","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","ADD_STA_FAIL","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","NO_FREE_CHL","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","CHL_INTER","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","CHL_INTERAP","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","NOT_SAFE_AP","1");
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","DOS_ATTACK","1");
		}

		prop_count=load_prop(SEP_EQUAL,WIRELESS_CONF,properties);
		memset(ap_parameter, 0, sizeof(ap_parameter));
		get_prop("USE_AUTO_CHANNEL", ap_parameter, properties, prop_count);
		free_prop(properties,prop_count);
		if (0 == strlen(ap_parameter))
		{
			save_global_conf(SEP_EQUAL,WIRELESS_CONF,"","USE_AUTO_CHANNEL","1");
			save_global_conf(SEP_EQUAL,WIRELESS_CONF,"","AUTO_CHANNEL_MODE","0");
			save_global_conf(SEP_EQUAL,WIRELESS_CONF,"","AUTO_CHANNEL_INTERVAL","86400");
		}
	}
	else
	{
		prop_count=load_prop(SEP_EQUAL,SEND_TRAP_CONF,properties);
		memset(ap_parameter, 0, sizeof(ap_parameter));
		if( get_prop("NTP_TRAP", ap_parameter, properties, prop_count) == NULL)
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","NTP_TRAP","0");

		memset(ap_parameter, 0, sizeof(ap_parameter));
		if( get_prop("INVALID_CERT_TRAP", ap_parameter, properties, prop_count) == NULL)
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","INVALID_CERT_TRAP","0");

		memset(ap_parameter, 0, sizeof(ap_parameter));
		if( get_prop("CHALLENGE_REPLAY_TRAP", ap_parameter, properties, prop_count) == NULL)
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","CHALLENGE_REPLAY_TRAP","0");

		memset(ap_parameter, 0, sizeof(ap_parameter));
		if( get_prop("MIC_JUGGLE_TRAP", ap_parameter, properties, prop_count) == NULL)
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","MIC_JUGGLE_TRAP","0");

		memset(ap_parameter, 0, sizeof(ap_parameter));
		if( get_prop("SECURITY_GRADE_DEBASE_TRAP", ap_parameter, properties, prop_count) == NULL)
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","SECURITY_GRADE_DEBASE_TRAP","0");

		memset(ap_parameter, 0, sizeof(ap_parameter));
		if( get_prop("MAC_CHANGED_DIRECT_TRAP", ap_parameter, properties, prop_count) == NULL)
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","MAC_CHANGED_DIRECT_TRAP","0");

		memset(ap_parameter, 0, sizeof(ap_parameter));
		if( get_prop("STA_INTER", ap_parameter, properties, prop_count) == NULL)
			save_global_conf(SEP_EQUAL,SEND_TRAP_CONF,"","STA_INTER","0");

		memset(ap_parameter, 0, sizeof(ap_parameter));
		free_prop(properties,prop_count);
	}
}
#endif
