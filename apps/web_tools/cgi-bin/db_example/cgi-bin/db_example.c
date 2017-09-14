#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include <stdlib.h>
#include "myproc.h"

DB_CONTENT_T db_content;

int initDBInterface(void)
{
	int rc = 0;
	sqlite3_initialize( );
	
	rc = sqlite3_open_v2( DB_FILE, &db_content.rwDBConn, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, NULL );
	if ( rc != SQLITE_OK) 
	{
		sqlite3_close( db_content.rDBConn );
		return DB_ERROR;
	}

	rc = sqlite3_open_v2( DB_FILE, &db_content.rDBConn, SQLITE_OPEN_READONLY |  SQLITE_OPEN_FULLMUTEX, NULL );
	if ( rc != SQLITE_OK) 
	{
		sqlite3_close( db_content.rDBConn );
		return DB_ERROR;
	}
	pthread_mutex_init(&db_content.m_lock,NULL);
	return DB_OK;
		
	 
	
}
void unInitDBInterface(void)
{
	sqlite3_shutdown( );	
	sqlite3_close(db_content.rDBConn);
	sqlite3_close(db_content.rwDBConn);
	pthread_mutex_destroy(&db_content.m_lock);
}
int main(void)
{
	int i = 0;
	int iCount = 0;
	#if 0
	PSTAINFO_T pStaInfo = NULL;
	
	int iCount = get_online_stas();
	for (i = 0;i<iCount;i++)
  		printf("%d %s %s\n",gStaInfo[i].id,gStaInfo[i].mac,gStaInfo[i].curDate);
  #endif
  #if 0
  AP_GROUP_INFO_T apgroupinfo;
  apgroupinfo.id = 1;
  strcpy(apgroupinfo.name,"default");
  strcpy(apgroupinfo.desc,"default");
  int ret = insert_ap_group(apgroupinfo);
  if (!ret)
  {
  	printf("error!\n");
  	return 0;
  }	
  #endif
  #if 0
  int groupid = 2;
  int ret = delete_ap_group(groupid);
  if (!ret)
  {
  	printf("error!\n");
  	return 0;
  }
  
  iCount = get_ap_group();
  printf("iCount:%d\n",iCount);
  for (i = 0;i<iCount;i++)
  		printf("%d %s %s\n",gapInfo[i].id,gapInfo[i].name,gapInfo[i].desc);
	#endif
	
	#if 0
	iCount = get_version_config();
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s %s %s\n",gVersionInfo[i].id,gVersionInfo[i].hardware_version,gVersionInfo[i].manufacturer,gVersionInfo[i].software_version);
  }
  #endif
  
  #if 0
	iCount = get_version_config();
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s %s %s\n",gVersionInfo[i].id,gVersionInfo[i].hardware_version,gVersionInfo[i].manufacturer,gVersionInfo[i].software_version);
  }
  #endif
  

  

  
  

  

  

  

  
  #if 0
	iCount = get_ntp_client_config();
	printf("iCount:%d \n",iCount);
	if (iCount > 1)
	{
		if (gNtpClientConfig[i].flag != NTP_AP_CONFIG)
		{
			return DB_ERROR;
		}
	}
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %d \n",gNtpClientConfig[i].id,gNtpClientConfig[i].syn_time);
  }
  #endif
  
  

  
  

  
 
  

  

  
  
  
  #if 0
	initDBInterface();
	char mac[MAC_LEN];
	AP_ITEM_T apItem;
	strcpy(mac,"00:11:B5:00:11:22");
	iCount = get_ap_accord_mac(&db_content,mac,&apItem);
	
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
		printf("%d %s %s  gid:%d \n",apItem.ap_item_id,apItem.ip,apItem.mac,apItem.ap_group_id);
	}
	unInitDBInterface();
  #endif
  
  
  #if 0
  int gid = 1;
  
	iCount = get_ap_policy_accord_gid(gid);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s %d  \n",gpolicyItem[i].policy_item_id,gpolicyItem[i].policy_name,gpolicyItem[i].day_index);
  }
  #endif
  #if 0
	initDBInterface();
	char gid = 5;
	AP_ITEM_T apItems[MAX_AP_PER_G];
	iCount = get_ap_accord_gid(&db_content,gid,apItems);

	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
		printf("%d %s %s  gid:%d \n",apItems[i].ap_item_id,apItems[i].ip,apItems[i].mac,apItems[i].ap_group_id);
	}
	unInitDBInterface();
  #endif
  #if 0
  initDBInterface();
  AP_ITEM_T apItem;
  strcpy(apItem.ip,"192.168.1.123");
  apItem.ap_status = 2;
  apItem.version_info_id = 4;
  apItem.radio_count = 2;
  strcpy(apItem.mac,"00:11:B5:01:01:29");
  iCount = update_ap_item_mac(&db_content,&apItem);
  printf("iCount:%d \n",iCount);
  unInitDBInterface();
  #endif
  #if 0
  	initDBInterface();
	iCount = update_invalid_ap_switch(&db_content,2);
	printf("new iCount:%d \n",iCount);
	unInitDBInterface();
  #endif
  #if 0
 	 initDBInterface();
	iCount = get_invalid_ap_switch(&db_content);
	printf("new iCount:%d \n",iCount);
	unInitDBInterface();
  #endif

  #if 0
  	initDBInterface();
	char mac[MAC_LEN];
	INVALID_AP_ITEM_T pap;
	strcpy(pap.mac,"22:xx:B5:43:44:66");
	iCount = insert_no_group_ap_item(&db_content,pap);
	printf("new iCount:%d \n",iCount);
	unInitDBInterface();
  #endif

  #if 0
  	initDBInterface();
	char mac[MAC_LEN];
	INVALID_AP_ITEM_T pap;
	strcpy(pap.mac,"00:11:B5:23:44:69");
	strcpy(pap.bssid,"00:11:B5:23:44:66");
	strcpy(pap.ssid,"00:11:B5:23:44:66");
	pap.radio_id = 1;
	pap.channel = 1;
	pap.single = 1;
	pap.snr = 1;
	pap.rate = 1;
	pap.ap_type = 1;
	iCount = insert_invalid_ap_item(&db_content,pap);
	printf("new iCount:%d \n",iCount);
	unInitDBInterface();
  #endif
  #if 0
  initDBInterface();
  POLICY_ITEM_T policyItems[MAX_AP_POLICY];
  iCount = get_policy_item(&db_content,policyItems);
  int apCount = 0;
 
  int j = 0;
  if (iCount > 0)
  {
  	printf("iCount:%d\n",iCount);
  	for (i = 0;i < iCount;i++)
  	{
	  	apCount = get_ap_policy_accord_gid(&db_content,policyItems[i].policy_group_id);
		printf("apCount:%d\n",apCount);	
	  	if (apCount > 0)
	  	{
			for (j = 0;j<apCount;j++)
			{
				printf("%s %s\n",gapItem[j].ap_name,gapItem[j].location);
			}
		}
  	}
  }
  unInitDBInterface();
  #endif
  
  #if 0
  initDBInterface();
  int gid = 1;
  int j = 0;
  char mac[MAC_LEN];
  WLAN_ITEM_T wlanItems[MAX_WLAN_ITEM];
  WLAN_SECURITY_POLICY_T wlanSecurityPolicys[MAX_WLAN_SECURITY];
  strcpy(mac,"00:11:B5:01:01:29");
  AP_ITEM_T apItem;
  
  iCount = get_ap_accord_mac(&db_content,mac,&apItem);//根据MAC地址获取AP信息
  printf("apItem count:%d  \n",iCount);
  if (iCount == 0)
  {
		printf("apItem record not right!\n");
  }
  else
  {
		iCount = get_ap_wlan_accord_gid(&db_content,apItem.ap_group_id,wlanItems);//获取当前AP的WLAN配置
		printf("wlanItem count:%d apItem.ap_group_id:%d\n",iCount,apItem.ap_group_id);
		if (iCount == 0 )
		{
			printf("wlanItem record not right");
		}
		for (i = 0;i<iCount;i++)
		{
			printf("ssid%d:%s wlan_security_policy_id:%d\n",i,wlanItems[i].ssid,wlanItems[i].wlan_security_policy_id);
			int sCount  = get_ap_wlan_security(&db_content,wlanItems[i].wlan_security_policy_id,wlanSecurityPolicys);	
			if (sCount > 0)
			{
				printf("key4 = %s\n",wlanSecurityPolicys[0].key4);
			}
		}
  }
  
  	
	 unInitDBInterface(); 
  #endif

  #if 0
  initDBInterface();
  AP_GROUP_INFO_T pAP_group;
  pAP_group.id = 1;
  pAP_group.radio_status = 4;
  iCount = update_ap_group_status(&db_content,pAP_group);
  printf("iCount:%d \n",iCount);
  unInitDBInterface();
  #endif

   #if 0
   	 initDBInterface();
   	RADIUS_CONFIG_T radiusConfigInfos[MAX_RADIUS_CONFIG];
	iCount = get_radius_config(&db_content,radiusConfigInfos);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s %s\n",radiusConfigInfos[i].id,radiusConfigInfos[i].ip,radiusConfigInfos[i].pwd);
 	 }
	unInitDBInterface();
  #endif
  
   #if 0
   	initDBInterface();
	NTP_CONFIG_T ntpConfigInfos[MAX_NTP_CONFIG];
	iCount = get_ntp_config(&db_content,ntpConfigInfos);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %d %s\n",ntpConfigInfos[i].id,ntpConfigInfos[i].switch_flag,ntpConfigInfos[i].ip);
	}
	unInitDBInterface();
  #endif

     #if 0
	 initDBInterface();
	 DOWNLOAD_CONFIG_T downloadConfigInfos[MAX_DOWNLOAD_CONFIG];
 	 iCount = get_download_config(&db_content,downloadConfigInfos);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s %s  \n",downloadConfigInfos[i].id,downloadConfigInfos[i].ip,downloadConfigInfos[i].version_name);
  }
	unInitDBInterface();
  #endif
    #if 0
	 initDBInterface();
	AS_CONFIG_T asConfigInfos[MAX_AS_CONFIG];
	iCount = get_as_config(&db_content,asConfigInfos);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s \n",asConfigInfos[i].id,asConfigInfos[i].cert_pwd);
  }
	unInitDBInterface();
  #endif
    #if 0
	 initDBInterface();
	SNMP_CONFIG_T snmpConfigInfos[MAX_SNMP_CONFIG];
	iCount = get_snmp_config(&db_content,snmpConfigInfos);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s \n",snmpConfigInfos[i].id,snmpConfigInfos[i].trap_ip1);
  }
	unInitDBInterface();
  #endif
  #if 0
	initDBInterface();
	int gVersionCount = 0;
	CHECK_INFO_T CheckInfo;
	VERSION_INFO_T VersionInfos[MAX_VERSION_INFO];
	strcpy(CheckInfo.mac,"00:11:B5:01:01:29");
	gVersionCount = get_version_config(&db_content,VersionInfos);
	int ret = check_ap_config(&db_content,&CheckInfo);
	if (ret > 0)
	{
		for (iCount = 0;iCount <gVersionCount;iCount++)
		{
			if ((!strcmp(VersionInfos[iCount].hardware_version,CheckInfo.hardware_version))
			&&(!strcmp(VersionInfos[iCount].hardware_version,CheckInfo.manufacturer))
			&&(!strcmp(VersionInfos[iCount].device_model,CheckInfo.device_model)))
			{
				break;
			}
		}
	}
	printf("version index:%d\n",iCount);
	unInitDBInterface();
  #endif

     #if 0
	 initDBInterface();
	 HOTBAK_CONFIG_T HotbakConfigs[MAX_HOTBAK_CONFIG];
	iCount = get_hotbak_config(&db_content,HotbakConfigs);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s \n",HotbakConfigs[i].id,HotbakConfigs[i].manage_ip);
  }
	unInitDBInterface();
  #endif
    
  #if 0
  	initDBInterface();
	NTP_SERVER_CONFIG_T  NtpServerConfigs[MAX_NTP_SERVER_CONFIG];
	iCount = get_ntp_server_config(&db_content,NtpServerConfigs);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %d \n",NtpServerConfigs[i].id,NtpServerConfigs[i].switch_flag);
  }
	unInitDBInterface();
  #endif
      #if 0
	initDBInterface();
	  NTP_CLIENT_CONFIG_T NtpClientConfigs[MAX_NTP_CLIENT_CONFIG];
	iCount = get_ntp_client_config(&db_content,NtpClientConfigs);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
		printf("%d %d \n",NtpClientConfigs[i].id,NtpClientConfigs[i].syn_time);
	}
	unInitDBInterface();
  #endif

    #if 0
	initDBInterface();
	ROUTE_CONFIG_T RouteConfigs[MAX_ROUTE_CONFIG];
	iCount = get_route_config(&db_content,RouteConfigs);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s \n",RouteConfigs[i].id,RouteConfigs[i].next_hop);
  }
	unInitDBInterface();
  #endif

   #if 0
	 initDBInterface();
   	SYSLOG_CONFIG_T SyslogConfigs[MAX_SYSLOG_CONFIG];
	iCount = get_syslog_config(&db_content,SyslogConfigs);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s %s \n",SyslogConfigs[i].id,SyslogConfigs[i].ip1,SyslogConfigs[i].ip2);
  }
	unInitDBInterface();
  #endif

    #if 0
	initDBInterface();
	NET_INFO_T NetInfos[MAX_NET_INFO];
	iCount = get_net_info(&db_content,NetInfos);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s %s \n",NetInfos[i].id,NetInfos[i].ip,NetInfos[i].netmask);
  }
	unInitDBInterface();
  #endif

	
  #if 0
  	initDBInterface();
  MAC_FILTER_T MacFilters[MAX_MAC_FILTER];
	iCount = get_mac_filter(&db_content,MacFilters);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s  \n",MacFilters[i].id,MacFilters[i].mac);
  }
	unInitDBInterface();
  #endif

     #if 0
	 initDBInterface();
	 RADIO_BASE_T Radio_bases[MAX_RADIO_BASE];
	iCount = get_radio_base(&db_content,Radio_bases);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %d %d  \n",Radio_bases[i].id,Radio_bases[i].MPDU,Radio_bases[i].MSDU);
  	}
	unInitDBInterface();
  #endif

  #if 0
  	initDBInterface();
  	RADIO_ADVANCE_T RadioAdvances[MAX_RADIO_ADVANCE];
	iCount = get_radio_advance(&db_content,RadioAdvances);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %d %s  \n",RadioAdvances[i].id,RadioAdvances[i].action,RadioAdvances[i].action_ip);
  }
	unInitDBInterface();
  #endif
  
    #if 0
	initDBInterface();
	CHAN_CONFIG_T ChanConfigs[MAX_CHAN_CONFIG];
	iCount = get_chan_config(&db_content,ChanConfigs);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %d %d  \n",ChanConfigs[i].id,ChanConfigs[i].chan_auto,ChanConfigs[i].min_rssi);
  }
	unInitDBInterface();
  #endif
  
  
  #if 0
  	initDBInterface();
  	SCAN_CONFIG_T ScanConfigs[MAX_SCAN_CONFIG];
	iCount = get_scan_config(&db_content,ScanConfigs);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %d %d  \n",ScanConfigs[i].id,ScanConfigs[i].chan,ScanConfigs[i].period);
  }
	unInitDBInterface();
  #endif
  
   #if 0
   	initDBInterface();
   	LOAD_BALANCE_T LoadBalances[MAX_LOAD_BALANCE];
	iCount = get_load_balance(&db_content,LoadBalances);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %d %d  \n",LoadBalances[i].id,LoadBalances[i].balance_type,LoadBalances[i].user_max);
  }
	unInitDBInterface();
  #endif
  
  #if 0
 	 initDBInterface();
  CAPWAP_TIMER_CONFIG_T CapwapTimerConfigs[MAX_CAPWAP_TIMER_CONFIG];
	iCount = get_capwap_timer_config(&db_content,CapwapTimerConfigs);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %d %d  \n",CapwapTimerConfigs[i].id,CapwapTimerConfigs[i].keep_alive,CapwapTimerConfigs[i].broadcast_period);
  }
	unInitDBInterface();
  #endif
  
  #if 0
  gid = 1;
  iCount = get_ap_wlan_security(gid);
	printf("iCount:%d \n",iCount);
	for (i = 0;i<iCount;i++)
	{
  		printf("%d %s %d  \n",gwlanSecurityPolicy[i].wlan_security_policy_id,gwlanSecurityPolicy[i].psk_key,gwlanSecurityPolicy[i].security_mode);
  }
  #endif
  
  #if 0
  char mac[MAC_LEN];
  strcpy(mac,"00:11:B5:22:44:66");
	iCount = update_ap_online_time_mac(mac);
	printf("iCount:%d \n",iCount);
  #endif
  
  



  #if 0
	char mac[MAC_LEN];
  	strcpy(mac,"00:11:B5:11:22:33");
	iCount = get_wlan_user_mode(mac,2);
	printf("new iCount:%d \n",iCount);
  #endif


  


  
 

  #if 0
  CHECK_INFO_T checkInfo;
  strcpy(checkInfo.mac,"00:11:B5:01:01:29");
  strcpy(checkInfo.hardware_version,"AP7161-Rev.A");
  strcpy(checkInfo.manufacturer,"powercn");
  strcpy(checkInfo.device_model,"SGR-W500-N82b");
  iCount = check_ap_config(&checkInfo);
  printf("%d\n",iCount);
  #endif

   #if 0
  CHECK_INFO_T checkInfo;
  strcpy(checkInfo.mac,"00:11:B5:1E:1C:3A");
  strcpy(checkInfo.hardware_version,"I32 V3.0");
  strcpy(checkInfo.manufacturer,"guoren");
  strcpy(checkInfo.device_mode,"V5.4.4");
  iCount = check_ap_config(&checkInfo);
  printf("%d\n",iCount);
  #endif
  

   #if 1
   initDBInterface();
  STA_ITEM_T pstaItem;
  strcpy(pstaItem.ap_mac,"00:11:B5:11:22:33"); 
  strcpy(pstaItem.bssid,"xxxxxx");
  strcpy(pstaItem.ssid,"ssid");
  strcpy(pstaItem.mac,"00:24:D2:0D:DC:C2");
  iCount = insert_sta_item(&db_content,pstaItem);
  printf("iCount:%d \n",iCount);
  unInitDBInterface();
  #endif

  #if 0
    iCount = get_ap_group();
  printf("iCount:%d\n",iCount);
  for (i = 0;i<iCount;i++)
  		printf("%d %s %s\n",gapInfo[i].id,gapInfo[i].name,gapInfo[i].desc);
  #endif

    #if 0
    AP_GROUP_INFO_T pAP_group;
    pAP_group.id = 1;
    iCount = get_ap_group_accord_id(pAP_group);
  printf("iCount:%d\n",iCount);
  for (i = 0;i<iCount;i++)
  		printf("%d %s %s %d\n",gapInfo[i].id,gapInfo[i].name,gapInfo[i].desc,gapInfo[i].radio_status);
  #endif
  
	return 0;
}
