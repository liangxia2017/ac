#ifndef _DBINTERFACE_H
#define _DBINTERFACE_H

#include "sqlite3.h"

#define MAX_STA_COUNT 128
#define MAC_LEN 20
#define STA_ONLINETIME_LEN 20
#define IP_LEN 20
#define MAX_PATH_LEN 60
#define MAX_NAME_LEN 30
#define MAX_DESC_LEN 60
#define MAX_LOCATION_LEN 30
#define MAX_PASSWORD_LEN 30
#define MAX_TIME_LEN 20
#define MAX_SSID_LEN 30
#define MAX_AP_GROUP 100
#define MAX_URL_LEN 100
#define MAX_KEY_LEN 30
#define MAX_DEVICE_MODE_LEN 100

#define SQLTM_COUNT       200 
#define SQLTM_TIME        50

typedef struct
{
	struct sqlite3         *rDBConn;
	struct sqlite3         *rwDBConn;
	pthread_mutex_t m_lock;
}DB_CONTENT_T;

typedef   struct   
{ 
    int   id;
    char mac[MAC_LEN];
    char curDate[STA_ONLINETIME_LEN];
} STAINFO_T; 
typedef STAINFO_T * PSTAINFO_T;

STAINFO_T gStaInfo[MAX_STA_COUNT];

typedef struct
{
	int ap_item_id;
	char mac[MAC_LEN];
	char ip[IP_LEN];
	int ap_group_id;
	char ap_name[MAX_NAME_LEN];
	char ap_desc[MAX_DESC_LEN];
	char location[MAX_LOCATION_LEN];
	char online_time[MAX_TIME_LEN];
	char start_time[MAX_TIME_LEN];
	int ap_status;
	int radio_count;
	int version_info_id;
}AP_ITEM_T;
typedef AP_ITEM_T *PAP_ITEM_T;

typedef struct
{
	int ap_item_id;
	char mac[MAC_LEN];
	char bssid[MAC_LEN];
	char ssid[MAX_SSID_LEN];
	int radio_id;
	int channel;
	int single;
	int snr;
	int rate;
	int ap_type;
}INVALID_AP_ITEM_T;
typedef INVALID_AP_ITEM_T *PINVALID_AP_ITEM_T;

typedef struct
{
	int ap_item_id;
	char mac[MAC_LEN];
	char online_time[MAX_TIME_LEN];
}NO_GROUP_AP_ITEM_T;
typedef INVALID_AP_ITEM_T *PNO_GROUP_AP_ITEM_T;


typedef struct
{
	int policy_item_id;
	int policy_group_id;
	char policy_name[MAX_NAME_LEN];
	int policy_type;
	int is_all_day;
	int start_hour;
	int start_min;
	int end_hour;
	int end_min;
	int week_index;
	int month_index;
	int day_index;
}POLICY_ITEM_T;
typedef POLICY_ITEM_T *PPOLICY_ITEM_T;



typedef struct
{
	int id;
	int wlan_item_id;
	int wlan_group_id;
	int wlan_security_policy_id;
	char ssid[MAX_SSID_LEN];
	int broadcast_ssid;
	int vlan;
	int qos;
	int max_user;
	int mac_filter_policy;
	int throughput_mode;
	int download_ssid;
	int download_user;
	int upload_ssid;
	int upload_user;
	int user_mode;
}WLAN_ITEM_T;
typedef WLAN_ITEM_T *PWLAN_ITEM_T;



typedef struct
{
	int wlan_security_policy_id;
	char name[MAX_NAME_LEN];
	int security_mode;
	int auth_mode;
	int key_len;
	int key_type;
	int encrypt_type;
	int default_key_index;
	char key1[MAX_KEY_LEN];
	char key2[MAX_KEY_LEN];
	char key3[MAX_KEY_LEN];
	char key4[MAX_KEY_LEN];
	char psk_key[MAX_KEY_LEN];
	
}WLAN_SECURITY_POLICY_T;
typedef WLAN_SECURITY_POLICY_T *PWLAN_SECURITY_POLICY_T;



typedef struct
{
	int id;
	char name[MAX_NAME_LEN];
	int radio_status;
	char desc[60];	
}AP_GROUP_INFO_T;
typedef AP_GROUP_INFO_T *PAP_GROUP_INFO_T;




typedef struct
{
	int id;
	int auth_type;
	int auth_level;
	char ip[IP_LEN];
	int port;
	char pwd[MAX_PASSWORD_LEN];
}RADIUS_CONFIG_T;
typedef RADIUS_CONFIG_T *PRADIUS_CONFIG_T;


typedef struct
{
	int id;
	int switch_flag;
	char ip[IP_LEN];	
}NTP_CONFIG_T;
typedef NTP_CONFIG_T *PNTP_CONFIG_T;


typedef struct
{
	int id;
	char ip[IP_LEN];
	int port;
	char server_cert_name[MAX_NAME_LEN];
	char ap_cert_name[MAX_NAME_LEN];
	char root_cert_name[MAX_NAME_LEN];
	char cert_pwd[MAX_PASSWORD_LEN];
}AS_CONFIG_T;
typedef AS_CONFIG_T *PAS_CONFIG_T;



typedef struct
{
	int id;
	char snmp_read_pw[MAX_PASSWORD_LEN];	
	char snmp_read_write_pw[MAX_PASSWORD_LEN];
	char trap_ip1[IP_LEN];
	char trap_ip2[IP_LEN];
	char trap_pw[MAX_PASSWORD_LEN];
	int snmp_read_write_port;
	int trap_port;
}SNMP_CONFIG_T;
typedef SNMP_CONFIG_T *PSNMP_CONFIG_T;

#define MAX_MANUFACTURER_LEN 50
#define MAX_SOFTWARE_VER_LEN 50
#define MAX_HARDWARE_vER_LEN 50

typedef struct
{
	int id;
	char hardware_version[MAX_HARDWARE_vER_LEN];	
	char manufacturer[MAX_MANUFACTURER_LEN];
	char device_model[MAX_DEVICE_MODE_LEN];
}VERSION_INFO_T;
typedef VERSION_INFO_T *PVERSION_INFO_T;



typedef struct
{
	int id;
	char start_ip[IP_LEN];	
	char end_ip[IP_LEN];
	char manage_ip[IP_LEN];
	char bak_manage_ip[IP_LEN];

}HOTBAK_CONFIG_T;
typedef HOTBAK_CONFIG_T *PHOTBAK_CONFIG_T;



typedef struct
{
	int id;
	int switch_flag;
	char ip1[IP_LEN];
	int syn_time;
	int flag;
}NTP_CLIENT_CONFIG_T;
typedef NTP_CLIENT_CONFIG_T *PNTP_CLIENT_CONFIG_T;


typedef struct
{
	int id;
	int switch_flag;

}NTP_SERVER_CONFIG_T;
typedef NTP_SERVER_CONFIG_T *PNTP_SERVER_CONFIG_T;


typedef struct
{
	int id;
	char dest_ip[IP_LEN];
	char netmask[IP_LEN];
	char next_hop[IP_LEN];
}ROUTE_CONFIG_T;
typedef ROUTE_CONFIG_T *PROUTE_CONFIG_T;


typedef struct
{
	int id;
	int level;
	char ip1[IP_LEN];
	char ip2[IP_LEN];
	int ip1_port;
	int ip2_port;
}SYSLOG_CONFIG_T;
typedef SYSLOG_CONFIG_T *PSYSLOG_CONFIG_T;



#define NET_NAME_LEN 20
typedef struct
{
	int id;
	char net_name[NET_NAME_LEN];
	char ip[IP_LEN];
	char netmask[IP_LEN];
	char gateway[IP_LEN];
}NET_INFO_T;
typedef NET_INFO_T *PNET_INFO_T;


typedef struct
{
	int id;
	char mac[MAC_LEN];
}MAC_FILTER_T;
typedef MAC_FILTER_T *PMAC_FILTER_T;



typedef struct
{
	int id;
	int radio_id;
	int switch_flag;
	int power_mode;
	int power;
	int mode;
	int rate;
	int n_rate;
	int mask;
	int channel_width;
	int guard_interval;
	int MPDU;
	int MSDU;
	int n_mode;
}RADIO_BASE_T;
typedef RADIO_BASE_T *PRADIO_BASE_T;


typedef struct
{
	int id;
	int isoliate;
	int igmp_snooping;
	int pre_auth;
	int up_link_check;
	int action;
	char action_ip[IP_LEN];
	char ntp_ip[IP_LEN];
	int syn_time;
}RADIO_ADVANCE_T;
typedef RADIO_ADVANCE_T *PRADIO_ADVANCE_T;



typedef struct
{
	int id;
	int radio_id;
	int chan;
	int switch_flag;
	int period;
}SCAN_CONFIG_T;
typedef SCAN_CONFIG_T *PSCAN_CONFIG_T;


typedef struct
{
	int id;
	int radio_id;
	int chan_auto;
	int chan_auto_mode;
	int chan_auto_time;
	int min_rssi;
}CHAN_CONFIG_T;
typedef CHAN_CONFIG_T *PCHAN_CONFIG_T;


typedef struct
{
	int id;
	int keep_alive;
	int broadcast_period;
}CAPWAP_TIMER_CONFIG_T;
typedef CAPWAP_TIMER_CONFIG_T *PCAPWAP_TIMER_CONFIG_T;


typedef struct
{
	int id;
	int switch_flag;
	int balance_type;
	int user_start_max;
	int user_diff;
	int user_max;
}LOAD_BALANCE_T;
typedef LOAD_BALANCE_T *PLOAD_BALANCE_T;



typedef struct
{
	int id;
	char ip[IP_LEN];
	int port;
	char username[MAX_NAME_LEN];
	char pwd[MAX_PASSWORD_LEN];
	char version_name[MAX_URL_LEN];
}DOWNLOAD_CONFIG_T;
typedef DOWNLOAD_CONFIG_T *PDOWNLOAD_CONFIG_T;


typedef struct
{
	char mac[MAC_LEN];
	char hardware_version[MAX_HARDWARE_vER_LEN];	
	char manufacturer[MAX_MANUFACTURER_LEN];
	char device_model[MAX_DEVICE_MODE_LEN];
}CHECK_INFO_T;
typedef CHECK_INFO_T *PCHECK_INFO_T;


typedef struct
{
	int id;
	char ap_mac[MAC_LEN];
	char bssid[MAC_LEN];
	char mac[MAC_LEN];
	char ssid[MAX_SSID_LEN];
	char start_time[MAX_TIME_LEN];
}STA_ITEM_T;
typedef STA_ITEM_T *PSTA_ITEM_T;



#endif


