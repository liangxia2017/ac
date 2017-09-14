#ifndef MAIN_H
#define FILE_PATH  "/tmp"
#define LOG_NAME  "log.txt"
#define LOG_MAIN_NAME "main.c"
#define VERSION "CAPWAPS_CLIENT V1.00"
#define AC_CLEINT_PORT 1235
#define AC_SERVER_PORT 18603
#define CAPWAPS_IP "192.168.1.99"
#define CAPWAPD_IP "127.0.0.1"

#define MAX_APPS_CONNECTED_TO_AC 4

#define CMD_TYPE_SIZE	sizeof(unsigned char)
#define CONNECTION_OK 1   /*���ӳɹ���CAPWAPS �ᷢ��һ���ӳɹ���Ϣ*/
#define JOIN_CHECK 2   /*��AP�����CAPWAPS�ᷢ��AP ��MAC��ַ�����ݿ��ѯ*/
#define GLOB_CONF_MSG 3 /*�·�ȫ�ֵ����ã�add by lzc��20110225*/

#define JOIN_CHECK_RESULT 4   /*��AP�����MAC��ַ�����ݿ��еĲ��ҽ����add by lzc��20110228*/


//add by lzc,20110123
#if 1
#include "dbinterface.h"
typedef struct
{
	NTP_CLIENT_CONFIG_T ntp_client_config;
 	VERSION_INFO_T version_info;
 	RADIUS_CONFIG_T radius_config;
 	RADIUS_CONFIG_T radius_config_back;
 	SNMP_CONFIG_T snmp_config;
 	AS_CONFIG_T as_config;
} CWWTPInfoValuesOnAC;

#endif

int capwaps_client_flag = 0; /* ��CAPWAPS���ӳɹ��ı�־*/



#define GET_UPDATE_MESSAGE_FROM_WEB   2
#define MESSAGE_SIZE  1024*5
#define SEND_UPDATE_MESSAGE_TO_CAPWAPD   11
#define HEND_LEN   8
#endif
