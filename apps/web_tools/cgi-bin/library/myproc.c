#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include    <time.h>
#include <pthread.h>

#include <stdlib.h>
#include "myproc.h"
#include "config.h"

#ifdef MEM_DEBUG
#include "memwatch.h"
#else
#include <stdlib.h>
#endif

void get_column_text(char *dest,const char *src)
{
	if (src)
		strcpy(dest,src);
}

//AP操作语句
/*==========================================================================*
 * FUNCTION :int get_ap_accord_mac(char *mac)
 * PURPOSE  :根据MAC地址获取AP信息		 
 * PARAMETERS:
 *	pdb_content->上下文
 *	mac->AP的MAC地址
 *	pApItem->返回的AP信息
 * RETURN   :返回AP数目
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_ap_accord_mac(DB_CONTENT_T *pdb_content,char *mac,AP_ITEM_T * pApItem)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;
	if (pApItem == NULL)
	{
		return 0;
	}

	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_AP_ITEM_MAC) )
	{
		goto bad;
	}
	
	idx = sqlite3_bind_parameter_index( lStmt, ":mac" );
	sqlite3_bind_text( lStmt, idx, mac, -1, SQLITE_STATIC );
	while( (rc = StepSql(pdb_content,lStmt)) == SQLITE_ROW ){
			pApItem->ap_item_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
			get_column_text(pApItem->mac,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
			get_column_text(pApItem->ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
			pApItem->ap_group_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
			get_column_text(pApItem->ap_name,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
			get_column_text(pApItem->ap_desc,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
			get_column_text(pApItem->location,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
			get_column_text(pApItem->online_time,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
			get_column_text(pApItem->start_time,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
			pApItem->ap_status = (int)sqlite3_column_int( lStmt, columnIndex++ );
			pApItem->radio_count = (int)sqlite3_column_int( lStmt, columnIndex++ );
			iCount += 1;
			columnIndex = 0;
	}
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_ap_accord_gid(int gid)
 * PURPOSE  :根据组ID获取在线AP信息
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *    gid->组ID
 *    pApItem->返回AP信息
 * RETURN   :返回AP数目
 *	
 * COMMENTS : gid AP组ID
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_ap_accord_gid(DB_CONTENT_T *pdb_content,int  gid,PAP_ITEM_T  ApItems)

{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;
	if (ApItems == NULL)
	{
		return 0;
	}

	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_AP_ITEM_GID) )
	{
		goto bad;
	}
	
	
    idx = sqlite3_bind_parameter_index( lStmt, ":gid" );
    sqlite3_bind_int( lStmt, idx, gid);
        
    while( (rc = StepSql(pdb_content,lStmt)) == SQLITE_ROW ){
    		ApItems[iCount].ap_item_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		get_column_text(ApItems[iCount].mac,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		get_column_text(ApItems[iCount].ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		ApItems[iCount].ap_group_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		get_column_text(ApItems[iCount].ap_name,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		get_column_text(ApItems[iCount].ap_desc,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		get_column_text(ApItems[iCount].location,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		get_column_text(ApItems[iCount].online_time,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		get_column_text(ApItems[iCount].start_time,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		ApItems[iCount].ap_status = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		ApItems[iCount].radio_count = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;
    	  	iCount += 1;
    }
    	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int update_ap_online_time_mac(char *mac)
 * PURPOSE  :根据MAC地址修改AP在线时间，状态和版本信息		 
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *	pItem->ap
 * RETURN   :返回状态
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int update_ap_item_mac(DB_CONTENT_T *pdb_content,PAP_ITEM_T pItem)
{
	int rc = 0;
	int  idx = -1;
	sqlite3_stmt    *lStmt = NULL;

	BeginTrans(pdb_content);
	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, UPDATE_AP_ITEM_MAC) )
	{
		goto bad;
	}
	
	idx = sqlite3_bind_parameter_index( lStmt, ":ip" );
	sqlite3_bind_text( lStmt, idx, pItem->ip, -1, SQLITE_STATIC );
	idx = sqlite3_bind_parameter_index( lStmt, ":id" );
	sqlite3_bind_int( lStmt, idx, pItem->version_info_id);
	idx = sqlite3_bind_parameter_index( lStmt, ":sta" );
	sqlite3_bind_int( lStmt, idx, pItem->ap_status);
	idx = sqlite3_bind_parameter_index( lStmt, ":radio_count" );
	sqlite3_bind_int( lStmt, idx, pItem->radio_count);
	idx = sqlite3_bind_parameter_index( lStmt, ":mac" );
	sqlite3_bind_text( lStmt, idx, pItem->mac, -1, SQLITE_STATIC );
	rc = StepSql(pdb_content,lStmt);
	if (( rc != SQLITE_DONE )&&( rc != SQLITE_ROW ))
	{
		goto bad;
	}
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	EndTrans(pdb_content);
	return DB_OK;
bad:
	EndTrans(pdb_content);
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int update_invalid_ap_switch(int iSwitch)
 * PURPOSE  :修改非法AP扫描开关
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *	iSwitch->开关
 * RETURN   :
 *	
 * COMMENTS : 0 关闭 1打开
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int update_invalid_ap_switch(DB_CONTENT_T *pdb_content,int iSwitch)
{
	int rc = 0;
	int  idx = -1;
	sqlite3_stmt    *lStmt = NULL;
	BeginTrans(pdb_content);
	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, UPDATE_INVALID_AP_SWITCH) )
	{
		goto bad;
	}
	
	idx = sqlite3_bind_parameter_index( lStmt, ":flag" );
	sqlite3_bind_int( lStmt, idx, iSwitch);

	rc = StepSql(pdb_content,lStmt);
	if (( rc != SQLITE_DONE )&&( rc != SQLITE_ROW ))
	{
		goto bad;
	}

	sqlite3_finalize( lStmt );
	lStmt = NULL;
	EndTrans(pdb_content);
	return DB_OK;
bad:
	EndTrans(pdb_content);
	return DB_ERROR;	
}



/*==========================================================================*
 * FUNCTION :int get_invalid_ap_switch(void)
 * PURPOSE  :获取非法AP扫描开关
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : -1 失败 0 关闭 1 打开
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
  int get_invalid_ap_switch(DB_CONTENT_T *pdb_content)
{
	int rc = 0;
	int  idx = -1;
	int iFlag;
	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rDBConn, &lStmt, GET_INVALID_AP_SWITCH) )
	{
		goto bad;
	}
	if(StepSql(pdb_content,lStmt) == SQLITE_ROW)
	{
		iFlag=sqlite3_column_int(lStmt, 0);
	} 

	 sqlite3_finalize( lStmt );
	 lStmt = NULL;
	 return iFlag;
bad:
	return iFlag;
		
}


/*==========================================================================*
 * FUNCTION :int insert_no_group_ap_item(DB_CONTENT_T *pdb_content,AP_ITEM_T pAP)
 * PURPOSE  :增加未分组AP
 *	 
 *PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
 int insert_no_group_ap_item(DB_CONTENT_T *pdb_content,INVALID_AP_ITEM_T pAP)
{
	int i = 0;
	int rc = 0;
	int iCount = 0;
	int  idx = -1;
	sqlite3_stmt    *lStmt = NULL;
	CHECK_INFO_T checkInfo;
	strcpy(checkInfo.mac,pAP.mac);
	iCount =    check_ap_config(pdb_content,&checkInfo);
	if (iCount > 0)
	{
		return DB_OK; 
	}
	BeginTrans(pdb_content);
	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, INSERT_NO_GROUP_AP_ITEM) )
	{
		goto bad;
	}

	idx = sqlite3_bind_parameter_index( lStmt, ":mac" );
	sqlite3_bind_text( lStmt, idx, pAP.mac, -1, SQLITE_STATIC );
	
	rc = StepSql(pdb_content,lStmt);
	if (( rc != SQLITE_DONE )&&( rc != SQLITE_ROW ))
	{
		goto bad;
	}
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	EndTrans(pdb_content);
	return DB_OK;
 bad:
 	EndTrans(pdb_content);
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int insert_invalid_ap_item(INVALID_AP_ITEM_T pAP)
 * PURPOSE  :增加非法AP列表
 *	 
 *PARAMETERS:
 *	pdb_content->上下文
 *	pAP->非法AP信息
 *
 * RETURN   :
 *	
 * COMMENTS : 0 关闭 1打开
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int insert_invalid_ap_item(DB_CONTENT_T *pdb_content,INVALID_AP_ITEM_T pAP)
{
	int i = 0;
	int rc = 0;
	int iCount = 0;
	int  idx = -1;
	sqlite3_stmt    *lStmt = NULL;

	BeginTrans(pdb_content);
	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, INSERT_INVALID_AP_ITEM) )
	{
		goto bad;
	}
	idx = sqlite3_bind_parameter_index( lStmt, ":mac" );
	sqlite3_bind_text( lStmt, idx, pAP.mac, -1, SQLITE_STATIC );
	idx = sqlite3_bind_parameter_index( lStmt, ":bssid" );
	sqlite3_bind_text( lStmt, idx, pAP.bssid, -1, SQLITE_STATIC );
	idx = sqlite3_bind_parameter_index( lStmt, ":ssid" );
	sqlite3_bind_text( lStmt, idx, pAP.ssid, -1, SQLITE_STATIC );
	idx = sqlite3_bind_parameter_index( lStmt, ":radio_id" );
	sqlite3_bind_int( lStmt, idx,  pAP.radio_id);
	idx = sqlite3_bind_parameter_index( lStmt, ":channel" );
	sqlite3_bind_int( lStmt, idx,  pAP.channel);
	idx = sqlite3_bind_parameter_index( lStmt, ":single" );
	sqlite3_bind_int( lStmt, idx,  pAP.single);
	idx = sqlite3_bind_parameter_index( lStmt, ":snr" );
	sqlite3_bind_int( lStmt, idx,  pAP.snr);
	idx = sqlite3_bind_parameter_index( lStmt, ":rate" );
	sqlite3_bind_int( lStmt, idx,  pAP.rate);
	idx = sqlite3_bind_parameter_index( lStmt, ":ap_type" );
	sqlite3_bind_int( lStmt, idx,  pAP.ap_type);

	rc = StepSql(pdb_content,lStmt);
	if (( rc != SQLITE_DONE )&&( rc != SQLITE_ROW ))
	{
		goto bad;
	}
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	EndTrans(pdb_content);
	return DB_OK;
 bad:
 	EndTrans(pdb_content);
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_ap_policy_accord_gid(int gid)
 * PURPOSE  :获取策略，条件是策略所属的策略组在关系表中
 *	 
 *PARAMETERS:
 *	pdb_content->上下文
 *	policyItems->时间策略
 *
 * RETURN   :时间策略数目
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
 int get_policy_item(DB_CONTENT_T *pdb_content,PPOLICY_ITEM_T policyItems)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_AP_POLICY_ITEM) )
	{
		goto bad;
	}
        while( (rc = StepSql(pdb_content,lStmt)) == SQLITE_ROW ){
    		policyItems[iCount].policy_item_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].policy_group_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		strcpy(policyItems[iCount].policy_name,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		policyItems[iCount].policy_type = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].is_all_day = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].start_hour = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].start_min = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].end_hour = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].end_min = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].week_index = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].month_index = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].day_index = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;
	  	iCount += 1;
    }
    
    	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}


/*==========================================================================*
 * FUNCTION :int get_wlan_user_mode(char apMac[MAC_LEN],int wlanID)
 * PURPOSE  :根据AP MAC地址和WLAN ITEM ID获取WLAN模式
 *	 
 *PARAMETERS:
 *	pdb_content->上下文
 *	apMac->ap的MAC地址
 *    wlanID->wlan索引
 *
 * RETURN   : 0 本地 1 集中
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_wlan_user_mode(DB_CONTENT_T *pdb_content,char apMac[MAC_LEN],int wlanID)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	int iUserMode = 0;
	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_WLAN_USER_MODE) )
	{
		goto bad;
	}
	
	idx = sqlite3_bind_parameter_index( lStmt, ":id" );
	sqlite3_bind_int( lStmt, idx, wlanID);
	idx = sqlite3_bind_parameter_index( lStmt, ":mac" );
	sqlite3_bind_text( lStmt, idx, apMac, -1, SQLITE_STATIC );
	
	if(StepSql(pdb_content,lStmt)==SQLITE_ROW)
	{
		iUserMode=sqlite3_column_int(lStmt, 0);
	} 
	 sqlite3_finalize( lStmt );
	 lStmt = NULL;
    	 return iUserMode;
bad:
	return iUserMode;
}

/*==========================================================================*
 * FUNCTION :int get_ap_policy_accord_gid(DB_CONTENT_T *pdb_content,int gid,PPOLICY_ITEM_T policyItems)
 * PURPOSE  :根据组ID获取时间策略信息
 *	 
 *PARAMETERS:
 *	pdb_content->上下文
 *	gid->ap组ID
 *    policyItems->时间策略信息
 *
 * RETURN   : 
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_ap_policy_accord_gid(DB_CONTENT_T *pdb_content,int gid,PPOLICY_ITEM_T policyItems)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	int iUserMode = 0;
	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_AP_POLICY_GID) )
	{
		goto bad;
	}
			
        
    idx = sqlite3_bind_parameter_index( lStmt, ":gid" );
    sqlite3_bind_int( lStmt, idx, gid);
    
    while( StepSql(pdb_content,lStmt ) == SQLITE_ROW ) {
    		policyItems[iCount].policy_item_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].policy_group_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		strcpy(policyItems[iCount].policy_name,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		policyItems[iCount].policy_type = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].is_all_day = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].start_hour = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].start_min = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].end_hour = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].end_min = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].week_index = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].month_index = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		policyItems[iCount].day_index = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;
    	  	iCount += 1;
    }
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_ap_wlan_accord_gid(DB_CONTENT_T *pdb_content,int gid,PWLAN_ITEM_T wlanItems)
 * PURPOSE  :根据AP组ID获取AP的WLAN配置信息		 
 *	 
 *PARAMETERS:
 *	pdb_content->上下文
 *	gid->ap组索引
 *	wlanItems->wlan配置信息
 *
 * RETURN   :WLAN配置数目
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_ap_wlan_accord_gid(DB_CONTENT_T *pdb_content,int gid,PWLAN_ITEM_T wlanItems)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	int iUserMode = 0;
	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_WLAN_ITEM_APGID) )
	{
		goto bad;
	}
    
	idx = sqlite3_bind_parameter_index( lStmt, ":gid" );
	sqlite3_bind_int( lStmt, idx, gid);
    
    while( StepSql( pdb_content,lStmt ) == SQLITE_ROW ) {
		wlanItems[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].wlan_item_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].wlan_group_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].wlan_security_policy_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		get_column_text(wlanItems[iCount].ssid,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		wlanItems[iCount].broadcast_ssid = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].vlan = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].qos = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].max_user = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].mac_filter_policy = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].throughput_mode = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].download_ssid = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].download_user = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].upload_ssid = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].upload_user = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanItems[iCount].user_mode = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;
    	 	iCount += 1;
    }
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_ap_wlan_security(DB_CONTENT_T *pdb_content,int id,PWLAN_SECURITY_POLICY_T wlanSecurityPolicys)
 * PURPOSE  :根据WLAN配置项中安全策略ID,获取安全策略信息	 
 *	 
 *PARAMETERS:
 *	pdb_content->上下文
 *	gid->时间策略ID
 *	wlanSecurityPolicys->时间策略
 *
 * RETURN   :安全策略数目
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_ap_wlan_security(DB_CONTENT_T *pdb_content,int id,PWLAN_SECURITY_POLICY_T wlanSecurityPolicys)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	int iUserMode = 0;
	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_WLAN_SECURITY_POLICY) )
	{
		goto bad;
	}

    idx = sqlite3_bind_parameter_index( lStmt, ":id" );
    sqlite3_bind_int( lStmt, idx, id);
    while( StepSql( pdb_content,lStmt ) == SQLITE_ROW ) {
    		wlanSecurityPolicys[iCount].wlan_security_policy_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
		get_column_text(wlanSecurityPolicys[iCount].name,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		wlanSecurityPolicys[iCount].security_mode= (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanSecurityPolicys[iCount].auth_mode = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanSecurityPolicys[iCount].key_len = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanSecurityPolicys[iCount].key_type = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanSecurityPolicys[iCount].encrypt_type = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		wlanSecurityPolicys[iCount].default_key_index = (int)sqlite3_column_int( lStmt, columnIndex++ );
		get_column_text(wlanSecurityPolicys[iCount].key1,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		get_column_text(wlanSecurityPolicys[iCount].key2,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		get_column_text(wlanSecurityPolicys[iCount].key3,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		get_column_text(wlanSecurityPolicys[iCount].key4,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		get_column_text(wlanSecurityPolicys[iCount].psk_key,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		columnIndex = 0;
    	  	iCount += 1;
    }
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}


//AP组操作
/*==========================================================================*
 * FUNCTION :int get_ap_group_accord_policy_gid(DB_CONTENT_T *pdb_content,POLICY_ITEM_T policyItem,PAP_GROUP_INFO_T apInfos)
 * PURPOSE  :根据时间策略组ID获取AP组信息
 *	 
 * RETURN   :
 *	
 * COMMENTS :
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_ap_group_accord_policy_gid(DB_CONTENT_T *pdb_content,POLICY_ITEM_T policyItem,PAP_GROUP_INFO_T apInfos)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	int iUserMode = 0;
	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_AP_GROUP_ACCORD_POLICY_GID) )
	{
		goto bad;
	}


     idx = sqlite3_bind_parameter_index( lStmt, ":id" );
    sqlite3_bind_int( lStmt, idx, policyItem.policy_group_id);
	
    while( StepSql( pdb_content,lStmt ) == SQLITE_ROW ) {
    		apInfos[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		strcpy(apInfos[iCount].name,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		apInfos[iCount].radio_status = (int)sqlite3_column_int( lStmt, columnIndex++ );	
    	  	strcpy(apInfos[iCount].desc,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		columnIndex = 0;
	  	iCount += 1;
    }
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}


/*==========================================================================*
 * FUNCTION :int update_ap_group_status(AP_GROUP_INFO_T pAP_group)
 * PURPOSE  :根据组ID修改AP组射频状态
 *	 
 * RETURN   :
 *	
 * COMMENTS : 0 关闭 1 开启
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int update_ap_group_status(DB_CONTENT_T *pdb_content,AP_GROUP_INFO_T pAP_group)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	int iUserMode = 0;
	sqlite3_stmt    *lStmt = NULL;
	BeginTrans(pdb_content);
	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, UPDATE_AP_GROUP_STATUS) )
	{
		goto bad;
	}

	idx = sqlite3_bind_parameter_index( lStmt, ":radio_status" );
	sqlite3_bind_int( lStmt, idx, pAP_group.radio_status);
	idx = sqlite3_bind_parameter_index( lStmt, ":id" );
	sqlite3_bind_int( lStmt, idx, pAP_group.id);
	
	rc = StepSql(pdb_content,lStmt );

	if (( rc != SQLITE_DONE )&&( rc != SQLITE_ROW ))
	{
		goto bad;
	}

	sqlite3_finalize( lStmt );
	lStmt = NULL;
	EndTrans(pdb_content);
	return DB_OK;
bad:
	EndTrans(pdb_content);
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_radius_config(DB_CONTENT_T *pdb_content,PRADIUS_CONFIG_T  radiusConfigInfos)
 * PURPOSE  :获取RADIUS配置
 *	 
 * RETURN   :
 *	
 * COMMENTS :
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_radius_config(DB_CONTENT_T *pdb_content,PRADIUS_CONFIG_T  radiusConfigInfos)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	int iUserMode = 0;
	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_RADIUS_CONFIG) )
	{
		goto bad;
	}
	while( StepSql(pdb_content,lStmt ) == SQLITE_ROW ) 
	{
		radiusConfigInfos[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
		radiusConfigInfos[iCount].auth_type = (int)sqlite3_column_int( lStmt, columnIndex++ );
		radiusConfigInfos[iCount].auth_level = (int)sqlite3_column_int( lStmt, columnIndex++ );
		strcpy(radiusConfigInfos[iCount].ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		radiusConfigInfos[iCount].port = (int)sqlite3_column_int( lStmt, columnIndex++ );
		strcpy(radiusConfigInfos[iCount].pwd,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		columnIndex = 0;
		iCount += 1;
	}
    
	sqlite3_finalize( lStmt );
	lStmt=NULL;
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_ntp_config(DB_CONTENT_T *pdb_content,PNTP_CONFIG_T ntpConfigInfos)
 * PURPOSE  :获取NTP配置
 *	 
 * RETURN   :
 *	
 * COMMENTS :
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_ntp_config(DB_CONTENT_T *pdb_content,PNTP_CONFIG_T ntpConfigInfos)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	int iUserMode = 0;
	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_NTP_CONFIG) )
	{
		goto bad;
	}
    while( StepSql(pdb_content,lStmt ) == SQLITE_ROW ) {
    		ntpConfigInfos[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		ntpConfigInfos[iCount].switch_flag = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		strcpy(ntpConfigInfos[iCount].ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		columnIndex = 0;
	   	iCount += 1;
    }
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}


/*==========================================================================*
 * FUNCTION :int get_download_config(void)
 * PURPOSE  :获取当前下载配置
 *	 
 * RETURN   :当前下载配置条目
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
 int get_download_config(DB_CONTENT_T *pdb_content,PDOWNLOAD_CONFIG_T downloadConfigInfos )
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_DOWNLOAD_CONFIG) )
	{
		goto bad;
	}
	while( StepSql(pdb_content, lStmt ) == SQLITE_ROW ) {
		downloadConfigInfos[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
		strcpy(downloadConfigInfos[iCount].ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		downloadConfigInfos[iCount].port = (int)sqlite3_column_int( lStmt, columnIndex++ );
		strcpy(downloadConfigInfos[iCount].username,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		strcpy(downloadConfigInfos[iCount].pwd,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		strcpy(downloadConfigInfos[iCount].version_name,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		columnIndex=0;
		iCount += 1;
	}
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}


/*==========================================================================*
 * FUNCTION :int get_as_config(DB_CONTENT_T *pdb_content,PAS_CONFIG_T asConfigInfos)
 * PURPOSE  :获取AS配置
 *	 
 * RETURN   :返回当前AS配置
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_as_config(DB_CONTENT_T *pdb_content,PAS_CONFIG_T asConfigInfos)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	int iUserMode = 0;
	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_AS_CONFIG) )
	{
		goto bad;
	}
     while( StepSql(pdb_content, lStmt ) == SQLITE_ROW ) {
    		asConfigInfos[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
		strcpy(asConfigInfos[iCount].ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		asConfigInfos[iCount].port = (int)sqlite3_column_int( lStmt, columnIndex++ );
		strcpy(asConfigInfos[iCount].server_cert_name,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		strcpy(asConfigInfos[iCount].ap_cert_name,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		strcpy(asConfigInfos[iCount].root_cert_name,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		strcpy(asConfigInfos[iCount].cert_pwd,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		columnIndex = 0;
    	  	iCount += 1;
   	 }
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}


/*==========================================================================*
 * FUNCTION :int get_snmp_config(DB_CONTENT_T *pdb_content,PSNMP_CONFIG_T snmpConfigInfos)
 * PURPOSE  :获取SNMP配置
 *	 
 * RETURN   :返回当前SNMP配置
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_snmp_config(DB_CONTENT_T *pdb_content,PSNMP_CONFIG_T snmpConfigInfos)

{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;

	sqlite3_stmt    *lStmt = NULL;
	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_SNMP_CONFIG) )
	{
		goto bad;
	}
    while( StepSql(pdb_content, lStmt ) == SQLITE_ROW ) {
    		snmpConfigInfos[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		strcpy(snmpConfigInfos[iCount].snmp_read_pw,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		strcpy(snmpConfigInfos[iCount].snmp_read_write_pw,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		strcpy(snmpConfigInfos[iCount].trap_ip1,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		strcpy(snmpConfigInfos[iCount].trap_ip2,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		strcpy(snmpConfigInfos[iCount].trap_pw,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		snmpConfigInfos[iCount].snmp_read_write_port = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		snmpConfigInfos[iCount].trap_port = (int)sqlite3_column_int( lStmt, columnIndex++ );
		iCount += 1;
		columnIndex = 0;
    }
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}

int check_ap_mac(DB_CONTENT_T *pdb_content,PCHECK_INFO_T pCheckInfo)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_AP_ITEM_COUNT_MAC) )
	{
		goto bad;
	}
	
	idx = sqlite3_bind_parameter_index( lStmt, ":mac" );
	sqlite3_bind_text( lStmt, idx, pCheckInfo->mac, -1, SQLITE_STATIC );
	

	if(StepSql(pdb_content,lStmt) == SQLITE_ROW)
	{
		iCount=sqlite3_column_int(lStmt, 0);
	} 
	 sqlite3_finalize( lStmt );
	 lStmt = NULL;
    	 return iCount;
		  
	bad:
	return DB_ERROR;
		
}

int check_ap_version(DB_CONTENT_T *pdb_content,PCHECK_INFO_T pCheckInfo)
{
	char sqlbuf[MAX_SQL_LEN];
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;
	
	snprintf(sqlbuf,MAX_SQL_LEN,QUERY_VERSION_INFO_CON,pCheckInfo->hardware_version,pCheckInfo->manufacturer,pCheckInfo->device_model);
	
	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, sqlbuf) )
	{
		goto bad;
	}
	 
	 if(StepSql(pdb_content,lStmt)==SQLITE_ROW)
        {
                iCount=sqlite3_column_int(lStmt, 0);
        } 
	 sqlite3_finalize( lStmt );
	 lStmt = NULL;
  	  return iCount;
bad:
	return DB_ERROR;
}
/*==========================================================================*
 * FUNCTION :int check_ap_config(PCHECK_INFO_T pCheckInfo)
 * PURPOSE  :根据MAC,硬件版本信息，厂商信息和设备型号判断当前AP是否合法		 
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *    pCheckInfo->检查信息
 * RETURN   :0 不存在 !0 代表存在
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int check_ap_config(DB_CONTENT_T *pdb_content,PCHECK_INFO_T pCheckInfo)
{
	int ret =  check_ap_mac(pdb_content,pCheckInfo);
	return ret;
}

/*==========================================================================*
 * FUNCTION :int get_version_config(DB_CONTENT_T *pdb_content,PVERSION_INFO_T VersionInfos)
 * PURPOSE  :获取AP版本信息
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :VersionInfos返回版本信息
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_version_config(DB_CONTENT_T *pdb_content,PVERSION_INFO_T VersionInfos)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_VERSION_INFO) )
	{
		goto bad;
	}
	
    while( StepSql( pdb_content,lStmt ) == SQLITE_ROW ) {
    		VersionInfos[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
		get_column_text(VersionInfos[iCount].hardware_version,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		get_column_text(VersionInfos[iCount].manufacturer,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		get_column_text(VersionInfos[iCount].device_model,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		columnIndex = 0;
		iCount += 1;
    }
    
	sqlite3_finalize( lStmt );
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_hotbak_config(DB_CONTENT_T *pdb_content,PHOTBAK_CONFIG_T HotbakConfigs)
 * PURPOSE  :返回热备信息
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :HotbakConfigs配置信息
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_hotbak_config(DB_CONTENT_T *pdb_content,PHOTBAK_CONFIG_T HotbakConfigs)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_HOTBAK_CONFIG) )
	{
		goto bad;
	}
	
    while( StepSql(pdb_content, lStmt ) == SQLITE_ROW ) {
    		HotbakConfigs[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		strcpy(HotbakConfigs[iCount].start_ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		strcpy(HotbakConfigs[iCount].end_ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		strcpy(HotbakConfigs[iCount].manage_ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		strcpy(HotbakConfigs[iCount].bak_manage_ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		columnIndex = 0;
    	  	iCount += 1;
    }
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_ntp_server_config(DB_CONTENT_T *pdb_content,PNTP_SERVER_CONFIG_T NtpServerConfigs)
 * PURPOSE  :返回NTP服务器配置信息
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :NTP服务器配置信息
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_ntp_server_config(DB_CONTENT_T *pdb_content,PNTP_SERVER_CONFIG_T NtpServerConfigs)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_NTP_SERVER_CONFIG) )
	{
		goto bad;
	}
	
    while( StepSql(pdb_content,lStmt ) == SQLITE_ROW ) {
    		NtpServerConfigs[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		NtpServerConfigs[iCount].switch_flag = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;
	    	iCount += 1;
    }
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}



/*==========================================================================*
 * FUNCTION :int get_ntp_client_config(DB_CONTENT_T *pdb_content,PNTP_CLIENT_CONFIG_T NtpClientConfigs)
 * PURPOSE  :返回NTP客户端配置信息
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :NTP客户端配置信息
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_ntp_client_config(DB_CONTENT_T *pdb_content,PNTP_CLIENT_CONFIG_T NtpClientConfigs)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_NTP_CLIENT_CONFIG) )
	{
		goto bad;
	}

	while( StepSql(pdb_content,lStmt ) == SQLITE_ROW ) {
		NtpClientConfigs[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
		NtpClientConfigs[iCount].switch_flag = (int)sqlite3_column_int( lStmt, columnIndex++ );
		strcpy(NtpClientConfigs[iCount].ip1,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		NtpClientConfigs[iCount].syn_time = (int)sqlite3_column_int( lStmt, columnIndex++ );
		NtpClientConfigs[iCount].flag = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;
		iCount += 1;  		
	}
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}



/*==========================================================================*
 * FUNCTION :int get_route_config(DB_CONTENT_T *pdb_content,PROUTE_CONFIG_T  RouteConfigs)
 * PURPOSE  :返回路由配置信息
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_route_config(DB_CONTENT_T *pdb_content,PROUTE_CONFIG_T  RouteConfigs)
{
		int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_ROUTE_CONFIG) )
	{
		goto bad;
	}
	
    while( StepSql(pdb_content, lStmt ) == SQLITE_ROW ) {
    		RouteConfigs[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		strcpy(RouteConfigs[iCount].dest_ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		strcpy(RouteConfigs[iCount].netmask,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		strcpy(RouteConfigs[iCount].next_hop,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		columnIndex = 0;
    	 	 iCount += 1;
    }
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}


/*==========================================================================*
 * FUNCTION :int get_syslog_config(DB_CONTENT_T *pdb_content,PSYSLOG_CONFIG_T SyslogConfigs)
 * PURPOSE  :返回系统日志
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_syslog_config(DB_CONTENT_T *pdb_content,PSYSLOG_CONFIG_T SyslogConfigs)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_SYSLOG_CONFIG) )
	{
		goto bad;
	}
	
    while( StepSql( pdb_content,lStmt ) == SQLITE_ROW ) {
    		SyslogConfigs[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		SyslogConfigs[iCount].level = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		strcpy(SyslogConfigs[iCount].ip1,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		strcpy(SyslogConfigs[iCount].ip2,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		SyslogConfigs[iCount].ip1_port = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		SyslogConfigs[iCount].ip2_port = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;	
	   	iCount += 1;
    }
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_net_info(DB_CONTENT_T *pdb_content,PNET_INFO_T NetInfos)
 * PURPOSE  :获取网口配置信息
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_net_info(DB_CONTENT_T *pdb_content,PNET_INFO_T NetInfos)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_NET_INFO) )
	{
		goto bad;
	}
	
	while( StepSql(pdb_content, lStmt ) == SQLITE_ROW ) {
    		NetInfos[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
		get_column_text(NetInfos[iCount].net_name,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		get_column_text(NetInfos[iCount].ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		get_column_text(NetInfos[iCount].netmask,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		get_column_text(NetInfos[iCount].gateway,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		columnIndex = 0;	
		iCount += 1;
    	}

	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}


/*==========================================================================*
 * FUNCTION :int get_mac_filter(DB_CONTENT_T *pdb_content,PMAC_FILTER_T MacFilters)
 * PURPOSE  :MAC地址过滤
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_mac_filter(DB_CONTENT_T *pdb_content,PMAC_FILTER_T MacFilters)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_MAC_FILTER) )
	{
		goto bad;
	}
	while( StepSql( pdb_content,lStmt ) == SQLITE_ROW ) {
		MacFilters[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
		strcpy(MacFilters[iCount].mac,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		columnIndex = 0;	
		iCount += 1;
	}
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}


/*==========================================================================*
 * FUNCTION :int get_radio_base(DB_CONTENT_T *pdb_content,PRADIO_BASE_T Radio_bases)
 * PURPOSE  :获取射频配置信息
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_radio_base(DB_CONTENT_T *pdb_content,PRADIO_BASE_T Radio_bases)
{
		int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_RADIO_BASE) )
	{
		goto bad;
	}
    while( StepSql( pdb_content,lStmt ) == SQLITE_ROW ) {
    		Radio_bases[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		Radio_bases[iCount].radio_id= (int)sqlite3_column_int( lStmt, columnIndex++ );
    		Radio_bases[iCount].switch_flag= (int)sqlite3_column_int( lStmt, columnIndex++ );
    		Radio_bases[iCount].power_mode= (int)sqlite3_column_int( lStmt, columnIndex++ );
    		Radio_bases[iCount].power= (int)sqlite3_column_int( lStmt, columnIndex++ );
    		Radio_bases[iCount].mode= (int)sqlite3_column_int( lStmt, columnIndex++ );
    		Radio_bases[iCount].rate= (int)sqlite3_column_int( lStmt, columnIndex++ );
    		Radio_bases[iCount].n_rate = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		Radio_bases[iCount].mask= (int)sqlite3_column_int( lStmt, columnIndex++ );
    		Radio_bases[iCount].channel_width= (int)sqlite3_column_int( lStmt, columnIndex++ );
    		Radio_bases[iCount].guard_interval= (int)sqlite3_column_int( lStmt, columnIndex++ );
    		Radio_bases[iCount].MPDU = (int)sqlite3_column_int( lStmt, columnIndex++ );
		Radio_bases[iCount].MSDU = (int)sqlite3_column_int( lStmt, columnIndex++ );
		Radio_bases[iCount].n_mode = (int)sqlite3_column_int( lStmt, columnIndex++ );
		iCount += 1;
		columnIndex = 0;	
    }
   
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_radio_advance(DB_CONTENT_T *pdb_content,PRADIO_ADVANCE_T RadioAdvances)
 * PURPOSE  :获取射频高级配置
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_radio_advance(DB_CONTENT_T *pdb_content,PRADIO_ADVANCE_T RadioAdvances)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rDBConn, &lStmt, QUERY_RADIO_ADVANCE) )
	{
		goto bad;
	}
    while( StepSql( pdb_content,lStmt ) == SQLITE_ROW ) {
    		RadioAdvances[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		RadioAdvances[iCount].isoliate = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		RadioAdvances[iCount].igmp_snooping = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		RadioAdvances[iCount].pre_auth = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		RadioAdvances[iCount].up_link_check = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		RadioAdvances[iCount].action = (int)sqlite3_column_int( lStmt, columnIndex++ );
		get_column_text(RadioAdvances[iCount].action_ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
		get_column_text(RadioAdvances[iCount].ntp_ip,(const char*)sqlite3_column_text( lStmt, columnIndex++ ));
    		RadioAdvances[iCount].syn_time = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;
    	 	iCount += 1;
    }
    	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}



/*==========================================================================*
 * FUNCTION :int get_scan_config(DB_CONTENT_T *pdb_content,PSCAN_CONFIG_T ScanConfigs )
 * PURPOSE  :获取背景扫描配置
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_scan_config(DB_CONTENT_T *pdb_content,PSCAN_CONFIG_T ScanConfigs )
{
		int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_SCAN_CONFIG) )
	{
		goto bad;
	}
    while( StepSql(pdb_content, lStmt ) == SQLITE_ROW ) {
    		ScanConfigs[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		ScanConfigs[iCount].chan = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		ScanConfigs[iCount].switch_flag = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		ScanConfigs[iCount].period = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;			
		iCount += 1;
    }
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}


/*==========================================================================*
 * FUNCTION :int get_chan_config(DB_CONTENT_T *pdb_content,PCHAN_CONFIG_T ChanConfigs)
 * PURPOSE  :获取信道配置
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_chan_config(DB_CONTENT_T *pdb_content,PCHAN_CONFIG_T ChanConfigs)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_CHAN_CONFIG) )
	{
		goto bad;
	}
    while( StepSql(pdb_content, lStmt ) == SQLITE_ROW ) {
    		ChanConfigs[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		ChanConfigs[iCount].radio_id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		ChanConfigs[iCount].chan_auto = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		ChanConfigs[iCount].chan_auto_mode = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		ChanConfigs[iCount].chan_auto_time = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		ChanConfigs[iCount].min_rssi = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;
    	       iCount += 1;
    }
    	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_capwap_timer_config(DB_CONTENT_T *pdb_content,PCAPWAP_TIMER_CONFIG_T CapwapTimerConfigs)
 * PURPOSE  :CAPWAP保活
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_capwap_timer_config(DB_CONTENT_T *pdb_content,PCAPWAP_TIMER_CONFIG_T CapwapTimerConfigs)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_CAPWAP_TIMER_CONFIG) )
	{
		goto bad;
	}
    while( StepSql(pdb_content, lStmt ) == SQLITE_ROW ) {
    		CapwapTimerConfigs[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		CapwapTimerConfigs[iCount].keep_alive = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		CapwapTimerConfigs[iCount].broadcast_period = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;	
		iCount += 1;
    }
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int get_load_balance(DB_CONTENT_T *pdb_content,PLOAD_BALANCE_T LoadBalances)
 * PURPOSE  :负载均恒
 *	 
 * PARAMETERS:
 *	pdb_content->上下文
 *
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_load_balance(DB_CONTENT_T *pdb_content,PLOAD_BALANCE_T LoadBalances)
{
		int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, QUERY_LOAD_BALANCE) )
	{
		goto bad;
	}
    while( StepSql(pdb_content, lStmt ) == SQLITE_ROW ) {
    		LoadBalances[iCount].id = (int)sqlite3_column_int( lStmt, columnIndex++);
    		LoadBalances[iCount].switch_flag = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		LoadBalances[iCount].balance_type = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		LoadBalances[iCount].user_start_max = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		LoadBalances[iCount].user_diff = (int)sqlite3_column_int( lStmt, columnIndex++ );
    		LoadBalances[iCount].user_max = (int)sqlite3_column_int( lStmt, columnIndex++ );
		columnIndex = 0;
 	        iCount += 1;
    }

	sqlite3_finalize( lStmt );
	lStmt = NULL;
	return iCount;
bad:
	return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int check_sta_mac(DB_CONTENT_T *pdb_content,const char *pmac)
 * PURPOSE  :检查终端MAC地址是否存在
 *	 
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int check_sta_mac(DB_CONTENT_T *pdb_content,STA_ITEM_T psta_item)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, CHECK_STA_ITEM) )
	{
		goto bad;
	}
	
	idx = sqlite3_bind_parameter_index( lStmt, ":mac" );
	sqlite3_bind_text( lStmt, idx, psta_item.mac, -1, SQLITE_STATIC );
	

	if(StepSql(pdb_content,lStmt) == SQLITE_ROW)
	{
		iCount=sqlite3_column_int(lStmt, 0);
	} 
	 sqlite3_finalize( lStmt );
	 lStmt = NULL;
    	 return iCount;
		  
	bad:
	return DB_ERROR;
		
}


/*==========================================================================*
 * FUNCTION :int update_sta_item(DB_CONTENT_T *pdb_content,STA_ITEM_T psta_item)
 * PURPOSE  :修改终端在线时长
 *	 
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int update_sta_item(DB_CONTENT_T *pdb_content,STA_ITEM_T psta_item)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;
	
	BeginTrans(pdb_content);
	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, UPDATE_STA_ITEM) )
	{
		goto bad;
	}
	idx = sqlite3_bind_parameter_index( lStmt, ":mac" );
	sqlite3_bind_text( lStmt, idx, psta_item.mac, -1, SQLITE_STATIC );
    rc = StepSql(pdb_content,lStmt );
    if (( rc != SQLITE_DONE )&&( rc != SQLITE_ROW ))
    {
    	goto bad;
    }
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	EndTrans(pdb_content);
	return DB_OK;
 bad:
 		EndTrans(pdb_content);
		return DB_ERROR;
}


/*==========================================================================*
 * FUNCTION :int insert_sta_item(STA_ITEM_T psta_item)
 * PURPOSE  :增加终端
 *	 
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int insert_sta_item(DB_CONTENT_T *pdb_content,STA_ITEM_T psta_item)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;

	if (check_sta_mac(pdb_content,psta_item) > 0)
	{
		return update_sta_item(pdb_content,psta_item);
	}
	BeginTrans(pdb_content);
	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, INSERT_STA_ITEM) )
	{
		goto bad;
	}
	idx = sqlite3_bind_parameter_index( lStmt, ":ap_mac" );
	sqlite3_bind_text( lStmt, idx, psta_item.ap_mac, -1, SQLITE_STATIC );
	idx = sqlite3_bind_parameter_index( lStmt, ":bssid" );
	sqlite3_bind_text( lStmt, idx, psta_item.bssid, -1, SQLITE_STATIC );
	idx = sqlite3_bind_parameter_index( lStmt, ":mac" );
	sqlite3_bind_text( lStmt, idx, psta_item.mac, -1, SQLITE_STATIC );
	idx = sqlite3_bind_parameter_index( lStmt, ":ssid" );
	sqlite3_bind_text( lStmt, idx, psta_item.ssid, -1, SQLITE_STATIC );
   

    rc = StepSql(pdb_content,lStmt );
    if (( rc != SQLITE_DONE )&&( rc != SQLITE_ROW ))
    {
    	goto bad;
    }
    
	sqlite3_finalize( lStmt );
	lStmt = NULL;
	EndTrans(pdb_content);
	return DB_OK;
 bad:
 		EndTrans(pdb_content);
		return DB_ERROR;
}

/*==========================================================================*
 * FUNCTION :int delete_sta_item(STA_ITEM_T psta_item)
 * PURPOSE  :删除终端
 *	 
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  方志军              DATE: 2011-02-15 15:47
 *==========================================================================*/
int delete_sta_item(DB_CONTENT_T *pdb_content,STA_ITEM_T psta_item)
{
	int rc = 0;
	int  idx = -1;
	int columnIndex = 0;
	int iCount = 0;
	sqlite3_stmt    *lStmt = NULL;
	BeginTrans(pdb_content);
	if( !PrepareSql(pdb_content->rwDBConn, &lStmt, DELETE_STA_ITEM) )
	{
		goto bad;
	}

	idx = sqlite3_bind_parameter_index( lStmt, ":mac" );
	sqlite3_bind_text( lStmt, idx, psta_item.mac, -1, SQLITE_STATIC );


	rc = StepSql(pdb_content, lStmt );
	if (( rc != SQLITE_DONE )&&( rc != SQLITE_ROW ))
	{
		goto bad;
	}

	sqlite3_finalize( lStmt );
	lStmt = NULL;
	EndTrans(pdb_content);
	return DB_OK;
bad:
	EndTrans(pdb_content);
	return DB_ERROR;
}


