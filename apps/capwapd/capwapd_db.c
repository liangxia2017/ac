
 #include "includes.h"
/*12小时确认一次当前ap数是否超过license数限制，否则停止工作*/
 void check_licences(void *eloop_data, void *user_ctx,short int ap_id,unsigned int l2tp_id)
 {
 	 int ret,license ;
 	sqlite3_exec(pFile,"select count(*) from ap_info where ap_group_name !='unknown') ",get_db_value,&ret,NULL);
	license = system("/ac/sbin/readblock -l /ac/config/license.bin");
	if(license >= 255 || license < 1)
		license = 1;
	/*允许ap数为license数的16倍*/
	while(license*16 < ret)
	{
		print_info("ap number are more than license !");
		sleep(180);
		sqlite3_exec(pFile,"select count(*) from ap_info where ap_group_name !='unknown') ",get_db_value,&ret,NULL);
		license = system("/ac/sbin/readblock -l /ac/config/license.bin");
		if(license >= 255 || license < 1)
			license = 1;	
		if(license*16 >= ret)
			break;
	}
	eloop_register_timeout(43200, 0, check_licences, NULL, NULL,NULL);
 }

 int init_db()
 {
 	 int ret;
	char *tmp_str = "/ac/db/ac.s3db";
	char *sql_str = NULL;
	ret = sqlite3_open_v2(tmp_str, &pFile, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX,NULL); 
	if(ret != SQLITE_OK)
	{
		sqlite3_close(pFile);
		return 1;
	}
	sqlite3_busy_timeout(pFile,60);/*防止忙时写入，延迟ms再写*/
	sqlite3_exec(pFile,"update ap_info set status=2 where status=1 ",0,0,NULL);
	//eloop_register_timeout(43200, 0, check_licences, NULL, NULL,NULL);
	return 0;  
 }
int get_db_value(void *arg, int col_num, char **col_value, char **col_name) 
{
   	if(col_value == NULL)
		return 0;
   //	*(int *)arg=atoi(col_value[0]);
   	*(int *)arg = col_value[0]==NULL ? 0:atoi(col_value[0]);
		return 0;
}

int get_old_report_value(void *arg, int col_num, char **col_value, char **col_name) 
{
	struct ap_report *report  = (struct ap_report *)arg;
   	if(col_value == NULL)
		return 0;
   	report->eth_re=atoi(col_value[2]);
	report->eth_se=atoi(col_value[3]);
	report->wifi_re=atoi(col_value[4]);
	report->wifi_se=atoi(col_value[5]);
	report->lte_re=atoi(col_value[6]);
	report->lte_se=atoi(col_value[7]);
	return 0;
}
int db_join(void *arg, int col_num, char **col_value, char **col_name)  
{
	struct ap_node *node  = (struct ap_node *)arg;
	int ret= -1;
	char sql_str[512] = {0};
	
	char *msg_buf = node->rsp_buf;
	msg_buf += node->rsp_buf_len;/*先跳过capwap头和msg头*/
	*(short int *)msg_buf = htons(33);
	msg_buf += 2;node->rsp_buf_len +=2;
	*(short int *)msg_buf = htons(4);
	msg_buf += 2;node->rsp_buf_len +=2;

	if(atoi(col_value[4]) == 0)
	{/*新加入的ap，考虑放入到unkown组*/
		sqlite3_exec(pFile,"select count(*) from ap_info where ap_group_name='unknown'",get_db_value,&ret,0);
		if(ret >= 32)
		{
			*(int *)msg_buf = htonl(6);
			msg_buf += 4;node->rsp_buf_len +=4;
			return 0;
		}
		sprintf(sql_str,"insert or replace into ap_info (ap_mac,ap_ip,ap_group_name,status,last_join_time,config_mask,soft_ver) \
		values ('%s','%s','unknown',1,datetime('now','localtime'),0,'%s')",node->apmac,inet_ntoa(node->ap_from->sin_addr),node->soft_ver);
		ret = sqlite3_exec(pFile,sql_str,0,0,0);
		*(int *)msg_buf = htonl(0);
		msg_buf += 4;node->rsp_buf_len +=4;
		return 0;
	}
	
	sprintf(sql_str, "update ap_info set ap_ip='%s',status=1,last_join_time=datetime('now','localtime'),\
		config_mask=65535,soft_ver='%s' where id=%d ", inet_ntoa(node->ap_from->sin_addr),node->soft_ver,atoi(col_value[0]));
	ret = sqlite3_exec(pFile,sql_str,0,0,0);
	sprintf(sql_str,"mkdir -p /opt/micro_ac/ap_log/%s;/ac/script/ap_log %s up",node->apmac,node->apmac);
	system(sql_str);
	*(int *)msg_buf = htonl(0);
	msg_buf += 4;node->rsp_buf_len +=4;
	return 0;	
}
int pack_wlan_id_security(void *arg, int col_num, char **col_value, char **col_name) 
{
	struct ap_node *node  = (struct ap_node *)arg;
	int ret= -1;
	short tmp;
	char sql_str[512] = {0};
	char *msg_buf = node->rsp_buf;
	msg_buf += node->rsp_buf_len;/*先跳过capwap头和msg头*/
	ret = (col_value[2] != NULL )? atoi(col_value[2]):0;
	switch(ret)
	{
		case 1:/*psk*/
			*(char *)msg_buf = 1;
			msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
			*(char *)msg_buf = (col_value[3] != NULL )? atoi(col_value[3]):0;
			msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
			tmp=strlen(col_value[4]);
			*(short *)msg_buf = htons(tmp);
			msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
			strcpy(msg_buf,col_value[4]);
			msg_buf += tmp;node->rsp_buf_len += tmp;node->subid_len +=tmp;node->id_len +=tmp;
			break;
		
		case 2:/*eap*/
			*(char *)msg_buf = 2;
			msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
			*(char *)msg_buf = col_value[3]==NULL ? 0:atoi(col_value[3]);
			msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
			*(int *)msg_buf = htonl(inet_addr(col_value[5]));
			msg_buf += 4;node->rsp_buf_len += 4;node->subid_len +=4;node->id_len +=4;	
			*(short *)msg_buf = htons(col_value[6]==NULL ? 0:atoi(col_value[6]));
			msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
			*(int *)msg_buf = htonl(inet_addr(col_value[7]));
			msg_buf += 4;node->rsp_buf_len += 4;node->subid_len +=4;node->id_len +=4;
                        if(col_value[8] != NULL)	
			  *(short *)msg_buf = htons(atoi(col_value[8]));
                        else
                          *(short *)msg_buf = 0;
			msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;	
			
			tmp=strlen(col_value[9]);
			*(short *)msg_buf = htons(tmp);
			msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
			strcpy(msg_buf,col_value[9]);
			msg_buf += tmp;node->rsp_buf_len += tmp;node->subid_len +=tmp;node->id_len +=tmp;
			break;
		default:/*open*/
			*(char *)msg_buf=0;
			msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len+=1;
			break;
	}		
	return 0;
}
int pack_wlan_id(void *arg, int col_num, char **col_value, char **col_name)  
{
	struct ap_node *node  = (struct ap_node *)arg;
	int ret= -1;
	short tmp;
	char sql_str[512] = {0};
	char *subid_len;/*统计subid长度*/
	
	node->subid_len = 0;
	char *msg_buf = node->rsp_buf;
	msg_buf += node->rsp_buf_len;
	
	*(char *)msg_buf=atoi(col_value[2]);
	msg_buf += 1;node->rsp_buf_len += 1;node->id_len +=1;
	subid_len = msg_buf;
	msg_buf += 1;node->rsp_buf_len += 1;node->id_len +=1;
	tmp = atoi(col_value[12]);
	*(char *)msg_buf = tmp;/*集中转发模式*/
	node->forward_flag = node->forward_flag + tmp;/*后面会判断flag，若为1则要为该ap生成隧道接口*/
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;/*算上一个保留的字节*/
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;/*跳过保留字段*/
	*(char *)msg_buf=atoi(col_value[3]);
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf=atoi(col_value[4]);
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(short *)msg_buf=htons(atoi(col_value[5]));/*vlan id*/
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	*(short *)msg_buf=htons(atoi(col_value[6]));
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	*(short *)msg_buf=htons(atoi(col_value[7]));
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len+=2;
	*(short *)msg_buf=htons(atoi(col_value[8]));
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	*(short *)msg_buf=htons(atoi(col_value[9]));
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	*(char *)msg_buf = strlen(col_value[10]);/*ssid长度*/
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	strcpy(msg_buf,col_value[10]);/*ssid*/
	tmp=strlen(col_value[10]);
	msg_buf += tmp;node->rsp_buf_len += tmp;node->subid_len +=tmp;node->id_len +=tmp;
	/*安全模式*/
	if(strlen(col_value[11])==0)/*值为空则为open信号*/
	{
		*(char *)msg_buf=0;
		msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len+=1;
		*(char *)subid_len = node->subid_len;
		return 0;
	}

	sprintf(sql_str,"select * from wlan_security_policy where security_policy_name \
		in(select security_policy from wlan_config where wlan_group_name \
	in(select wlan_group_name from group_relation where ap_group_name \
	in(select ap_group_name from ap_info where ap_mac='%s')))",node->apmac);
	sqlite3_exec(pFile,sql_str,pack_wlan_id_security,node,0);
	*(char *)subid_len = node->subid_len;
	
	return 0;
}
int pack_wireless_id(void *arg, int col_num, char **col_value, char **col_name)  
{
	struct ap_node *node  = (struct ap_node *)arg;
	int ret= -1,tmp,i,j;
	char sql_str[512] = {0};
	char *subid_len;

	
	char *msg_buf = node->rsp_buf;
	msg_buf += node->rsp_buf_len;
	/*先封装1、2号卡的消息*/
	for(i=1;i<3;i++)
	{
		node->subid_len = 0;/*记录radio id的长度*/
		j= (i==1) ? 0: 9;/*9为radio id的元素的个数*/
		*(char *)msg_buf = i;
		msg_buf += 1;node->rsp_buf_len += 1;node->id_len +=1;
		subid_len = msg_buf;/*预留长度，过一会赋值*/
		msg_buf += 1;node->rsp_buf_len += 1;node->id_len +=1;
		
		*(char *)msg_buf= (col_value[2+j] != NULL )? atoi(col_value[2+j]):0;
		msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
		*(char *)msg_buf= (col_value[3+j] != NULL )? atoi(col_value[3+j]):0;/* 并防止字符串为空*/
		msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
		*(char *)msg_buf=(col_value[4+j] != NULL )? atoi(col_value[4+j]):0;
		msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;	
		*(char *)msg_buf=(col_value[5+j] != NULL )? atoi(col_value[5+j]):0;
		msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;		
		*(char *)msg_buf=(col_value[6+j] != NULL )? atoi(col_value[6+j]):0;
		msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;		
		*(char *)msg_buf=(col_value[7+j] != NULL )? atoi(col_value[7+j]):0;
		msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;		
		*(char *)msg_buf=(col_value[8+j] != NULL )? atoi(col_value[8+j]):0;
		msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;		
		*(char *)msg_buf=(col_value[9+j] != NULL )? atoi(col_value[9+j]):0;
		msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
		#if 1/*目前只对radio 1配置低速率*/
		if(i==1 && col_value[10+j] != NULL)
			*(short *)msg_buf=htons(strtol(col_value[10+j],NULL,16));
		#endif
		msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
		*(char *)subid_len = node->subid_len;
	}

	/*填充wireless全局配置*/
	*(short *)msg_buf=htons((col_value[20] != NULL )? atoi(col_value[20]):0);/*帧间隔*/
	msg_buf += 2;node->rsp_buf_len += 2;node->id_len +=2;
	*(short *)msg_buf=htons((col_value[21] != NULL )? atoi(col_value[21]):0);
	msg_buf += 2;node->rsp_buf_len += 2;node->id_len +=2;
	*(char *)msg_buf=(col_value[22] != NULL )? atoi(col_value[22]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->id_len +=1;
	*(char *)msg_buf=(col_value[23] !=NULL) ? atoi(col_value[23]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->id_len +=1;	
	*(short *)msg_buf=htons((col_value[24] !=NULL) ? atoi(col_value[24]):0);/*信道周期*/
	msg_buf += 2;node->rsp_buf_len += 2;node->id_len +=2;
	*(unsigned char *)msg_buf=(col_value[25] !=NULL) ? atoi(col_value[25]):0;/*5.8G优先接入*/
	msg_buf += 1;node->rsp_buf_len += 1;node->id_len +=1;	
	*(unsigned char *)msg_buf=(col_value[26] !=NULL) ? atoi(col_value[26]):0;/*弱信号禁止接入*/
	msg_buf += 1;node->rsp_buf_len += 1;node->id_len +=1;	
	*(unsigned char *)msg_buf=(col_value[27] !=NULL) ? atoi(col_value[27]):0;/*5.2频段开关*/
	msg_buf += 1;node->rsp_buf_len += 1;node->id_len +=1;	
		
}
int pack_fuction_id(void *arg, int col_num, char **col_value, char **col_name)  
{
	struct ap_node *node  = (struct ap_node *)arg;
	int ret= -1,i,j;
	short int tmp = 0;
	short int tmp2 = 0;
	char sql_str[512] = {0};
	char *subid_len;
	
	char *msg_buf = node->rsp_buf;
	msg_buf += node->rsp_buf_len;
	#if 1/*链路完整性*/
	*(char *)msg_buf = 1;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = 5;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[2] !=NULL) ? atoi(col_value[2]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[3] !=NULL) ? atoi(col_value[3]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[4] !=NULL) ? atoi(col_value[4]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[10] !=NULL) ? atoi(col_value[10]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[25] !=NULL) ? atoi(col_value[25]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	#endif
	#if 1/*ntp*/
	*(char *)msg_buf = 2;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = 6;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[5] !=NULL) ? atoi(col_value[5]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[6] !=NULL) ? atoi(col_value[6]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(unsigned int *)msg_buf =  (col_value[6] !=NULL) ? inet_addr(col_value[7]):0;/*ntp服务器*/
	msg_buf += 4;node->rsp_buf_len += 4;node->subid_len +=4;node->id_len +=4;
	#endif
	#if 1/*定位*/
	*(char *)msg_buf = 3;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = 2;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[8] !=NULL) ? atoi(col_value[8]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[9] !=NULL) ? atoi(col_value[9]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	#endif
	#if 1/*ap重定向*/
	*(char *)msg_buf = 4;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;

	tmp = (col_value[12] !=NULL) ? strlen(col_value[12]):0;
	tmp2 = (col_value[13] !=NULL) ? strlen(col_value[13]):0;
	*(char *)msg_buf = 6 + tmp + tmp2;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[11] !=NULL) ? atoi(col_value[11]):0;
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	*(short *)msg_buf = htons((col_value[12] !=NULL) ? tmp:0);
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	if(col_value[12] !=NULL)
	{	
		strcpy(msg_buf,col_value[12]);
		msg_buf += tmp;node->rsp_buf_len += tmp;node->subid_len +=tmp;node->id_len +=tmp;
	}
	*(short *)msg_buf = htons((col_value[13] !=NULL) ? tmp2:0);
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	if(col_value[13] !=NULL)
	{	
		strcpy(msg_buf,col_value[13]);
		msg_buf += tmp2;node->rsp_buf_len += tmp2;node->subid_len +=tmp2;node->id_len +=tmp2;
	}
	#endif
	#if 1/*ap日志下发*/
	*(char *)msg_buf = 5;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = 2;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[14] !=NULL) ? atoi(col_value[14]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[15] !=NULL) ? atoi(col_value[15]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	#endif
	#if 1/*补丁命令下发*/
	*(char *)msg_buf = 6;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;

	tmp = (col_value[17] !=NULL) ? strlen(col_value[17]):0;
	*(char *)msg_buf = 4 + tmp;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[16] !=NULL) ? atoi(col_value[16]):0;
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	*(short *)msg_buf = htons((col_value[17] !=NULL) ? tmp:0);
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	if(col_value[17] !=NULL)
	{	
		strcpy(msg_buf,col_value[17]);
		msg_buf += tmp;node->rsp_buf_len += tmp;node->subid_len +=tmp;node->id_len +=tmp;
	}
	#endif
	#if 1/*dns黑白名单*/
	*(char *)msg_buf = 7;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;

	tmp = (col_value[19] !=NULL) ? strlen(col_value[19]):0;
	tmp2 = (col_value[24] !=NULL) ? strlen(col_value[24]):0;
	*(char *)msg_buf = 4 + tmp;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[18] !=NULL) ? atoi(col_value[18]):0;
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	*(short *)msg_buf = htons((col_value[19] !=NULL) ? tmp:0);
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	if(col_value[19] !=NULL)
	{	
		strcpy(msg_buf,col_value[19]);
		msg_buf += tmp;node->rsp_buf_len += tmp;node->subid_len +=tmp;node->id_len +=tmp;
	}
	
	*(short *)msg_buf = htons((col_value[24] !=NULL) ? tmp2:0);
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	if(col_value[24] !=NULL)
	{	
		strcpy(msg_buf,col_value[24]);
		msg_buf += tmp2;node->rsp_buf_len += tmp2;node->subid_len +=tmp2;node->id_len +=tmp2;
	}
	#endif
	#if 1/*内容更新*/
	*(char *)msg_buf = 8;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;

	tmp = (col_value[23] !=NULL) ? strlen(col_value[23]):0;
	*(char *)msg_buf = 6 + tmp;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[20] !=NULL) ? atoi(col_value[20]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(char *)msg_buf = (col_value[21] !=NULL) ? atoi(col_value[21]):0;
	msg_buf += 1;node->rsp_buf_len += 1;node->subid_len +=1;node->id_len +=1;
	*(short *)msg_buf = htons((col_value[22] !=NULL) ? tmp:5241);
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	*(short *)msg_buf = tmp;
	msg_buf += 2;node->rsp_buf_len += 2;node->subid_len +=2;node->id_len +=2;
	if(col_value[23] !=NULL)
	{	
		strcpy(msg_buf,col_value[23]);
		msg_buf += tmp;node->rsp_buf_len += tmp;node->subid_len +=tmp;node->id_len +=tmp;
	}
	#endif
	return 0;
}

int assemble_echo_mask_id(int mask,struct ap_node *node)
{
	#define wlan_id 4097
	#define wirless_id 4098
	#define fuction_id 4099
	int ret= -1;
	char sql_str[512] = {0};
	short int *id_len;
	char *msg_buf = node->rsp_buf;
	msg_buf += node->rsp_buf_len;
	switch (mask)
	{
		case wlan_id:
		print_info("pack_wlan_msg\n");
			*(short int *)msg_buf = htons(4097);
			msg_buf += 2;node->rsp_buf_len +=2;
			id_len = (short *)msg_buf;
			msg_buf += 2;node->rsp_buf_len +=2;
			
			sprintf(sql_str,"select * from wlan_config where wlan_group_name \
				in(select wlan_group_name from group_relation where ap_group_name \
				in(select ap_group_name from ap_info where ap_mac='%s'))",node->apmac);
			sqlite3_exec(pFile,sql_str,pack_wlan_id,node,0);
			*id_len = htons(node->id_len);
			node->id_len = 0;
			break;
		case wirless_id:
			print_info("pack_wirelss_msg\n");
			*(short int *)msg_buf = htons(4098);
			msg_buf += 2;node->rsp_buf_len +=2;
			id_len = (short *)msg_buf;
			msg_buf += 2;node->rsp_buf_len +=2;

			sprintf(sql_str,"select * from wireless_config where wireless_group_name \
				in(select wireless_group_name from group_relation where ap_group_name \
				in(select ap_group_name from ap_info where ap_mac='%s'))",node->apmac);
					
			sqlite3_exec(pFile,sql_str,pack_wireless_id,node,0);
			*id_len = htons(node->id_len);
			node->id_len = 0;
			break;
		case fuction_id:
				print_info("pack_fuction_msg\n");
			*(short int *)msg_buf = htons(4099);
			msg_buf += 2;node->rsp_buf_len +=2;
			id_len = (short *)msg_buf;
			msg_buf += 2;node->rsp_buf_len +=2;

			sprintf(sql_str,"select * from func_config where function_group_name \
				in(select function_group_name from group_relation where ap_group_name \
				in(select ap_group_name from ap_info where ap_mac='%s'))",node->apmac);
			sqlite3_exec(pFile,sql_str,pack_fuction_id,node,0);
			
			*id_len = htons(node->id_len);
			node->id_len = 0;
			break;
		default:
			return 0;
	}
	return 0;
}

int clean_tunnel(void *arg, int col_num, char **col_value, char **col_name) 
{
	char sql_str[512] = {0};
	unsigned int ret = *(unsigned int *)arg;
	if(atoi(col_value[0]) == 1)
	{
	sprintf(sql_str, "/usr/sbin/ip l2tp del tunnel tunnel_id %u",ret);
	system(sql_str);
}
	return 0;
}

int ap_down_hook(void *arg, int col_num, char **col_value, char **col_name) 
{
	char sql_str[512] = {0};
	unsigned int ret = *(unsigned int *)arg;
	sprintf(sql_str, "delete from sta_list_assc where assc_ap_mac='%s'", col_value[0]);
	sqlite3_exec(pFile,sql_str,0,0,0);
	
	sprintf(sql_str, "select forward_mode from wlan_config where wlan_group_name \
	in (select wlan_group_name from group_relation where ap_group_name='%s')", col_value[1]);
	sqlite3_exec(pFile,sql_str,clean_tunnel,&ret,0);
	//memset(sql_str,0,sizeof(sql_str));
	sprintf(sql_str, "/ac/script/ap_log %s idle",  col_value[0]);
	system(sql_str);
	return 0;
}
	

