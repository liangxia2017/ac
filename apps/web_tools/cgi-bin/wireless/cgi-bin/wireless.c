#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include	<netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include "iwlib.h"
#include <webman.h>
#include <string.h>

#define DEFAUTL_DATARATE_11ag "54M auto"
#define DEFAUTL_DATARATE_11b "11M auto"

//int apply_rebuild_wlans();
//extern int cgi_send_msgid_to_snmp_syslog(const int msg_id, const char * data, const size_t data_size);


//add by liuyaxiong,20080520
#if 0 //only for test
struct	iw_point_int
{
	caddr_t	pointer;	/* Pointer to the data  (in user space) */
	unsigned short int length;		/* number of fields or size in bytes */
	unsigned short int flags;		/* Optional params */
};
struct	iwreq_int
{
	union
	{
		char	ifrn_name[IFNAMSIZ];	/* if name, e.g. "eth0" */
	} ifr_ifrn;

	/* Data part */
	union
	{
		char	name[IFNAMSIZ];	/* if name, e.g. "eth0" */
		unsigned int         mode;		/* Operation mode */
		struct iw_point_int data;		/* Other large parameters */
	}	u;
};

//#define	IEEE80211_IOCTL_GETPARAM_INT	(0x8BE0+1)//(SIOCIWFIRSTPRIV+1)
enum {	IEEE80211_PARAM_PC_TXPOWERDROP_INT = 90, // 得到设置同频功率调整时功率减少值，0.5db step
     };

//调用本函数，得到下调的功率值
//输入:无线网卡VAP名，如wlan0,awlan20等
//返回:<0 表示错误   >=0  下调的功率值，0.5db step
int get_txpowerdrop_cmd(char *ifname)
{
	int skfd;
	struct iwreq_int wrq;
	int *txdrop;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (skfd < 0)
	{
		//fprintf(stdout,"\ntxdrop socket open error\n");
		return(-1);
	}

	strncpy(wrq.ifr_ifrn.ifrn_name, ifname, IFNAMSIZ);
	wrq.u.mode = IEEE80211_PARAM_PC_TXPOWERDROP_INT;
	wrq.u.data.length = 0L;

	if (ioctl(skfd, IEEE80211_IOCTL_GETPARAM_INT, &wrq) < 0)
	{
		//fprintf(stdout,"\ntxdrop socket oprate error\n");
		close(skfd);
		return(-1);
	}

	close(skfd);

	txdrop = (int *)wrq.u.name;
	//fprintf(stdout,"\ntxdrop=%d\n", *txdrop);
	return(*txdrop);
}
#endif

#if 0
int return_id(char *filename)
{
	int ret_val = -1; //add xieshaojun@greentech 2008-04-15
	FILE* fidIn = NULL;
	char line[255];
	//char line1[255];
	char key[255];
	strcpy(key, "wifi1");

	if ((fidIn = fopen(filename, "r")) == NULL)
		return -1;

	ret_val = -1;

	while (!feof(fidIn))
	{
		fgets(line, 255, fidIn);

		if (feof(fidIn))
			break;

		if (strstr(line, key) != 0)
		{
			//return 0; //if return fclose() not called
			ret_val = 0;
			break;
		}
	}

	fclose(fidIn);
	return ret_val;
	//return -1;
}

//add xieshaojun@grentech 2008-04-15
//功能:在数组中查找数据率字符串
//返回值:0-没有找到;1-找到
char dataratestr_in_array(char (* arr)[16], int size, char * src)
{
	char ret_val = 0;
	int i = 0;

	for (i = 0; i < size; ++i)
	{
		if (0 == strcmp(arr[i], src))
		{
			ret_val = 1;
			break;
		}
	}

	return ret_val;
}

/*旧版本的配置文件中可能没有此项参数 ，给默认值
add xieshaojun 2008-05-14*/
static void set_def_datarate(char * mode, char *rate, char * arate)
{
	if (0 == strlen(rate))
		strcpy(rate, DEFAUTL_DATARATE_11ag);

	if ( 0 == strlen(arate))
		strcpy(arate, DEFAUTL_DATARATE_11ag);
}

static inline char *
iw_get_ifname(char *	name,	/* Where to store the name */
	      int	nsize,	/* Size of name buffer */
	      char *	buf)	/* Current position in buffer */
{
	char *	end;

	/* Skip leading spaces */
	while (isspace(*buf))
		buf++;

#ifndef IW_RESTRIC_ENUM
	/* Get name up to the last ':'. Aliases may contain ':' in them,
	 * but the last one should be the separator */
	end = strrchr(buf, ':');
#else
	/* Get name up to ": "
	 * Note : we compare to ": " to make sure to process aliased interfaces
	 * properly. Doesn't work on /proc/net/dev, because it doesn't guarantee
	 * a ' ' after the ':'*/
	end = strstr(buf, ": ");
#endif

	/* Not found ??? To big ??? */
	if ((end == NULL) || (((end - buf) + 1) > nsize))
		return(NULL);

	memcpy(name, buf, (end - buf));
	name[end - buf] = '\0';

	return(end + 2);
}

int get_channel(int skfd, char *ifname)
{
	struct iwreq		wrq;
	struct iw_range	range;
	double		freq;
	int			channel = 0;

	//char buffer[128]={0,};
	/* Get list of frequencies / channels */
	if (iw_get_range_info(skfd, ifname, &range) < 0)
	{
		//printf("%-8.8s  no frequency information.\n\n",  ifname);
		return 0;
	}

	if (iw_get_ext(skfd, ifname, SIOCGIWFREQ, &wrq) >= 0)
	{
		freq = iw_freq2float(&(wrq.u.freq));
		//iw_print_freq(buffer, freq); //error
		channel = iw_freq_to_channel(freq, &range);

		if (channel < 0)
		{
			channel = 0;
		}
	}

	return channel;
}

int  iw_enum_channel(int ifs, int * channel)
{
	char	buff[1024] = {0,};
	FILE *	fh;
	int		i = 0;
	int skfd = -1;

	if ((skfd = iw_sockets_open()) < 0)
	{
		return 0;
	}

//add by liuyaxiong,20080525
#if 1
//在/proc/net/wireless目录下, 按wlan0/awlan20......次序排列，所以，取头一个或头两个设备的信息即可
#endif

	fh = fopen(PROC_NET_WIRELESS, "r");

	if (fh != NULL)
	{
		/* Eat 2 lines of header */
		fgets(buff, sizeof(buff), fh);
		fgets(buff, sizeof(buff), fh);

		/* Read each device line */
		while (fgets(buff, sizeof(buff), fh) && (ifs > 0))
		{
			char	name[IFNAMSIZ + 1];
			char *s;
			/* Extract interface name */
			s = iw_get_ifname(name, sizeof(name), buff);

			if (!s)
			{
				channel[i] = 0;
//add by liuyaxiong,20080525
#if 1
				fclose(fh);
#endif
				goto exit;
			}
			else
			{
				/* Got it, print info about this interface */
				// (*fn)(skfd, name, args, count);
//add by liuyaxiong,20080525
#if 1
#else
				bzero(buff, 0);
#endif
				channel[i] = get_channel(skfd, name);
			}

			i++;
			ifs--;
		}

		fclose(fh);
	}

exit:
	close(skfd);
	return i;
}
//得到无线网卡数量
int iw_get_wifis()
{
	char		buff[1024];
	FILE *	fh;
	struct ifconf ifc;
	struct ifreq *ifr;
	int		i;
	char *q;
	int count = 0;
	char  name[IFNAMSIZ + 1];
	int skfd;

	if ((skfd = iw_sockets_open()) < 0)
	{
		perror("socket");
		exit(-1);
	}

	/* Check if /proc/net/wireless is available */
	fh = fopen(PROC_NET_DEV, "r");

	if (fh != NULL)
	{
		/* Success : use data from /proc/net/wireless */

		/* Eat 2 lines of header */
		fgets(buff, sizeof(buff), fh);
		fgets(buff, sizeof(buff), fh);

		/* Read each device line */
		while (fgets(buff, sizeof(buff), fh))
		{
			char *s;

			/* Extract interface name */
			s = iw_get_ifname(name, sizeof(name), buff);

			if (!s)
				/* Failed to parse, complain and continue */
				fprintf(stderr, "Cannot parse " PROC_NET_DEV "\n");
			else
			{
				q = strstr(name, "wifi");

				if (q != NULL)
				{
					count++;
				}
			}
		}

		fclose(fh);
	}
	else
	{
		/* Get list of configured devices using "traditional" way */
		ifc.ifc_len = sizeof(buff);
		ifc.ifc_buf = buff;

		if (ioctl(skfd, SIOCGIFCONF, &ifc) < 0)
		{
			//fprintf(stderr, "SIOCGIFCONF: %s\n", strerror(errno));
			close(skfd);
			return -1;
		}

		ifr = ifc.ifc_req;

		/* Print them */
		for (i = ifc.ifc_len / sizeof(struct ifreq); --i >= 0; ifr++)
		{
			q = strstr(name, "wifi");

			if (q != NULL)
			{
				count++;
			}
		}
	}

	close(skfd);
	return count;
}


/*
* Main function,
* prints request results into stdout
*/
#if 0
int change_mode(int mode, int  ret, char * buffer)
{
	if (mode == 1)
	{
		switch (ret)
		{
		case 0:
			strcat(buffer, "11B");
			break;
		case 1	:
			strcat(buffer, "11G");
			break;
		case 2	:
			strcat(buffer, "11NGHT20");
			break;
		case 3	:
			strcat(buffer, "11G");
			break;
		case 4:
			strcat(buffer, "11NGHT20");
			break;
		case 5	:
			strcat(buffer, "11A");
			break;
		case 6	:
			strcat(buffer, "11NAHT20");
			break;
		case 7	:
			strcat(buffer, "11NAHT20");
			break;
		default :
			strcat(buffer, "11NGHT20");
			break;
		}
	}
	else
	{
		if (strcmp(buffer, "11B") == 0)
			ret = 0;
		else if (strcmp(buffer, "11G") == 0)
			ret = 1;
		else if (strcmp(buffer, "11NGHT20") == 0)
			ret = 2;
		else if (strcmp(buffer, "11G") == 0)
			ret = 3;
		else if (strcmp(buffer, "11NGHT20") == 0)
			ret = 4;
		else if (strcmp(buffer, "11A") == 0)
			ret = 5;
		else if (strcmp(buffer, "11NAHT20") == 0)
			ret = 6;
		else if (strcmp(buffer, "11NAHT20") == 0)
			ret = 7;
		else
			ret = 2;

		return ret;
	}

	return 0;
}
#endif
#endif
int main(int arg, char **argv)
{
	prop_data properties[KEYS_MAX];
	int prop_count = 0;
	char wlan0_rts[32] = {0};
	char wlan0_frag[32] = {0};
	char wlan0_txpower[8] = {0};
	char wlan0_txpower_temp[8] = {0};
	char awlan0_txpower[8] = {0};
	char awlan0_txpower_temp[8] = {0};
	char wlan0_beacon[32] = {0};
	//	char wlan0_loadbalance[32]={0};
	char wlan0_channel[8] = {0};
	char wlan0_wmode[20] = {0};
	char wlan0_wmode_temp[20] = {0};
	char awlan0_wmode[20] = {0};
	char awlan0_wmode_temp[20] = {0};
	char wlan0_rf_temp[8] = {0};
	char awlan0_rf_temp[8] = {0};
	char wlan0_datarate[32] = {0};			//add xieshaojun@grentech 2008-04-15
	char wlan0_datarate_temp[32] = {0};	//add xieshaojun@grentech 2008-04-15
	char wlan0_datarate_temp_11n[32] = {0};		//add xieshaojun@grentech 2008-04-15
	char awlan0_datarate[32] = {0};		//add xieshaojun@grentech 2008-04-15
	char awlan0_datarate_temp[32] = {0};	//add xieshaojun@grentech 2008-04-15
	char n_channel_bandwidth_temp[32] = {0};
	//add by xieshaojun 2008-06-05
	char use_auto_channel[16] = {'0'};
	char auto_channel_interval[16] = {0};
	char pl_mode[16] = {'0'};
	char pl_si[8] = {'1'};
	char pl_ti[16] = {'0'};
	strcpy(pl_ti, "1000");//100 kbps

	char auto_txpower[8] = {'0'}; //0-关闭，1-开启
	char wlan0_auto_tx_same_ssid[8] = {'0'}; //只考虑相同SSID影响

	//char country_name[32] = {'0'};
	char n_space_rate[32] = {'0'};
	char n_channel_bandwidth[32] = {'0'};
	char n_work_rate[32] = {'0'};
	char n_mpdu[32] = {'0'};
	char n_msdu[32] = {'0'};
	char n_guard_inter[32] = {'0'};
	char dtim_period[32] = {'0'};

	char wlan0_channel1[8] = {0};
	char wlan0_rf1[8] = {0};
	char wlan0_rf4[8] = {0};
	char proc_dev[32] = "/proc/net/dev";

	int need_rebuild_wlan = 0;

#if 1 /*edit by liwei, 20100917*/
	char n_mpdu_temp[32] = {'0'};
#endif
	//edit by xieshaojnu 20080607
	static char * const aargv[] = {WIRELESS_SCRIPT, NULL};
	int pid;
	stringlist *my_list = NULL;

	unsigned char error_page[1024] = {0};
	int ret = -1;
#if 0
	//add xieshaojun@grentech 2008-04-15
	char valid_rate_11a[9][16] = {"54M auto", "6M fixed", "9M fixed",
				      "12M fixed", "18M fixed", "24M fixed", "36M fixed",
				      "48M fixed", "54M fixed"
				     };
	char valid_rate_11b[5][16] = {"11M auto", "1M fixed", "2M fixed",
				      "5.5M fixed", "11M fixed"
				     };
	char datarate_str_valid = 0; //0-not valid,1-valid
#endif

	strcpy(auto_channel_interval, "600");
	//strcpy(auto_channel_min_signal, "-70");

	/* Output MIME header */
//add by liuyaxiong,20081129 ???
#if 0
	//试图使IE进度条快速结束，但没有效果
	printf( "Content-type: text/html\n" );
	printf( "Connection: close\n\n" );
#else
	printf( "Content-type: text/html\n\n" );
#endif
#if 0//add by xialiang,20100521,去点页面不需要的信息
	struct product_info_txt_t txt_info;

	add_str(&my_list, "{AP_FACT}", "0");
	add_str(&my_list, "{AP_TYPE}", "1");
	handle_product_info_pwlan(PATH, 0, &txt_info);
	add_str(&my_list, "{AP_TITLE}", txt_info.title);
	add_str(&my_list, "{LINK0_FACT}", txt_info.link0);
	add_str(&my_list, "{LINK_FACT}", txt_info.link);
	add_str(&my_list, "{LINK_TXT_FACT}", txt_info.link_txt);

	add_str(&my_list, "{PRODUCT_SERIES_FACT}", txt_info.p_ser);
	add_str(&my_list, "{PRODUCT_NAME_FACT}", txt_info.p_name);
	add_str(&my_list, "{PRODUCT_TYPE_FACT}", txt_info.p_type);
	add_str(&my_list, "{PRODUCT_MODE_FACT}", txt_info.p_mode);
#endif
	sprintf(error_page, "%s/%s", HTMLS_PATH, ERROR_PAGE);
	ret = http_timeout_update(TIMEOUT_FILE_PATH, getenv("REMOTE_ADDR"));

	if (ret == -1)
	{
		//输出尚未登录页面
		add_str(&my_list, "{ERROR_INFO}", "尚未登录，请重新登录");
		print_html(error_page, &my_list);
		free_list(&my_list);
		return 0;
	}
	else if (ret == 0)
	{
		add_str(&my_list, "{ERROR_INFO}", "连接超时，请重新登录");
		print_html(error_page, &my_list);
		free_list(&my_list);
		return 0;
	}

	prop_count = load_prop(SEP_EQUAL, WIRELESS_CONF, properties);
	get_prop("WLAN0_TXPOWER", wlan0_txpower_temp, properties, prop_count);
	get_prop("AWLAN0_TXPOWER", awlan0_txpower_temp, properties, prop_count);
	get_prop("WLAN0_WMODE", wlan0_wmode_temp, properties, prop_count);
	get_prop("AWLAN0_WMODE", awlan0_wmode_temp, properties, prop_count);
	get_prop("WLAN0_RF", wlan0_rf_temp, properties, prop_count);
	get_prop("AWLAN0_RF", awlan0_rf_temp, properties, prop_count);
	get_prop("WLAN0_DATARATE", wlan0_datarate_temp, properties, prop_count);
	get_prop("AWLAN0_DATARATE", awlan0_datarate_temp, properties, prop_count);
	get_prop("WORK_RATE_11N", wlan0_datarate_temp_11n, properties, prop_count);
	
   	get_prop("CHANNEL_BANDWIDTH_11N", n_channel_bandwidth_temp, properties, prop_count);
#if 1 /*edit by liwei, 20100917*/
	get_prop("A_MPDU_11N", n_mpdu_temp, properties, prop_count);
#endif

	free_prop(properties, prop_count);
#if 0
	set_def_datarate(wlan0_wmode_temp, wlan0_datarate_temp, awlan0_datarate_temp);

	char old_bg_chl[4];
	char old_a_chl[8];

	int tableSize = iw_get_wifis();
	int channel[16] = {0,};
	iw_enum_channel(tableSize, channel);
	sprintf(old_bg_chl, "%d", channel[0]);
	sprintf(old_a_chl, "%d", channel[1]);
#endif
	/* Save configuration */
	prop_count = process_queue(properties);

	if (prop_count > 0)
	{
		char wlan0_rf[8] = {0};
		char awlan0_rf[8] = {0};

		get_prop("WLAN0_RTS", wlan0_rts, 	properties, prop_count);
		get_prop("WLAN0_FRAG", wlan0_frag, 	properties, prop_count);
		get_prop("WLAN0_BEACON", wlan0_beacon, 	properties, prop_count);

		get_prop("WLAN0_TXPOWER", wlan0_txpower, 	properties, prop_count);
		get_prop("AWLAN0_TXPOWER", awlan0_txpower, 	properties, prop_count);
		get_prop("WLAN0_WMODE", wlan0_wmode, properties, prop_count);
		get_prop("AWLAN0_WMODE", awlan0_wmode, properties, prop_count);
		get_prop("WLAN0_RF", wlan0_rf, properties, prop_count);
		get_prop("AWLAN0_RF", awlan0_rf, properties, prop_count);
		get_prop("WLAN0_DATARATE", wlan0_datarate, properties, prop_count);
		get_prop("AWLAN0_DATARATE", awlan0_datarate, properties, prop_count);


		get_prop("AUTO_FREQ_PERIOD", use_auto_channel, properties, prop_count);
		get_prop("AUTO_FREQ_SWITCH", auto_channel_interval, properties, prop_count);
		get_prop("PL_MODE", pl_mode, properties, prop_count);
		get_prop("PL_SI", pl_si, properties, prop_count);
		get_prop("PL_TI", pl_ti, properties, prop_count);


		//	set_def_datarate(wlan0_wmode, wlan0_datarate, awlan0_datarate);

		get_prop("AUTO_TXPOWER", auto_txpower, properties, prop_count);
		get_prop("WLAN0_AUTO_TX_SAME_SSID", wlan0_auto_tx_same_ssid, properties, prop_count);
		get_prop("WLAN0_CHANNEL", wlan0_channel, properties, prop_count);
		get_prop("AWLAN0_CHANNEL", wlan0_channel1, properties, prop_count);


		get_prop("N_SPACE_RATE", n_space_rate, properties, prop_count);
		//get_prop("COUNTRY_NAME", , properties, prop_count);
		get_prop("N_CHANNEL_BANDWIDTH", n_channel_bandwidth, properties, prop_count);
		get_prop("N_A_MPDU", n_mpdu, properties, prop_count);

		get_prop("N_A_MSDU", n_msdu, properties, prop_count);



		get_prop("N_GUARD_INTERVAL", n_guard_inter, properties, prop_count);
		get_prop("N_WORK_RATE", n_work_rate, properties, prop_count);
		get_prop("DTIM_PERIOD", dtim_period, properties, prop_count);

		free_prop(properties, prop_count);
#if 0
		//add xieshaojun@grentech 2008-04-15
		//判断数据是否有效
		datarate_str_valid = 0;

		if (3 == atoi(wlan0_wmode)) //11b/g
		{
			datarate_str_valid = dataratestr_in_array(valid_rate_11a, 9, wlan0_datarate);

			if (0 == datarate_str_valid)
				datarate_str_valid = dataratestr_in_array(valid_rate_11b, 5, wlan0_datarate);
		}

#if 1	//add by xieshaojun 20080928
		else if (33 == atoi(wlan0_wmode)) //11g
		{
			datarate_str_valid = dataratestr_in_array(valid_rate_11a, 9, wlan0_datarate);
		}

#endif
		else //11b
		{
			//printf("wlan0_datarate=%s",wlan0_datarate);
			datarate_str_valid = dataratestr_in_array(valid_rate_11b, 5, wlan0_datarate);
		}

		if ( (strlen(awlan0_datarate) > 0) && (1 == datarate_str_valid)) //11a
		{
			//printf("awlan0_datarate=%s",awlan0_datarate);
			datarate_str_valid = dataratestr_in_array(valid_rate_11a, 9, awlan0_datarate);
		}

#endif
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "WLAN0_RTS", wlan0_rts);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "WLAN0_FRAG", wlan0_frag);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "BEACON_INTV", wlan0_beacon);

		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "WLAN0_TXPOWER", wlan0_txpower);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "AWLAN0_TXPOWER", awlan0_txpower);

		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "WLAN0_WMODE", wlan0_wmode);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "AWLAN0_WMODE", awlan0_wmode);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "WLAN0_RF", wlan0_rf);

		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "AWLAN0_RF", awlan0_rf);

		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "WLAN0_CHANNEL", wlan0_channel);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "AWLAN0_CHANNEL", wlan0_channel1);


		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "WLAN0_DATARATE", wlan0_datarate);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "AWLAN0_DATARATE", awlan0_datarate);

		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "AUTO_FREQ_PERIOD", use_auto_channel);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "AUTO_FREQ_SWITCH", auto_channel_interval);
		//save_global_conf(SEP_EQUAL,WIRELESS_CONF,"","AUTO_CHANNEL_MIN_SIGNAL",auto_channel_min_signal);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "PL_MODE", pl_mode);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "PL_SI", pl_si);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "PL_TI", pl_ti);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "AUTO_TXPOWER", auto_txpower);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "WLAN0_AUTO_TX_SAME_SSID", wlan0_auto_tx_same_ssid);

		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "SPACE_RATE_11N", n_space_rate);


		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "CHANNEL_BANDWIDTH_11N", n_channel_bandwidth);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "A_MPDU_11N", n_mpdu);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "A_MSDU_11N", n_msdu);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "GUARD_INTERVAL_11N", n_guard_inter);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "WORK_RATE_11N", n_work_rate);
		save_global_conf(SEP_EQUAL, WIRELESS_CONF, "", "DTIM_PERIOD", dtim_period);

		if ((strcmp(wlan0_txpower, wlan0_txpower_temp) != 0)
				|| (strcmp(wlan0_wmode, wlan0_wmode_temp) != 0)
				|| (strcmp(awlan0_wmode, awlan0_wmode_temp) != 0)
				|| (strcmp(wlan0_rf, wlan0_rf_temp) != 0)
				|| (strcmp(awlan0_rf, awlan0_rf_temp) != 0)
				|| (strcmp(awlan0_txpower, awlan0_txpower_temp) != 0)

				//add xieshaojun@powercn.com 2008-04-17
				|| (strcmp(n_work_rate, wlan0_datarate_temp_11n) != 0)
				|| (strcmp(wlan0_datarate, wlan0_datarate_temp) != 0)
            || (strcmp(awlan0_datarate, awlan0_datarate_temp)!=0)
            || (strcmp(n_mpdu, n_mpdu_temp)!=0)
            || (strcmp(n_channel_bandwidth_temp, n_channel_bandwidth)))
		{
			need_rebuild_wlan = 1;
		}

#if 0	//xsj 20090602

		if (0 != strcmp(wlan0_rf_temp, wlan0_rf))
		{
			char ups[8], downs[8];
			prop_count = load_prop(SEP_EQUAL, WIFI_UPDOWN_STAT_FILE, properties);
			get_prop("WIFI0_UPTIME", ups, properties, prop_count);
			get_prop("WIFI0_DOWNTIME", downs, properties, prop_count);
			free_prop(properties, prop_count);
			sprintf(ups, "%d", atoi(ups) + 1);
			sprintf(downs, "%d", atoi(downs) + 1);

			if (0 == strcmp(wlan0_rf, "1"))
				save_global_conf(SEP_EQUAL, WIFI_UPDOWN_STAT_FILE, "", "WIFI0_UPTIME", ups);
			else
				save_global_conf(SEP_EQUAL, WIFI_UPDOWN_STAT_FILE, "", "WIFI0_DOWNTIME", downs);
		}

		if (0 != strcmp(awlan0_rf_temp, awlan0_rf))
		{
			char ups[8], downs[8];
			prop_count = load_prop(SEP_EQUAL, WIFI_UPDOWN_STAT_FILE, properties);
			get_prop("WIFI1_UPTIME", ups, properties, prop_count);
			get_prop("WIFI1_DOWNTIME", downs, properties, prop_count);
			free_prop(properties, prop_count);
			sprintf(ups, "%d", atoi(ups) + 1);
			sprintf(downs, "%d", atoi(downs) + 1);

			if (0 == strcmp(awlan0_rf, "1"))
				save_global_conf(SEP_EQUAL, WIFI_UPDOWN_STAT_FILE, "", "WIFI1_UPTIME", ups);
			else
				save_global_conf(SEP_EQUAL, WIFI_UPDOWN_STAT_FILE, "", "WIFI1_DOWNTIME", downs);
		}

#endif

#if 0	//add by xieshaojun 20081110

		if (strcmp(old_bg_chl, wlan0_channel))
		{
			struct channel_change_stut chl_change_data;
			memset(&chl_change_data, 0, sizeof(struct channel_change_stut));
			chl_change_data.msg_id = ALARM_ID_CHANNEL_CHANGED;
			chl_change_data.datalen = sizeof(chl_change_data);
			strcpy(chl_change_data.mode, "802.11b/g");
			strcpy(chl_change_data.old_chl, old_bg_chl);
			strcpy(chl_change_data.new_chl, wlan0_channel);
			cgi_send_msgid_to_snmp_syslog((char*)&chl_change_data, sizeof(struct channel_change_stut));
		}

		if (strcmp(old_a_chl, "0") && strcmp(old_a_chl, wlan0_channel1))
		{
			struct channel_change_stut chl_change_data;
			memset(&chl_change_data, 0, sizeof(struct channel_change_stut));
			chl_change_data.msg_id = ALARM_ID_CHANNEL_CHANGED;
			chl_change_data.datalen = sizeof(chl_change_data);
			strcpy(chl_change_data.mode, "802.11a");
			strcpy(chl_change_data.old_chl, old_a_chl);
			strcpy(chl_change_data.new_chl, wlan0_channel1);
			cgi_send_msgid_to_snmp_syslog((char*)&chl_change_data, sizeof(struct channel_change_stut));
		}

#endif

		if (need_rebuild_wlan == 1)
		{
			//apply_rebuild_wlans();
		}
		else
		{
			pid = fork();

			if (pid == 0)
			{
				//execv(aargv[0], aargv);
				_exit(0);
			}
			else //????,xieshaojun
			{
				int ret = 0;
				ret = wait(NULL);
			}

		}
	}

	/* Load configuration */
	prop_count = load_prop(SEP_EQUAL, WIRELESS_CONF, properties);
	get_prop("WLAN0_RTS", wlan0_rts, properties, prop_count);
	get_prop("WLAN0_FRAG", wlan0_frag, properties, prop_count);
	get_prop("WLAN0_TXPOWER", wlan0_txpower, properties, prop_count);
	get_prop("AWLAN0_TXPOWER", awlan0_txpower, properties, prop_count);
	get_prop("BEACON_INTV", wlan0_beacon, properties, prop_count);
	get_prop("WLAN0_CHANNEL", wlan0_channel, properties, prop_count);
	get_prop("AWLAN0_CHANNEL", wlan0_channel1, properties, prop_count);
	get_prop("WLAN0_WMODE", wlan0_wmode, properties, prop_count);
	get_prop("AWLAN0_WMODE", awlan0_wmode, properties, prop_count);
	get_prop("WLAN0_DATARATE", wlan0_datarate, properties, prop_count);
	get_prop("AWLAN0_DATARATE", awlan0_datarate, properties, prop_count);
	get_prop("AWLAN0_RF", wlan0_rf1, properties, prop_count);
	get_prop("WLAN0_RF", wlan0_rf4, properties, prop_count);

	get_prop("AUTO_FREQ_SWITCH", use_auto_channel, properties, prop_count);

	get_prop("AUTO_FREQ_PERIOD", auto_channel_interval, properties, prop_count);

	get_prop("PL_MODE", pl_mode, properties, prop_count);


	get_prop("PL_SI", pl_si, properties, prop_count);

	get_prop("PL_TI", pl_ti, properties, prop_count);

	get_prop("AUTO_TXPOWER", auto_txpower, properties, prop_count);

	get_prop("WLAN0_AUTO_TX_SAME_SSID", wlan0_auto_tx_same_ssid, properties, prop_count);

	//get_prop("COUNTRY_NAME", country_name, properties, prop_count);
	get_prop("SPACE_RATE_11N", n_space_rate, properties, prop_count);
	get_prop("CHANNEL_BANDWIDTH_11N", n_channel_bandwidth, properties, prop_count);
	get_prop("A_MPDU_11N", n_mpdu, properties, prop_count);
	get_prop("A_MSDU_11N", n_msdu, properties, prop_count);
	get_prop("GUARD_INTERVAL_11N", n_guard_inter, properties, prop_count);
	get_prop("WORK_RATE_11N", n_work_rate, properties, prop_count);
	get_prop("DTIM_PERIOD", dtim_period, properties, prop_count);

	free_prop(properties, prop_count);

	//set_def_datarate(wlan0_wmode, wlan0_datarate, awlan0_datarate);

	add_str(&my_list, "{WLAN0_RTS}", wlan0_rts);
	add_str(&my_list, "{WLAN0_FRAG}", wlan0_frag);
	add_str(&my_list, "{WLAN0_BEACON}", wlan0_beacon);
	add_str(&my_list, "{WLAN0_TXPOWER}", wlan0_txpower);
	add_str(&my_list, "{AWLAN0_TXPOWER}", awlan0_txpower);

	//		add_str(&my_list,"{WLAN0_LOADBALANCE}",wlan0_loadbalance);
	add_str(&my_list, "{WLAN0_CHANNEL}", wlan0_channel);
	add_str(&my_list, "{AWLAN0_CHANNEL}", wlan0_channel1);

	add_str(&my_list, "{WLAN0_WMODE}", wlan0_wmode);
	add_str(&my_list, "{AWLAN0_WMODE}", awlan0_wmode);
	add_str(&my_list, "{WLAN0_DATARATE}", wlan0_datarate);
	add_str(&my_list, "{AWLAN0_DATARATE}", awlan0_datarate);
	add_str(&my_list, "{CHANGED_INFO}", "");
	add_str(&my_list, "{AWLAN0_RF}", wlan0_rf1);
	add_str(&my_list, "{WLAN0_RF}", wlan0_rf4);

	add_str(&my_list, "{AUTO_FREQ_SWITCH}", use_auto_channel);

	add_str(&my_list, "{AUTO_FREQ_PERIOD}", auto_channel_interval);

	add_str(&my_list, "{PL_MODE}", pl_mode);
	add_str(&my_list, "{PL_SI}", pl_si);
	add_str(&my_list, "{PL_TI}", pl_ti);
	add_str(&my_list, "{AUTO_TXPOWER}", auto_txpower);
	#if 0//刘亚雄做的，相同ssid才做功率调整
	add_str(&my_list, "{WLAN0_AUTO_TX_SAME_SSID}", wlan0_auto_tx_same_ssid);
	char buf[128] = {0};
	sprintf(buf, "%3.1fdBm", get_txpowerdrop_cmd("wlan0") / (2.0));
	add_str(&my_list, "{AUTO_TXPOWER_SUB_VALUE}", buf);
	#endif
	add_str(&my_list, "{SPACE_RATE_11N}", n_space_rate);
	//add_str(&my_list,"{COUNTRY_NAME}",country_name);
	add_str(&my_list, "{CHANNEL_BANDWIDTH_11N}", n_channel_bandwidth);

	add_str(&my_list, "{A_MPDU_11N}", n_mpdu);
	add_str(&my_list, "{A_MSDU_11N}", n_msdu);
	add_str(&my_list, "{GUARD_INTERVAL_11N}", n_guard_inter);
	add_str(&my_list, "{WORK_RATE_11N}", n_work_rate);
	add_str(&my_list, "{DTIM_PERIOD_IN}", dtim_period);
#if 0
	if (return_id(proc_dev) == 0)
		print_html(WIRELESS_FILE2, &my_list);
	else
		print_html(WIRELESS_FILE, &my_list);
#endif
	print_html(WIRELESS_FILE2, &my_list);
	free_list(&my_list);
	return 0;
}

#if 0	//add by xieshaojun 20081006
int apply_rebuild_wlans()
{
	int  pid = -1;
	char * aargv[] = {REBUILD_WLAN_SCRIPT, NULL};

	pid = fork();

	if (pid < 0)
		return -1;
	else if (0 == pid)
	{
		//setsid();
		//sleep(1);
		//execv(aargv[0],aargv);
		//exit(0);
		execv(aargv[0], aargv);
		_exit(0);
	}
	else
	{
		int ret = 0;
		ret = wait(NULL);
	}

	return 0;
}
#endif

