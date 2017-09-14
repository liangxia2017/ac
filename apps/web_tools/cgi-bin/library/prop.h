/** @name CGI-BIN library of functions (prop.h) */
//@{

#ifndef _PROP_H
#define _PROP_H

#include "postdata.h"
/* Separators */
#define SEP_EQUAL '='
#define SEP_SPACE ' '
#define SEP_TAB (char)0x9
#define SEP_Q_MARK	'"'
#define ENABLE 1
#define DISABLE 2
#define WEBVERIFY 3

#define LENGTH 255
#define KEYS_MAX 255
#define MAC_STR_LEN 	20

#define SERVICE_ON 128
//#define FALSE 0
//#define TRUE 1

#define ABS(x) ((x)>=0? (x):(-x))

// Function declarations

/**
 Remove specified characters from the beginning and the end of a string.
 @param string  Source string.
 @param delimiter  Character to remove.
 @return Pointer to the new beginning of the string.
*/
char *strip (char *string, char delimiter);

/**
 Split a string into a key and value pair. String must be in one of the
 following formats:
 <key><separator><value> or setenv <key><separator><value>
 @param type  Type of character separating key and value in the string.
 Available types are: SEP_EQUAL - "=", SEP_SPACE - " ", SEP_TAB "\t".
 @param line  String.
 @param prop  Pointer to the structure with keys and values.
 @return 0 - if there was an error, 1 - otherwise.
*/
int get_pr (char type, char *line, prop_data *prop);

/**
 Split a string into a key and value pairs and save them in an array
 of structures. Source string can consist of substrings separated by
 newline characters. Each substring is searched for the key and value
 pair.
 @param type  Type of character separating key and value in the string.
 Available types are: SEP_EQUAL - "=", SEP_SPACE - " ", SEP_TAB "\t".
 @param str  String.
 @param properties  Array of structures with keys and values.
 @return Count of extracted key and value pairs.
*/
int load_from_str (char type, char *str, prop_data properties[]);

/**
 Split data read from a file into key and value pairs and save them in
 an array of structures. Each string read from the file is searched for
 key and value pair.
 @param type  Type of character separating key and value in the string.
 Available types are: SEP_EQUAL - "=", SEP_SPACE - " ", SEP_TAB "\t".
 @param file_name  String with file name.
 @param properties  Array of structures with keys and values.
 @return Count of extracted key and value pairs.
*/
int load_prop (char type, char *file_name, prop_data properties[]);

/**
 Enable, disable, verify service. Function reads lines from a file, searches
 for a line beginning with service name, depending on the action
 comments out that line, uncomments it or verifies if it is commented or
 not. Also starts or stops the service.
 @param type  Character separating different fields in the line.
 @param filename  String with file name.
 @param service  String with service name.
 @param action  ENABLE, DISABLE or WEBVERIFY.
 @return 0 - on error, 1 - otherwise, SERVICE_ON value - if action is
 WEBVERIFY and service is uncomented, i.e. it is on.
*/
int service_action (char type, char *filename, char *service, char action);

/**
 Save key and value pairs to file. The format is <key><separator>
 <value><newline>.
 @param type  Character separating different fields in the line.
 @param filename  String with file name.
 @param properties  Array of structures with keys and values.
 @param count  Count of array elements.
 @return 0 - on error, 1 - otherwise.
*/
int save_prop (char type, char *filename, prop_data properties[], int count);

/**
 Search for a value in an array of key and value structures.
 @param key  String with key.
 @param result  String with value if it is found, NULL - otherwise.
 @param properties  Array of structures with keys and values.
 @param count  Count of array elements.
 @return String with value if it is found, NULL - otherwise.
*/
char *get_prop (char *key, char *result, prop_data properties[], int count);

/**
 Search for value array in an array of key and value structures.
 @param key  String with key.
 @param result  String array with value if it is found.
 @param properties  Array of structures with keys and values.
 @param count  Count of array elements.
 @param max_size  max count of retun array elements.
 @return int with array size value if it is found, 0 - otherwise.
*/
int get_prop_arr(char *key, char (* result)[LENGTH], prop_data properties[], int count, int max_size);

/**
 Save a key and value pair to a file. Saves a line with structure:
 <command><key><separator><value>
 @param type  Type of character separating key and value. Available
 types are: SEP_EQUAL - "=", SEP_SPACE - " ", SEP_TAB "\t".
 @param filename  String with file name.
 @param command  String with command.
 @param key  String with key.
 @param value  String with value.
 @return 0 - on error, 1 - otherwise.
*/

int  calc_netpriexlength(char *netmas);//caculate net priex lenght (news add)
/*
 Search for a value in accordance with the key
 @param type  Type of character separating key and value. Available
 types are: SEP_EQUAL - "=", SEP_SPACE - " ", SEP_TAB "\t".
 @param filename  String with file name
 @param key  String with key.
 @param value  String with value
 @return 0 - on error, 1 - otherwise.
 */
int load_mtipaddress_conf(char type, char* filename, char *command, char *key, char *value);//load the ip  of mt(news add)
/**
 Save a key and value pair to a file. Saves a line with structure:
 <command><key><separator><value>
 @param type  Type of character separating key and value. Available
 types are: SEP_EQUAL - "=", SEP_SPACE - " ", SEP_TAB "\t".
 @param filename  String with file name.
 @param command  String with command.
 @param key  String with key.
 @param value  String with value.
 @return 0 - on error, 1 - otherwise.
*/

#if 0 /*edit by xieshaojun, org=1, Reason:not used*/
char *put_prop (char *key, char *result, prop_data properties[], int count);
/**
 *  Put result into properties
 *   Added by cchen
 *   */
#endif

int save_global_conf(char type, char* filename, char *command, char *key, const char *value);
/**
 Save a key and value pair to a file. Saves a line of new record
 @param type  Type of character separating key and value. Available
 types are: SEP_EQUAL - "=", SEP_SPACE - " ", SEP_TAB "\t".
 @param filename  String with file name.
 @param command  String with command.
 @param key  String with key.
 @param value  String with value.
 @return 0 - on error, 1 - otherwise.
*/

int save_sec_conf(char type, char* filename, char *command, char *key, char *value);//add a  news row in security_begin
/**
 Save a key and value pair to a file. Saves a line of new record
 @param type  Type of character separating key and value. Available
 types are: SEP_EQUAL - "=", SEP_SPACE - " ", SEP_TAB "\t".
 @param filename  String with file name.
 @param command  String with command.
 @param key  String with key.
 @param value  String with value.
 @return 0 - on error, 1 - otherwise.
*/

int save_spi_conf(char type, char* filename, char *command, char *key, char *value);
/* read a key and value pair to a file. Saves a line of record
 @param type  Type of character separating key and value. Available
 types are: SEP_EQUAL - "=", SEP_SPACE - " ", SEP_TAB "\t".
 @param filename  String with file name.
 @param command  String with command.
 @param key  String with key.
 @param value  String with value.
 @return 0 - on error, 1 - otherwise.
*/

int load_spishared_conf(char type, char* filename, char *command, char *key, char *value, int flag);
/* read a key and value pair to a file. Saves a line of  record
 @param type  Type of character separating key and value. Available
 types are: SEP_EQUAL - "=", SEP_SPACE - " ", SEP_TAB "\t".
 @param filename  String with file name.
 @param command  String with command.
 @param key  String with key.
 @param value  String with value.
 @return 0 - on error, 1 - otherwise.
*/

int load_spi_conf(char type, char* filename, char *command, char *key, char *value);
int load_mtip_conf(char type, char* filename, char *command, char *key, char *value, int flag);
int delete_spishared_conf(char type, char* filename, char *command, char *key, int flag);
/**
 Free memory used by an array of structures with keys and values.
 @param properties  String with key.
 @param result  String with value if it is found, NULL - otherwise.
 @param properties  Array of structures with keys and values.
 @param count  Count of array elements.
 @return Always returns 1.
*/
void free_prop (prop_data properties[], int count);

/**
 Copy a file.
 @param oldname  Source file name.
 @param newname  Destination file name.
 @return 0 - on error, 1 - otherwise.
*/
int file_copy (char *oldname, char *newname);

int hex2int(char c);

/**
 Send a signal to a process.
 @param proc_name  String with process name.
 @param signal  Signal to send to the process.
 @return 0 - on error, 1 - otherwise.
*/
int kill_proc (char *proc_name, int signal);

/**
 Get interface ip address
 @param ifname  Interface name
 @return string with ip address. On error condition returns empty string.
*/

char *getipaddr(char *ifname);

/**
 Get interface netmask
 @param ifname  Interface name
 @return string with netmask. On error condition returns empty string.
*/
char *ip_intoa(unsigned long int i_ip);
char *getnetmask(char *ifname);
int get_dev_addr(char *dev_mac, char *ifname);
int setioctl_ifaddr(char *ifaddr, char *ifname, int ioctl_type);
int setioctl_dev_ifflag(short ifflag, char *ifname, int ioctl_type);
int macstr2byte(char *macstr, unsigned char *des_byte, int byte_len);
int macstraddr(char *macstr, unsigned char *addr);
int str2byte( unsigned char *str, int len,  char *byte_out);

char * iw_pr_ether(char *buffer,  unsigned char *	ptr);
int  save_list_conf(char *file_name, const char *key, char *value);
int del_list_conf(char *file_name, const char *begin_flag, const char *end_flag, char *value);

int get_psk_type_and_key_len(const char * wep_key_type, char * psk_type, int * key_len);

//add by xsj 080818
#if 1
#if 0
#define TRAPTYPE_GROUP 0x09	//与<snmp>/zag500.c保持一致
#else
enum
{
	COMMTYPE_GROUP = 8,
	TRAPTYPE_GROUP,
	ETHERNET_GROUP
};
#define NETLINK_TRAP_ID     30   /*xieshaojun, for snmp,syslog,defined in linux kernel*/
#define NETLINK_FIT_AP      31   /*xieshaojun, for fitap,defined in linux kernel*/
#endif
extern int cgi_send_msgid_to_snmp_syslog(const char * data, const size_t data_size);
extern int get_serid();
extern void create_empty_file(const char * fname);
void get_logined_user(char * user_name);
#endif

#if  1
#define SNMPD_UPDATE_PORT			9004
#define ALARM_ID_IP_CHANGED (1)
#define SNMPD_UPDATE_CMD_SET_FILE_ERROR 2
#define SNMPD_UPDATE_CMD_SET_START 3
#define ALARM_ID_CHANNEL_CHANGED (4)
#define SNMPD_UPDATE_CMD_SET_UPDATE_OK 5
#define SNMPD_UPDATE_CMD_SET_REBOOT 6
#define NTP_SCR_ERROR 7
#define ALARM_ID_KEEP_ALIVE_TRAP (11)

#define COLD_START_TRAP_MSG 14
#define WARM_WARM_TRAP_MSG 15
#define RADIUS_AUTHSERVER_UNAVAILABLE 16 /*RADIUS认证服务器不可达*/
#define RADIUS_ACCTSERVER_UNAVAILABLE 17 /*RADIUS计费服务器不可达*/
#define FLASH_WRITE_FAIL 18
#define SYS_UPGRADE_FAIL 19
#define WAPI_ATTACK_INVALID_CERT 20 /*非法证书用户侵入网络通告*/
#define WAPI_ATTACK_CHALLENGE_REPLAY 21 /*客户端重放攻击通告*/
#define WAPI_ATTACK_MIC_JUGGLE 22 /*篡改攻击通告*/
#define WAPI_ATTACK_ADDR_REDIRECTION 23 /*地址重定向攻击通告*/
#define WAPI_ATTACK_LOW_SAFE_LEVEL 24 /*安全等级降低攻击通告*/
#define RADIUS_AUTHSERVER_CONNECTED 25 /*RADIUS认证服务器可达*/
#define RADIUS_ACCTSERVER_CONNECTED 26 /*RADIUS计费服务器可达*/

#if 1//升级相关告警号
#define ALARM_UPGRADE_FIRM_FALED (27)
#define ALARM_IMG_CHECK_FALED (28)
#define ALARM_UPGRADE_FIRM_SUCCESS (29)
#endif
#define ALARM_ADMIN_LOGINED (31)
#define CONFIG_FILE_CHANGED (32)
#define ALARM_LOGIN_FAILED  (33)
#define ALARM_CPU_USAGE_HIGHT	(40)//发现cpu 使用率高过阀值告警
#define ALARM_CPU_USAGE_HIGHT_RECOVER	(41)//发现cpu 使用率高过阀值告警解除
#define ALARM_MEM_USAGE_HIGHT	(42)//发现内存使用率高过阀值告警
#define ALARM_MEM_USAGE_HIGHT_RECOVER	(43)//发现内存使用率高过阀值告警解除
//#define ALARM_ID_COLD_START (6)//系统冷启动
//#define ALARM_ID_IP_CHANGED (7)//IP 地址变更
#define ALARM_APMTMODE_CHANGED (44)//AP无线监视工作模式变更通告
#define ALARM_APUPGRADE_ERROR (35)//AP软件升级失败通告这个需要修改
#define ALARM_AP_SSIDINTERF (46)//SSID密钥冲突通告
//#define ALARM_AP_HEART (47)//心跳周期通告
//#define ALARM_ID_CHANNEL_INTERFERENCE (12)//同频AP干扰告警
//#define ALARM_ID_CHANNEL_INTERFERENCERECOVER (13)//同频AP干扰告警清除
//#define ALARM_ID_CHANNEL_INTERFERENCE_AP (14) //邻频AP干扰告警
//#define ALARM_ID_CHANNEL_INTERFERENCERECOVER_AP (15) //邻频AP干扰告警解除
//#define ALARM_ID_CHANNEL_STAINTERFERENCE_AP (16)//终端干扰告警
//#define ALARM_ID_CHANNEL_STAINTERFERENCERECOVER_AP (17)//终端干扰告警解除
//#define ALARM_DEVICE_INTERFERENCE_DETECTED (48)//其他设备干扰告警
//#define ALARM_DEVICE_INTERFERENCERECOVER_DETECTED (49)//其他设备干扰告警解除
#define ALARM_MACLAYERDES_AP (50)//无线链路中断告警
#define ALARM_MACLAYERDESRECOVER_AP (51)//无线链路中断解除
//#define ALARM_ID_STA_ADD_FAIL (22)//AP无法增加新的移动用户告警
//#define ALARM_ID_STA_ADD_FAIL_RECOVER (52)//AP无法增加新的移动用户告警 解除
//#define ALARM_AP_CHANNEL_CHANGED	(24)//无线信道变更通告
//#define ALARM_STA_AUTH_FAIL (25)//终端鉴权失败通告
//#define ALARM_STA_ASSO_FAIL (26)//终端关联失败通告
//#define ALARM_ID_RADIUS_ACCTSERVER_AVAILABLE (56)//Radius计费服务器不可达/不可用/无法连接告警解除

//#define ALARM_MAC_CHANGED_DIRECTIONAL_ATTACK  (61)  /* 地址重定向攻击网管告警*/
#define ALARM_MACLAYERDES_AP2 (62)//无线链路中断告警
#define ALARM_MACLAYERDESRECOVER_AP2 (63)//无线链路中断解除
//#define ALARM_ID_AP_CO_INTERF_THRESHHD (64)//同频AP干扰告警
//#define ALARM_ID_AP_COINTERF_THRESHHDRECOVER (65)//同频AP干扰告警清除
//#define ALARM_ID_AP_NEIBORINTERF_THRESHHD (66) //邻频AP干扰告警
//#define ALARM_ID_AP_NEIBORINTERF_THRESHHDCOVER (67) //邻频AP干扰告警解除
//#define ALARM_ID_STA_INTERF_DETECTED_TRAP (68)//终端干扰告警
//#define ALARM_ID_STA_INTERF_DETECTED_CLEAR_TRAP (69)//终端干扰告警解除
#define ALARM_ID_SYS_REBOOT (99)
#define ALARM_ID_COLD_START (100)

int ap_snmpd_id(int ret);
#endif
struct syslogConfpara
{
	char server_path[128];
	char level[4];
	char type[8];
	char server_port[8];
} syslogConfpara;

extern const char * snmp_fac_names[];
extern const char *syslogAlamLevel[];
int load_syslog_table_item(struct syslogConfpara * item, const int index, char * file_name);
int
save_syslog_table(char *strFileName, char *value, const char *key_prefix, const int index);
int file_exist(const char * fname);
extern const char *wapi_cert_name[];
void exec_Restart_snmp();
//@}
#endif


