
#include <sqlite3.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "eloop.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <signal.h>

#define CAPWAP_PORT 5246
#define CAPWAP_MSG_SIZE 3072
#define NETLINK_SIZE 1024

enum
{
	join_request = 3,
	join_resp = 4,
	ap_event_request = 9,
	conf_update_resp =8,
	ap_event_resp = 10,
	echo_request = 13,
	echo_resp = 14,
	reset_resquest = 17,
	reset_resp = 18,
	station_conf_request = 25,
	station_conf_resp = 26	
};

struct capwap_header
{
	int 	header_flags:9,
		wireless_bind_id:5,
		radio_id:5,
		 header_len:5,
		 type:4,
		 version:4;

	
	short int fragment_id;
	
	short int  rev:3,
			frag_offset:13;
	
}__attribute__ ((packed));

struct runtime
{
	short int ac_state;
	struct capwap_header capwap_head;
	int recv_ac_sock;
};
struct sta_node
{
	unsigned char stamac[16];
	short int flag;/*0-上线，1-下线*/
	unsigned char radio_channel;
};

struct ap_node
{
	char *ap_model;
	char *company;
	char *hard_ver;
	unsigned char apmac[16];
	unsigned short int ap_id;
	unsigned short int keeplive;/*该ap的保活周期*/
	struct sockaddr_in *ap_from;
	struct sta_node sta_node;/*该ap上的用户信息*/
	unsigned short int rsp_buf_len;
	char *rsp_buf;/*id消息填充的buf*/
	unsigned short id_len;/*必要时，用于统计id长度*/
	unsigned short subid_len;/*必要时，用于统计subid长度*/
	unsigned int l2tp_id;
	unsigned short forward_flag;/*0-本地转发，1-集中转发*/
	char *soft_ver;
};
struct ap_report
{
	unsigned int eth_re;
	unsigned int eth_se;
	unsigned int wifi_re;
	unsigned int wifi_se;
	unsigned int lte_re;
	unsigned int lte_se;
};

void print_info(const char *fmt, ...);
int  capwap_init();
int get_db_value(void *arg, int col_num, char **col_value, char **col_name) ;
int db_join(void *arg, int col_num, char **col_value, char **col_name)  ;
int pack_wlan_id_security(void *arg, int col_num, char **col_value, char **col_name) ;
int pack_wlan_id(void *arg, int col_num, char **col_value, char **col_name)  ;
int pack_wireless_id(void *arg, int col_num, char **col_value, char **col_name)  ;
int pack_fuction_id(void *arg, int col_num, char **col_value, char **col_name)  ;
int assemble_echo_mask_id(int mask,struct ap_node *node);
int get_old_report_value(void *arg, int col_num, char **col_value, char **col_name) ;
int ap_down_hook(void *arg, int col_num, char **col_value, char **col_name) ;

extern int debug ;
extern int port ;
extern int event_sta_updown ;
extern struct runtime ac_runtime;

extern char trace[24] ;
char *errmsg;
 sqlite3 *pFile;

#ifndef MAC2STR
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#define MACSTR2 "%02x%02x%02x%02x%02x%02x"
#endif

