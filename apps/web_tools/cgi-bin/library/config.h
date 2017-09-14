#ifndef __CONFIG_H__
#define __CONFIG_H__
//#include <linux/wireless.h>
//#include <linux/netdevice.h>
//#include "www_ath_bcbuf.h"
#define	IEEE80211_ADDR_LEN	6		/* size of 802.11 address */
#define PRODUCT_ID "ap7161"

#define HTMLS_PATH                  "/opt/ac/www/"
#define LOGIN_SHOW_FIRST_CGI         "about.cgi"
#define AS_SUCC_FILE                 "as_succ.html"
#define AS_FAIL_FILE                  "as_fail.html"
#define ERROR_PAGE                  "error.html"
#define AS_QUERY_FILE               "as_cert_query.html"
#define CERT_QUERY_FILE             "as_cert_list.html"
#define AS_STATUS_INFO_FILE         "as_status_info.html"
#define AS_FILE                     "as.html"
#define BACKUP_DATA_PAGE            "backup_data.html"
#define DEF_FILE                    "def.html"
#define LOGIN_SECURE_FILE           "login_secure.html"
#define MENU_FILE                   "menu.html"
#define MENU_FILE_0                 "menu_0.html"
#define MENU_FILE_1                 "menu_1.html"
#define PASSWD_FILE                 "passwd.html"
#define INDEX_FILE                  "index.html"
#define RESET_FILE                  "reset.html"
#define RESUME_DATA_PAGE            "resume_data.html"
#define REBOOT_PAGE                 "reboot.html"
#define REBOOT1_PAGE                "reboot1.html"
#define FIRMWARE_UPGRADE_PAGE       "firmware_upgrade.html"
#define UPDATE_TIME_PAGE            "update_time.html"
#define ABOUT_PAGE                  "about.html"

#define TITLE_FILE                      HTMLS_PATH"title.html"
#define UPFILE_FILE                     HTMLS_PATH"upfile.html"
#define SWITCH_ID_FILE                  HTMLS_PATH"switch_id.html"
#define SWITCH_REBOOT_FILE              HTMLS_PATH"switch_reboot.html"
#define DHCLIENT_FILE                   HTMLS_PATH"dhclient.html"
#define WDS_FILE                        HTMLS_PATH"wds.html"
#define AWDS_FILE                       HTMLS_PATH"wds11a.html"
#define UNAUTH_FILE                     HTMLS_PATH"unauth.html"
#define WAPI_FILE                       HTMLS_PATH"wapi.html"
#define WIRELESS_FILE                   HTMLS_PATH"wireless.html"
#define TIME_REBOOT_FILE                HTMLS_PATH"time_reboot.html"
#define WIRELESS_FILE2                  HTMLS_PATH"wireless2.html"
#define DHCPD_FILE                      HTMLS_PATH"dhcpd.html"
#define MACFILTER_FILE                  HTMLS_PATH"macfilter.html"
#define QOS_FILE                        HTMLS_PATH"qos_tc.html"
#define WLAN_FILE                       HTMLS_PATH"wlan.html"
#define BASIC_AP_FILE                   HTMLS_PATH"basic_ap.html"
#define MANAGEMENT_CONTROL_FILE         HTMLS_PATH"management_control.html"
#define REBOOT_FILE                     HTMLS_PATH"reboot.html"
#define APP_CERT_FILE                   HTMLS_PATH"app_cert.html"
#define SNMP_FILE                       HTMLS_PATH"snmp.html"
#define ACL_DHCP_FILE                   HTMLS_PATH"dhcp_server.html"
#define NAT_FILE                        HTMLS_PATH"nat.html"
#define DEFCON_FILE                     HTMLS_PATH"defcon.html"
#define EAP_RADIUS_FILE                 HTMLS_PATH"radius.html"
#define TIME_ZONE_PAGE                  "time_zone.html"
#define ETH_CFG_FILE				HTMLS_PATH"eth_cfg.html"//add by xialiang,20110212

#define SYSLOG_HTML_FILE_FIRST          HTMLS_PATH"syslog_1.html"
#define SYSLOG_HTML_FILE_LAST           HTMLS_PATH"syslog_2.html"
#define STA_LIST_FILE                   HTMLS_PATH"station_list.html"
#define TEST_MAC_FILE                   HTMLS_PATH"testmac.html"

#define CP_TMP_DIR  "/tmp/"
#define PATH                            "/opt/config/" 
#define THIN_AUTH_PATH                  PATH"auth/"
#define SYSLOG_FILE_PATH                PATH"log/"           /*add by xieshaojun, 20100507*/
#define NETWORK_CONF                    PATH"network.conf"     /*edit by liwei, 20100402*/
#define BASIC_CONF                      NETWORK_CONF
#define RADIUS_CONF                     PATH"radius.conf"
#define AS_CONF                         PATH"AS.conf"
#define WIRELESS_CONF                   PATH"wireless.conf"
#define DHCPD_CONF                      PATH"udhcpd.conf"
#define WLAN_MAX_TRAFFIC_CONFIG         PATH"wlan_maxtraffic.conf"
#define WLAN_MAX_TF_UP_CONFIG           PATH"wlan_maxtf_up.conf"
#define PRE_AUTH_CONFIG                 PATH"pre_auth.conf"
#define MACFILTER_CONF                  PATH"macfilter.conf"
#define WDS_CONFIG                      PATH"wds.conf"
#define WEP_KEYS_CONF                   PATH"wep_keys.conf"
#define NTP_CONF                        PATH"ntp.conf"
#define AC_SYSINFO_CONF                 "/tmp/ac_sysinfo.conf"//edit by xialiang,20110127
#define TEMP_MAC_CONF				CP_TMP_DIR"mac.conf"
#define CHANNEL_CHANGES_FILE			"/tmp/freq_changes"
#define ADVANCED_CONF                   PATH"advanced.conf"
#define FIREWALL_CONF                   ADVANCED_CONF
#define TRAP_SWITCH_CONF                 PATH"trap_switch.conf"   /*add by liwei, 201005013*/
#define SEND_TRAP_CONF                  TRAP_SWITCH_CONF
#define CPU_MEM_VALUE_CONF		CP_TMP_DIR"cpu_mem_value.conf"

#define REBOOT_HOUR_CONF                PATH"timer_reboot.conf"
#define SYSLOG_TABLE_CONF               PATH"syslog_table.conf"
//#define DEFAULT_NOT_SET_CONF            PATH"not_def.conf"
#define PASSWD_CONF                     PATH"wwwuser.conf"    /*edit by liwei, 20100402*/
#define USER_CONF                       PASSWD_CONF
#define AWDS_CONF                       PATH"awds.conf"
#define VS_CONF                         PATH"virture_server.conf"

#define SNMP_PARA_CONF                       PATH"snmp.conf"
#define SNMPD_CONF                       "/tmp/snmp.conf"
#define WMM_CONF 			PATH"wmm.conf"/*wmm配置文件*/
//#define SNMP_AGENT_CONF                 "/etc/IWNCOMM/snmp/snmp_agent.conf"
#define WAPI_ASU_CONF                   PATH"asu.conf"
#define WAPI_CONF                   "/tmp/wapi.conf"


#define MANAGE_BR_CONF                  "/tmp/manage_br"
#define PARA_PARSE_CONF                 "/tmp/para_parse.conf"
#define TIMEOUT_FILE_PATH               "/root/"
#define MAX_TXPOWER_CONF                "/tmp/max_txpower"
#define WIFI_UPDOWN_STAT_FILE           "/tmp/wifi_updown_stat"
#define RF_ACTION_CONF                   "/tmp/rf_action.conf"
#define TEMP_DATA_TUNNEL                   "/tmp/data_tunnel"
#define JAC_CONF                        "/tmp/jac.conf"
#define GRE_CONF                        "/tmp/gre.conf"
#define GRE_CONF_BACKUP                 "/tmp/gre_backup.conf"
#define TEMP_USLD_CFG                   "/tmp/usld.conf" //add by luoqiwen 2009-03-17
#define TEMP_STA_ADDED_CONF             "/tmp/sta_added_info.conf"//added by chuyu 090319
#define TEMP_STA_DELETE_CONF            "/tmp/sta_delete_info.conf"
#define TEMP_STA_QOSP_CONF              "/tmp/sta_qosp_info.conf"
#define TEMP_STA_VLAN_CONF              "/tmp/sta_vlan_info.conf"
#define TEMP_STA_STASID_CONF            "/tmp/sta_stasid_info.conf"
#define TEMP_STA_PMK_CONF               "/tmp/sta_pmk_info.conf"
#define BSSID_DISTURB_LIST              "/tmp/bssid_disturb_list.conf"
#define RF_ACTION_CONF                  "/tmp/rf_action.conf"
#define REMOTE_UPGRADE_CONF        "/tmp/remupg.conf"
#define REMOTE_CONF 				 "/tmp/remote_conf.conf"

#define TEMP_ASCERT	                  "/tmp/fitap_as_cert"
#define TEMP_USERCERT               "/tmp/fitap_user_cert"
#define TEMP_TRUST_CERT	            "/tmp/fitap_trust_cert" /*add by xieshaojun,20100601*/
#define TEMP_P12_PASSWD           "/tmp/fitap_p12_psw" /*add by xieshaojun,20100601*/
#define PARSE_AP_VER            "/tmp/parse_ip_ver" /*add by xieshaojun,20100610*/

#define SYSLOG_FILE_FIRST                "/var/run/syslog1.log"
#define SYSLOG_FILE_SECOND                "/var/run/syslog2.log"


#define MAC_FILE "/etc/ap11n/default/mac.conf"

#if 1//add by xialiang,20101218
#define AP_MODEL_FILE "/etc/ap11n/default/ap_model.conf"
#endif

#define SCRIPTS_PATH               "/usr/www/script/"
#define AS_SCRIPT                   SCRIPTS_PATH"as_scr"
#define BRIDGE_WAPI_SCRIPT          SCRIPTS_PATH"wlanbridge_scr"
#define LED_SCRIPT                  SCRIPTS_PATH"led_scr"
#define REBOOT_SCRIPT               SCRIPTS_PATH"reboot_scr"
#define WIRELESS_SCRIPT             SCRIPTS_PATH"wireless_scr"
#define APPLY_BASIC_SCRIPT          SCRIPTS_PATH"apply_basic_scr"
#define DEFCON_SCRIPT               SCRIPTS_PATH"defcon_scr"
#define APPLY_DHCPD_SCRIPT          SCRIPTS_PATH"dhcpd_scr"
#define WLAN_SCRIPT                 SCRIPTS_PATH"wlan_scr"
#define SYSTEM_REBOOT               SCRIPTS_PATH"reboot_scr"
#define WDS_SCRIPT                  SCRIPTS_PATH"wds_scr"
#define CONFIG_MACFITER_SCRIPT      SCRIPTS_PATH"config_macfilter_scr"
#define MAC_FILTER_SET_SCRIPT       SCRIPTS_PATH"mac_filter_set"
#define MAC_FILTER_DEL_SCRIPT       SCRIPTS_PATH"mac_filter_del"
#define WIRELESS_CONFIG_SCRIPT      SCRIPTS_PATH"wireless_inure_scr"
#define REBUILD_WLAN_SCRIPT         SCRIPTS_PATH"del_wlan_rebuild_wlans"
//#define APPLY_WEP_KEY_SCRIPT        SCRIPTS_PATH"apply_wep_key_scr"
#define COPY_FILE_SCRIPT            SCRIPTS_PATH"copy_file"
#define  REBOOT_TIME_PING_SCRIPT    SCRIPTS_PATH"reboot_time_and_ping"
#define MANAGE_IP_CONTROL_SCRIPT    SCRIPTS_PATH"manage_control_scr"
#define GET_AC_IP_SCRIPT            SCRIPTS_PATH"get_ac_ip_scr"
#define CONFIG_DNS_SCRIPT           SCRIPTS_PATH"config_dns_scr"
#define CONFIG_MANAGEMENT_VLAN_SCRIPT           SCRIPTS_PATH"config_mgmt_vlan"
#define ACIP_CHANGED_SCRIPT         SCRIPTS_PATH"ac_ip_changed_scr"
#define NAT_SCRIPT                  SCRIPTS_PATH"set_nat_scr"
#define UDHCPD_SCRIPT               SCRIPTS_PATH"startup_udhcpd_scr"
#define SNMP_SCRIPT                 SCRIPTS_PATH"restart_snmp"
#define TRAP_SWITCH_SCRIPT          SCRIPTS_PATH"trap_switch_scr"
#define RESTART_APCOMM_SCRIPT              SCRIPTS_PATH"restart_apcomm"
#define SET_AUTO_TF_UP_SCRIPT			SCRIPTS_PATH"set_traffic_up_scr"
#define SET_AUTO_STA_TF_UP_SCRIPT			SCRIPTS_PATH"set_sta_traffic_up_scr"
//#define WLAN11I_SCRIPT              SCRIPTS_PATH"11i_scr"
//#define SEC_11I_SCRIPT              SEC_11I_SCRIPT

/*add by lzc,20100630*/
#define WLAN_DEL                    SCRIPTS_PATH"wlan_del"

#define DHCPD_LEASES_FILE           "/var/lib/misc/udhcpd.leases"
#define PROC_NET_ROUTE              "/proc/net/route"
#define DHCLIENT_LEASES             "/var/state/dhcp/dhclient.leases"
#define DATA_FILE_PATH              "/etc/ap11n/"   /*edit by liwei, 20100402*/
#define PROC_NET_DEV                "/proc/net/dev"
#define AP_SCAN_SCRIPT              SCRIPTS_PATH"ap_scan_scr"
//#define TFTP_UPG_GET_FILE_SCRIPT    SCRIPTS_PATH"tftp_upgrade_getfile_scr"
#define OPEN_RF_SCRIPT              SCRIPTS_PATH"open_rf"
#define CLOSE_RF_SCRIPT             SCRIPTS_PATH"close_rf"
#define ETH_INTEGRITY_SCR         SCRIPTS_PATH"eth_link_wlan_scr"
#define CAPWAP_INTEGRITY_SCR   SCRIPTS_PATH"capwap_integrity_scr"
#define SET_GRE_SCRIPT              SCRIPTS_PATH"set_gre_scr"
#define SET_NTP_SCRIPT              SCRIPTS_PATH"ntp_scr"
#define RUN_SET_NTP_SCRIPT          SCRIPTS_PATH"run_ntp_scr"
#define NTP_SYNC_SCRIPT          SCRIPTS_PATH"ntp_sync_scr"
#define STA_ADD_SCRIPT              SCRIPTS_PATH"sta_add_scr"
#define STA_DELETE_SCRIPT           SCRIPTS_PATH"sta_delete_scr"
#define STA_QOS_SCRIPT              SCRIPTS_PATH"sta_qos_scr"
#define STA_VLAN_SCRIPT             SCRIPTS_PATH"sta_vlan_scr"
#define STA_SID_SCRIPT              SCRIPTS_PATH"sta_sid_scr"
#define ADVANCED_SCRIPT           SCRIPTS_PATH"advanced_set_scr"  //addby xiaoshui,20100804
//#define STA_PMK_SCRIPT            SCRIPTS_PATH"sta_pmk_scr"
#define GENERATE_WAPICONF_SCR SCRIPTS_PATH"generate_wapiconf_scr"  //addby xieshaojun,20100809
#define SET_SNOOPING_SCR            SCRIPTS_PATH"set_snooping_scr"
#define SET_WLAN_RPIO_SCRIPT        SCRIPTS_PATH"set_wlan_priority_scr"
#define SET_DATA_TUNNEL_MODE_SCRIPT SCRIPTS_PATH"set_data_tunnel_mode_scr"
#define SET_JAC_SCRIPT              SCRIPTS_PATH"set_jac_scr"
#define AUTOSET_CHANNEL_SCRIPT      SCRIPTS_PATH"auto_channel_period_scr"
#define PARSE_TO_APCOMM_SCRIPT      SCRIPTS_PATH"parse_para_to_apcomm"
#define RUN_ARP_PING_SCRIPT         SCRIPTS_PATH"run_arp_ping_scr"
#define WIRELESS_NO_RF_SWITCH_SCRIPT    SCRIPTS_PATH"wireless_with_no_rf_switch_scr"
#define RUN_AUTO_CHANNEL_SCRIPT         SCRIPTS_PATH"run_auto_channel_period_scr"
#define SET_STP_SCR                     SCRIPTS_PATH"set_stp_scr"
#define SYSLOG_SCRIPT                   SCRIPTS_PATH"syslog_scr"
#define SET_DATE_SCR                    SCRIPTS_PATH"write_rtc_conf"
#define SET_ETHER_SCRIPT                SCRIPTS_PATH"rceth"
#define RESTART_SNMP                    SCRIPTS_PATH"restart_snmp"
#define STATIC_CPU_MEM_SCR		SCRIPTS_PATH"statis_cpu_mem_scr"
#define SET_WAPI_REKEY_SCRIPT		SCRIPTS_PATH"set_wapi_rekey_scr"


#if 1//add by xialiang,20110114

/*20091116,xieshaojun,热备用时不允许重新加入
#define REJOIN_AC_SCRIPT               SCRIPTS_PATH"rejoin_ac_scr"
*/
/*add by lzc,20100816*/
#if 1
#define SET_PUREN_SCRIPT                SCRIPTS_PATH"set_puren"
#endif
#define SYSLOG_TABLE_SIZE 6
#define MAX_SYSLOG_SERVER_CNT 4
#define SYSLOG_BACKUP_INTERVAL 15       /*胖AP的syslog从内存备份到flash的周期*/
#define SHA256_DIGEST_LENGTH      32


//定义日志文件中最大日志数量
#define MAX_LOG_FILE_SIZE 1000

//add by xsj 080815
#if 0
#include <asm/types.h>
#include <linux/netlink.h>
#endif

typedef struct logDataLink
{
	char logData[256];
	char *logDataid[6];
	int  logDataLen[6];
	int  logDateStrLen;
	int  pri;
	int  flags;
	struct logDataLink *next;
} logDataLink;

struct eth_status_msg
{
	unsigned short int msg_id;
	unsigned short int datalen;
	int link_status;
};

struct user_status_msg
{
	unsigned short int msg_id;/*1000:关联 1001:解关联 1010:鉴权成功 1011:鉴权失败
    							1012:请求AC判断是否接入  1013:收到prob req*/
	unsigned short int datalen;
	unsigned char mac[6];
	unsigned char macpad[2];
	unsigned char bssid[6];
	unsigned char bssidpad[2];
	unsigned char ssidlen;
	char ssid[32];
	char qos_enabled;		/*是否启用QOS,当msgid=1013时,表示 prob req是否为 广播*/
	unsigned char authtype;	/*鉴别类型,当msgid=1013时,固定为0*/
	unsigned char wlanid;		/*1..8  129..136*/
#if 1   /*add by liwei,20100826*/
	unsigned char radioid;  //  0-1
	char pad[3];
#endif
};
struct ap_addstafail_msg
{
	unsigned short int msg_id;//1002: AP无法增加新的移动用户
	unsigned short int datalen;
	unsigned char mac[6];
	unsigned char macpad[2];
	unsigned char bssid[6];
	unsigned char bssidpad[2];
	unsigned char ssidlen;
	char ssid[32];
	char ssidpad[3];
	unsigned int reasonid; //原因ID
	char reasonstring[128]; //原因描述
	char if_info[64]; /*接口信息*/
};
struct ap_nofreechannel_msg
{
	unsigned short int msg_id;//1003: AP可供使用的信道数过低
	unsigned short int datalen;
	unsigned char bssid[6];
	unsigned char bssidpad[2];
	unsigned char ssidlen;
	char ssid[32];
	char ssidpad[3];
	unsigned int bgflag; // 1:b/g频段 2:a频段
	char descripstring[128];  //原因描述
};
struct ap_channelinterference_msg
{   //当前信道出现干扰  ，表示其他AP的干扰
//出现干扰当前信道的AP ，表示其他不同ssid的AP的干扰
	unsigned short int msg_id;//1004: 当前信道出现干扰  1005:出现干扰当前信道的AP
	unsigned short int datalen;
	unsigned char bssid[6];
	unsigned char bssid_rouger[6];
	char ssid[32];
	unsigned char ssidlen;
	char pad[3];
};
struct ip_change_stut
{
	unsigned short int msg_id;
	unsigned short int datalen;
	u_char old_ip[8];
	u_char new_ip[8];
};
struct channel_change_stut
{
	unsigned short int msg_id;
	unsigned short int datalen;
	char mode[16];
	char old_chl[4];
	char new_chl[4];
};
struct cfg_change_stuct
{
	unsigned short int msg_id;
	unsigned short int datalen;
	char cfg_change_msg[512];
};

struct trap_msg_head
{
	unsigned short int msg_id;
	unsigned short int datalen;
};
#if 0//add by xialiang,20110115
struct nlmsg_cgi
{
	struct nlmsghdr hdr;
	char msg_data[4096];
};
#endif
//AP受到DOS攻击的trap
struct ap_dosattack_msg
{
	unsigned short int msg_id;//1009,AP受到DOS攻击
	unsigned short int datalen;
	unsigned int dip;  //攻击包的目的IP地址
	unsigned int sip;  //攻击包的源IP地址
	unsigned short int dport; //攻击包的目的端口
	unsigned short int sport; //攻击包的源端口
	unsigned char mac[6];     //攻击包的源MAC地址
	unsigned char fromethflag;//攻击包是否来自无线
	unsigned char pad;
};

#if 1 //add by xialiang,20100917
struct grentech_ethernet_stats
{
unsigned long eth_tx_multicast;/*发送的广播包*/
unsigned long eth_tx_unicast;/* 发送的单播包*/
unsigned long eth_rx_multicast;/*接收的广播包*/
unsigned long eth_rx_unicast;/*接收的单播包*/
//unsigned long eth_tx_drop;/*发送时丢弃的包*/
};
#endif

//struct sta_stats_report_msg,see wlan/ieee80211_ioctl.h

/**基本类型定义*/
typedef unsigned char  BYTE;    //1字节
typedef unsigned short WORD;    //2字节
typedef unsigned long  DWORD;   //4字节
#if 0
/**基本数据类型，长度为2个字节*/
struct word_data_t
{
	WORD    flag;           //数据类型标识
	WORD    length;         //数据长度
	BYTE    *data;          //数据内容
} __ATTRIBUTE_PACK__;

typedef struct word_data_t word_data;
#endif
#ifndef IFNAMSIZ
#define IFNAMSIZ        16
#endif
#define WALN_ALEN                                       6
#define P80211_IF_AUTH                          (0x8BE0 + 26)//0x8BFA
#define P80211_PACKET_SEND_FRAME        (WORD)0xF801

struct frame_element_t
{
	BYTE bssid[WALN_ALEN];
	BYTE da[WALN_ALEN];
	BYTE type;
	BYTE pad[3];
};//__attribute__ ((packed))
#if 0
struct param_element_t
{
	char    ifname[IFNAMSIZ];               /* name of the interface to send this out on */
	struct frame_element_t frame;
};
#endif

#define bit(x) 1<<x

//add by xieshj 2008-07-30
enum auth_type_11i
{
	AUTH_OPEN = 0,
	AUTH_WEP = 1,
	AUTH_EAP_TKIP = 4,
	AUTH_EAP_CCMP = 8,
	AUTH_PSK_TKIP = 16,
	AUTH_PSK_CCMP = 32
};

#define USER_BANDWIDTH_PREFIX "userbd"
#define WLAN_PRIORITY_PREFIX "wlan_pri"
#if AUTO_FLOW_VER == 1
#define AUTO_FLOW_PREFIX "auto_flow"
#endif
enum security_mode_t
{
	SECURITY_MODE_WAPI = 1,
	SECURITY_MODE_11I = 2,
	SECURITY_MODE_WAPI_11I = 3
};
#define    ENCRYPTION_NONE "0"
#define    ENCRYPTION_WEP "1"
#define    ENCRYPTION_TKIP "2"
#define    ENCRYPTION_AES "4"
#define    ENCRYPTION_TKIP_OR_AES "6"

#if 1
#define	IEEE80211_NODE_QOS	0x0002		/* QoS enabled */
#endif

//add by xsj 080815
#if 1
#if 0
#define ALARM_ID_COLD_START (100)
#define ALARM_ID_WARM_START (99)
#define ALARM_ID_AP_DOWN (0)
#define ALARM_ID_IP_CHANGED (1)
#define ALARM_ID_ENCRPTION_CHANGED (2)
#define ALARM_ID_PSK_CHANGED (3)
#define ALARM_ID_CHANNEL_CHANGED (4)
#define ALARM_ID_CHANNEL_INTERFERE (5)
#define ALARM_ID_FLASH_WRITEERROR (6)
#define ALARM_ID_TIME_SYNC_FAILED (7)
#define ALARM_ID_USABLE_CHANNEL_TOOFEW (9)
#define ALARM_ID_KEEP_ALIVE_TRAP (11)
#define ALARM_ID_DISCOVER_INSECURITY_AP (12)
#define ALARM_ID_CURRENT_CHANNAL_JAMMING (13)
#define ALARM_ID_REMOT_UPGRADE (26)
#define ALARM_ID_WLAN_RF_A (27)
#define ALARM_ID_WLAN_RF_B (28)
#define ALARM_ID_WIFI0_USER_NUM (29)
#define ALARM_ID_WIFI1_USER_NUM (30)
#define ALARM_ADMIN_LOGINED (31)
#define CONFIG_FILE_CHANGED (32)
#define ALARM_LOGIN_FAILED  (33)
#endif

#if 1 //xieshaojun,2010707
/*20100722,配置成可创建15个SSID后，双数SSID无法接入，实际只能支持8个SSID*/
//#define	ATH_BCBUF	16		/* number of beacon buffers ,see wlan/common/lmac/ath_dev/ath_internal.h*/
//#define	ATH_BCBUF	8		/* number of beacon buffers ,see wlan/common/lmac/ath_dev/ath_internal.h*/
#define SSID_MAX_COUNT ATH_BCBUF
#endif
#if 0
#define ALARM_ID_ETHERNET_LINK_STATUS	(500) /*以太网口连接状态*/
#define ALARM_ID_STA_ASSOCIATION    (1000) //用户关联成功
#define ALARM_ID_STA_DISASSOCIATION    (1001) //用户解除关联
#define ALARM_ID_STA_ADD_FAIL        (1002) //AP无法增加新的移动用户
#define ALARM_ID_NO_FREE_CHANNEL    (1003) /*AP可供使用的信道数过低*/
#define ALARM_ID_CHANNEL_INTERFERENCE        (1004) //当前信道出现干扰，表示其他AP的干扰
#define ALARM_ID_CHANNEL_INTERFERENCE_AP    (1005) //出现干扰当前信道的AP，表示其他不同ssid的AP的干扰
#define ALARM_ID_NOT_SAFE_AP                (1006) //出现不安全的AP
#define ALARM_ID_STA_INTERF_DETECTED			1007	/*AD-Hoc或多终端或终端距离较远，产生了B模式*/
#define ALARM_ID_STA_DDOS_ATTACK          1009  /*DDOS攻击*/
#define ALARM_ID_STA_AUTHENTICATE_SUCCESS	(1010)	/*鉴权成功*/
#define ALARM_ID_STA_AUTHENTICATE_FAILED	(1011)	/*鉴权失败*/
#define ALARM_ID_STA_ASK_FOR_ACCESS			(1012)	/*请求AC判断是否接入*/
#define ALARM_ID_STA_PROBE_REQUEST			(1013)	/*收到prob req*/
#define ALARM_ID_CHANNEL_INTERFERENCE_CLEAR		1022	/*干扰当前信道的相同SSID的AP消失*/
#define ALARM_ID_STA_INTERF_CLEAR				1023	/*终端干扰告警清除*/
#define ALARM_ID_CHANNEL_FREE					1024	/*可供使用的信道数过低告警清除*/
#define ALARM_ID_PRE_AUTH_80211I_REQUEST			(1025)	/*802.11预认证*/
#define ALARM_ID_BSSID_RATE_REPORT				(1026)	/*上报单位时间内基于BSSID流量*/
#define ALARM_ID_STA_ASSOCIATION_FAIL			(1027)	/*立即告警，要求包含端站MAC地址、关联失败原因*/
#define ALARM_ID_STA_AUTH_ERROR					(1028)	/*立即告警，要求包含端站MAC地址、鉴权失败原因*/
#define ALARM_ID_STA_POWER_DOWN					(1029)	/*STA未与AP解除关联而关机*/
#define ALARM_ID_STA_POWER_DOWN_CLEAR			(1030)	/*STA未与AP解除关联而关机，重新关联成功*/
#define ALARM_ID_REPORT_BLACK_STA_TO_AC	                (1031) /*add by luozhicheng,20100605,上报泛洪攻击终端到AC*/
#define ALARM_ID_REPORT_SPOOF_STA_TO_AC			(1032)	/*add bu lzc,20100610*/
#define ALARM_ID_REPORT_VAP_STATS			(1033)	/*add bu xieshaojun,20100701*/
#define ALARM_ID_REPORT_ETH_STATS			(1034)	/*add bu xieshaojun,20100701*/
#define ALARM_ID_REPORT_STA_STATS			(1035)	/*add bu xieshaojun,20100701*/
#endif
#endif
#define INSIDE_ERROR            "发生了一个内部错误，请重新尝试操作；若错误重复出现，请联系管理员!"

#define MAX_FILES		3 /*xieshaojun,from webtools*/
#if 0
struct upfile_st
{
	char data[3072]; //char *data;
	int datalen;
};
#else
struct upfile_st
{
	char name[255];
	char type[255];
	char encode[255];
	char data[3072]; //*data;
	int datalen ;
	int datatype;
};
#endif
#define  VERSIONNOW			0x0001	/*WAPI 当前版本号*/
#define CGI_PORT				9002
#define SNMPD_PORT			9003

#define CGI_CMD_RELOAD			0x0212
#define CGI_RELOAD_RESPONSE	0x0213
#define CGI_CMD_CHECKCERT		0x0214
#define CGI_CHECK_RESPONSE		0x0215
#if 0
#define CGI_CMD_ADD_DEV		0x0217
#define CGI_CMD_DEL_DEV		0x0218
#define CGI_CMD_EXIT			0x0800
#define CGI_CMD_RELOADNORESP	0x0801
#endif
#define RECETIMEOUT 10
#define TYPE_GET_STATIONS_COUNT 0
#define TYPE_GET_STATIONS_STATS 1


/*add by wyh@070209 for mib*/
#define SNMPD_CMD_GET_WAI_STATS		0x0312
#define SNMPD_CMD_SET_WAI_STATS		0x0313
#if 1 /*add by fangzhijun, 20100906*/
#define SNMPD_CMD_GET_SET_WAI_PRE		0x0314
#define SNMPD_CMD_GET_SET_UNI_REK_M		0x0315
#define SNMPD_CMD_GET_SET_UNI_REK_TIME		0x0316
#define SNMPD_CMD_GET_SET_UNI_REK_PACKETS		0x0317


#define SNMPD_CMD_GET_SET_MUL_REK_M		0x0318
#define SNMPD_CMD_GET_SET_MUL_REK_TIME		0x0319
#define SNMPD_CMD_GET_SET_MUL_REK_PACKETS		0x0320

#define SNMPD_CMD_GET_SET_LEAVE_REK 0x0321
#define SNMPD_CMD_GET_SET_RETRY_COUNT  0x0322
#define SNMPD_CMD_GET_SET_STA_TIMEOUT 0x0323
#define SNMPD_CMD_GET_LAST_NEG_AKM 0x0324
#define SNMPD_CMD_GET_LAST_BKID 0x0325
#define SNMPD_CMD_GET_LAST_AKM 0x0326
typedef struct _ap_wapi_pre
{
	int pre_status;
	int opt;
	unsigned int value;
	unsigned int result;
} AP_WAPI_PRE_ENTRY;

typedef struct _ap_wapi_uni_rek_m
{
	int opt;
	unsigned int value;
	unsigned int result;
} AP_WAPI_UNI_REK_M_ENTRY;

typedef struct _ap_wapi_uni_rek_time
{
	int opt;
	time_t value;
	time_t result;
} AP_WAPI_UNI_REK_TIME_ENTRY;

typedef struct _ap_wapi_uni_rek_packets
{
	int opt;
	unsigned int  value;
	unsigned int result;
} AP_WAPI_UNI_REK_PACKETS_ENTRY;

typedef struct _ap_wapi_mul_rek_m
{
	int opt;
	unsigned int value;
	unsigned int result;
} AP_WAPI_MUL_REK_M_ENTRY;

typedef struct _ap_wapi_mul_rek_time
{
	int opt;
	time_t value;
	time_t result;
} AP_WAPI_MUL_REK_TIME_ENTRY;

typedef struct _ap_wapi_mul_rek_packets
{
	int opt;
	unsigned int  value;
	unsigned int result;
} AP_WAPI_MUL_REK_PACKETS_ENTRY;

typedef struct _ap_wapi_leave_rek
{
	int opt;
	unsigned int  value;
	unsigned int result;
} AP_WAPI_LEAVE_REK_ENTRY;

typedef struct _ap_wapi_sta_timeout
{
	int opt;
	unsigned int  value;
	unsigned int result;
} AP_WAPI_STA_TIMEOUT_ENTRY;


typedef struct _ap_wapi_retry_count
{
	int opt;
	unsigned int flag;
	unsigned int authRetryCount;
	unsigned int uniRetryCount;
	unsigned int mulRetryCount;
} AP_WAPI_RETRY_COUNT_ENTRY;

typedef struct _ap_wapi_last_bkid
{
	u_int8_t bkid[16];
	u_int8_t lasttime[4];
} AP_WAPI_LAST_BKID_ENTRY;

typedef struct _ap_wapi_last_akm
{
	unsigned int flag;
} AP_WAPI_LAST_AKM_ENTRY;
#endif

#if 0
#define PRINT_LINE {printf("-------%s,line:%d++++++\n",__FILE__,__LINE__);}
#define DEBUG_INT(a) {printf("-------%s,line:%d,value=%d++++++\n",__FILE__,__LINE__, a);}
#define PRINTD {int testInputValue; printf("file:%s, line=%d\n", __FILE__, __LINE__); scanf("%d", &testInputValue);}
#else
#define PRINT_LINE
#define DEBUG_INT(a)
#define PRINTD
#endif

#define ESSID_BUF_LEN 36 /*add by xieshaojun,20100826*/
/*wapid发送到snmpd的格式*/
struct _wapi_trap_head_info
{
	unsigned short ver; /*版本号*/
	unsigned short cmd;/*消息号*/
	char		ssid[ESSID_BUF_LEN]; /*SSID名称*/
	char		bssid[20]; /*vap的mac*/
	char		sta_mac[20]; /*终端MAC地址*/
	char		if_info[64]; /*接口信息*/
};

#define MAX_MAC_ELM	255 /*MAC地址最大数为255*/

#endif // __CONFIG_H__
#endif
