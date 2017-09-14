/*add by xialiang,20140811*/
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

#include "includes.h"


/*
向buf填充capwap头和msg 头

buf:指向element id的起始地址
msg_type:要填充的msg type
len:所有element 的总长度
*/
int fill_head(void * buf,int msg_type,short int len)
{
	int tmp;
	if (!buf)
		return -1;
	char *msg_offset = buf;
	memset(msg_offset,0,ac_runtime.capwap_head.header_len*4 + 8);
	/*填充capwap头*/
	memcpy(msg_offset,&ac_runtime.capwap_head, 
		ac_runtime.capwap_head.header_len*4);
	#if 1/*小端特别处理下*/
	memcpy(&tmp,&ac_runtime.capwap_head, 4);
	*(int *)msg_offset = htonl(tmp);
	#endif
	msg_offset += ( int )ac_runtime.capwap_head.header_len*4;
	/*填充msg头*/
	*(int *)msg_offset = htonl(msg_type);
	msg_offset += 4;
	*(char *)msg_offset = 0;
	msg_offset += 1;
	*(short int *)msg_offset = htons(len - ac_runtime.capwap_head.header_len*4 - 8);
	msg_offset += 2;
	*(char *)msg_offset = 0;
	return 0;
}

int prepare_join_rsp(struct ap_node *node)
{
	int ret=0;
	char sql_str[512] = {0};
	char *msg_buf = node->rsp_buf;
		node->rsp_buf_len = ac_runtime.capwap_head.header_len*4 + 8;/*先跳过capwap头和msg头*/
	msg_buf += node->rsp_buf_len;
	#if 0/*确认ap信息是否注册*/
	sprintf(sql_str,"select count(*) from ap_version where \
		manufacturer='%s' and hardware_version='%s' \
		and product_model='%s'",node->company,node->hard_ver,node->ap_model);
	sqlite3_exec(pFile,sql_str,get_db_value,&ret,0);
	if(ret == 0)
	{/*未注册，所以拒绝加入*/
		*(short int *)msg_buf = htons(33);
		msg_buf += 2;node->rsp_buf_len +=2;
		*(short int *)msg_buf = htons(4);
		msg_buf += 2;node->rsp_buf_len +=2;

		*(int *)msg_buf = htonl(6);
		msg_buf += 4;node->rsp_buf_len +=4;
		goto out;
	}
	#endif
	sprintf(sql_str,"select id,ap_mac,ap_group_name,status,count(*) from ap_info where ap_mac='%s'",node->apmac);
	sqlite3_exec(pFile,sql_str,db_join,node,0);
out:	fill_head( node->rsp_buf, join_resp,  node->rsp_buf_len);
	/*capwap消息总长度等于capwap头+ msg头+ 所有element消息体长度*/
	return node->rsp_buf_len;
}

int prepare_echo_rsp(struct ap_node *node)
{
	int ret;
	char sql_str[512] = {0};
	sqlite3_stmt      *pstmt = 0;
       const char      *error =0;
	unsigned int           len,tmp_mask,i;
	unsigned int bit = 1;
       const void       *value;

	char *msg_buf = node->rsp_buf;
	node->rsp_buf_len = ac_runtime.capwap_head.header_len*4 + 8;/*先跳过capwap头和msg头*/
	msg_buf += node->rsp_buf_len;
	#if 1/*先确认是否发小类消息*/
	sprintf(sql_str,"select config_status from ap_info where ap_mac='%s'",node->apmac);
	ret = sqlite3_prepare(pFile, sql_str, strlen(sql_str), &pstmt, &error);
	if( ret != SQLITE_OK )
		return 0;
	while(1)
	{
		ret = sqlite3_step(pstmt);
		if( ret != SQLITE_ROW )
			break;

		value = sqlite3_column_blob(pstmt, 0);
		len = sqlite3_column_bytes(pstmt,0 );
	   
		if(value!=NULL)
		{
			memcpy(msg_buf, value, len);
			node->rsp_buf_len += len;
		}
	}
	sqlite3_finalize(pstmt);
	if(node->rsp_buf_len != ac_runtime.capwap_head.header_len*4 + 8)
	{
		sprintf(sql_str,"update ap_info set config_status='' where ap_mac='%s'",node->apmac);
		sqlite3_exec(pFile,sql_str,0,0,0);
		goto out;
	}
	#endif

	#if 1/*确认大类消息发送*/
	sprintf(sql_str,"select config_mask from ap_info where ap_mac='%s'",node->apmac);
	sqlite3_exec(pFile,sql_str,get_db_value,&ret,0);
	for(i=0;i<16;i++,bit=1)
	{
		tmp_mask = ret;
		bit = bit << i;
		tmp_mask = (tmp_mask & bit) >0 ? (i+4097) : 0;
		assemble_echo_mask_id(tmp_mask,node);
	}
	#endif
	
	#if 1 
	if(ret != 0)/*config_mask清零*/
	{	
		sprintf(sql_str,"update ap_info set config_mask=0 where ap_mac='%s'",node->apmac);
		sqlite3_exec(pFile,sql_str,0,0,0);
	}	
	
	if((ret & 1) == 1 && (node->forward_flag >= 1))/*为该ap建立隧道*/
	{
		sprintf(sql_str, "/ac/script/tunnel_set %d %u %s", node->forward_flag,node->l2tp_id,inet_ntoa(node->ap_from->sin_addr));
		system(sql_str);
	}
	#endif
	
out:	fill_head( node->rsp_buf, echo_resp,  node->rsp_buf_len);
	/*capwap消息总长度等于capwap头+ msg头+ 所有element消息体长度*/
	return node->rsp_buf_len;
}

int prepare_sta_request_rsp(struct ap_node *node)
{
	int ret=0,ret2=0,i=0;
	char sql_str[512] = {0};
	long long mac = 0;
	char tmp[64] = {0};

	char *msg_buf = node->rsp_buf;
	node->rsp_buf_len = ac_runtime.capwap_head.header_len*4 + 8;/*先跳过capwap头和msg头*/
	msg_buf += node->rsp_buf_len;

#if 1/*id 33消息体*/	
	*(short int *)msg_buf = htons(33);
	msg_buf += 2;node->rsp_buf_len +=2;
	*(short int *)msg_buf = htons(4);
	msg_buf += 2;node->rsp_buf_len +=2;
#if 0
	sprintf(sql_str,"select count(*) from ap_info where ap_mac='%s'",node->apmac);
	sqlite3_exec(pFile,sql_str,get_db_value,&ret,0);
	
	if(ret ==0)
	{/*如果查无此ap，则回应失败*/
		*(int *)msg_buf = htonl(1);
		msg_buf += 4;node->rsp_buf_len +=4;
		goto out;
	}
#endif


	if(node->sta_node.flag == 0)/*sta上线*/
	{	
	#if 1
		sprintf(sql_str,"select sta_blance_sw from ap_group where ap_group_name=\
		(select ap_group_name from ap_info where ap_mac='%s')",node->apmac);
		sqlite3_exec(pFile,sql_str,get_db_value,&ret,0);
		if(ret ==1)
		{
			sprintf(sql_str,"select avg(sta_num) from ap_info where \
				ap_group_name=(select ap_group_name from ap_info where ap_mac='%s')",node->apmac);
			sqlite3_exec(pFile,sql_str,get_db_value,&ret2,0);
			
			sprintf(sql_str,"select sta_num from ap_info where ap_mac='%s')",node->apmac);
			sqlite3_exec(pFile,sql_str,get_db_value,&ret,0);
			if(ret > ret2)
				goto kick;
		}
	#endif
		
		sprintf(sql_str,"insert or replace into sta_list_assc \
			(sta_mac,assc_ap_mac,assc_time,radio) values \
			('%s','%s',time('now','localtime'),'%d')",node->sta_node.stamac,node->apmac,node->sta_node.radio_channel);
			
		sqlite3_exec(pFile,sql_str,0,0,0);

		#if 1/*/*确认是否需要id 18 mac黑名单消息体*/
		sprintf(sql_str,"select count(*) from sta_blacklist where sta_mac='%s'",node->sta_node.stamac);
		sqlite3_exec(pFile,sql_str,get_db_value,&ret,0);
kick:	if(ret !=0)
		{
			*(int *)msg_buf = htonl(18);
			msg_buf += 4;node->rsp_buf_len +=4;
				
			*(short int *)msg_buf = htons(18);
			msg_buf += 2;node->rsp_buf_len +=2;
			*(short int *)msg_buf = htons(8);
			msg_buf += 2;node->rsp_buf_len +=2;

			*(short int *)msg_buf = htons(6);	
			msg_buf += 2;node->rsp_buf_len +=2;
			mac = strtoull(node->sta_node.stamac,NULL,16);
			memcpy(tmp,&mac,sizeof(mac));
			#if 1
			for(i=0;i<6;i++)
			{
				*(char *)msg_buf = (char)tmp[5-i];
				msg_buf += 1;
				node->rsp_buf_len++;
			}
			#endif
		}else
		{	
			*(int *)msg_buf = htonl(0);
			msg_buf += 4;node->rsp_buf_len +=4;
		}
		#endif
	}else
	{/*sta 下线*/
		sprintf(sql_str,"delete from sta_list_assc where sta_mac='%s' ",node->sta_node.stamac);
		sqlite3_exec(pFile,sql_str,0,0,0);
			*(int *)msg_buf = htonl(0);
			msg_buf += 4;node->rsp_buf_len +=4;

	}
			
out:	fill_head( node->rsp_buf, station_conf_resp,  node->rsp_buf_len);
	/*capwap消息总长度等于capwap头+ msg头+ 所有element消息体长度*/
	return node->rsp_buf_len;
#endif		
}

int send_capwap(int msg_type,struct ap_node *node)
{
	int capwap_msg_len = 0;
	node->rsp_buf = malloc(CAPWAP_MSG_SIZE);
	switch (msg_type)
	{
		case join_resp:
			capwap_msg_len = prepare_join_rsp(node);
			break;
		case echo_resp:
			capwap_msg_len = prepare_echo_rsp(node);
			break;
		case station_conf_resp:
			capwap_msg_len = prepare_sta_request_rsp(node);
			break;
	}
	if(capwap_msg_len != 0)
	{	
		sendto(ac_runtime.recv_ac_sock, node->rsp_buf, capwap_msg_len, 0, 
			(struct sockaddr *)node->ap_from,	 sizeof(struct sockaddr));
	}
	if(debug == 1)/*IPV4打印*/
	{
		int tmp_port = 0;
		char abuf [50];
		snprintf(abuf, sizeof(abuf), "%s", inet_ntoa(node->ap_from->sin_addr));
		tmp_port = ntohs(node->ap_from->sin_port);
		print_info("send msg_type=%d to %s:%d, bytes=%d :\n",msg_type,abuf,tmp_port,capwap_msg_len);
		dump_data(node->rsp_buf, capwap_msg_len);
	}
	free(node->rsp_buf);
	return 0;
}
int parseid_board(char *buf,struct ap_node *node)
{
	unsigned char tmp_str8[8] = {0};
	unsigned int tmp_value = 0,id_len  = 0,subid_len=0,cur_len=0;
	char *tmp_buf = buf;
	
	tmp_buf += 2;
	id_len = ntohs(*(short *)tmp_buf);
	tmp_buf += 6;/*跳过4字节的企业码*/
	char * sub_buf = tmp_buf;

	while(cur_len < id_len)
	{
		tmp_buf = sub_buf;
		tmp_value = ntohs(*(short *)tmp_buf);

		tmp_buf += 2;
		subid_len = ntohs(*(short *)tmp_buf);
		tmp_buf += 2;
		switch(tmp_value)
		{	
			case 0:
				strcpy(node->ap_model,tmp_buf);
				sub_buf += subid_len + 4;cur_len += subid_len + 4;/*指向下一个subid*/
				break;
			case 2:
				strcpy(node->company,tmp_buf);
				sub_buf += subid_len + 4;cur_len += subid_len + 4;/*指向下一个subid*/
				break;
			case 3:
				strcpy(node->hard_ver,tmp_buf);
				sub_buf += subid_len + 4;cur_len += subid_len + 4;/*指向下一个subid*/
				break;
			case 4:
				memcpy(tmp_str8,tmp_buf,subid_len);
				sprintf(node->apmac,MACSTR2,MAC2STR(tmp_str8));
				sub_buf += subid_len + 4;cur_len += subid_len + 4;/*指向下一个subid*/
				break;	
			case 5:
				strcpy(node->soft_ver,tmp_buf);
				sub_buf += subid_len + 4;cur_len += subid_len + 4;/*指向下一个subid*/
					
			default:
				sub_buf += subid_len + 4;cur_len += subid_len + 4;/*指向下一个subid*/
		}
	}
	return id_len+4;
}
int parseid_apmac(char *buf,struct ap_node *node)
{
	unsigned char tmp_str8[8] = {0};
	unsigned int tmp_value = 0,id_len  = 0,subid_len=0,cur_len=0;
	char *tmp_buf = buf;
	
	tmp_buf += 2;
	id_len = ntohs(*(short int*)tmp_buf);
	tmp_buf += 2;

	tmp_buf += 6;

	memcpy(tmp_str8,tmp_buf,6);
	sprintf(node->apmac,MACSTR2,MAC2STR(tmp_str8));
	/*保存该ap的l2tp_id,即mac的后四位整数*/
	tmp_buf += 2;
	node->l2tp_id = ntohl(*(unsigned int*)tmp_buf);
	
	return id_len+4;
}
int parseid_keeplive(char *buf,struct ap_node *node)
{
	char tmp_str8[8] = {0};
	unsigned int tmp_value = 0,id_len  = 0,subid_len=0,cur_len=0;
	char *tmp_buf = buf;
	
	char sql_str[256] = {0};
	unsigned char sta_num = 0;
	
	tmp_buf += 2;
	id_len = ntohs(*(short *)tmp_buf);
	tmp_buf += 2;
	char * sub_buf = tmp_buf;

	while(cur_len < id_len)
	{
		tmp_buf = sub_buf;
		tmp_value = *(char *)tmp_buf;

		tmp_buf += 1;
		subid_len = *(char *)tmp_buf;
		tmp_buf += 1;
		switch(tmp_value)
		{	
			case 1:
				node->keeplive = (*(char *)tmp_buf) == 0 ? 15 : (*(char *)tmp_buf)* 60;
				sub_buf += subid_len + 2;cur_len += subid_len + 2;/*指向下一个subid*/
				break;
			case 2:
				sta_num = *(char *)tmp_buf;
				sprintf(sql_str,"update ap_info set sta_num=%d where ap_mac='%s'",sta_num,node->apmac);
				sqlite3_exec(pFile,sql_str,0,0,0);
				sub_buf += subid_len + 2;cur_len += subid_len + 2;/*指向下一个subid*/
				break;
			default:
				sub_buf += subid_len + 2;cur_len += subid_len + 2;/*指向下一个subid*/
		}
	}
	return id_len+4;
}
int parseid_ap_report(char *buf,struct ap_node *node)
{
	char tmp_str8[8] = {0};
	unsigned int tmp_value = 0,id_len  = 0,subid_len=0,cur_len=0;
	char *tmp_buf = buf;
	
	tmp_buf += 2;
	id_len = ntohs(*(short *)tmp_buf);
	tmp_buf += 2;
	char * sub_buf = tmp_buf;

	
	char sql_str[512] = {0};
	struct ap_report *tmp_report = (struct ap_report *)malloc(sizeof(struct ap_report));
	memset(tmp_report,0,sizeof(struct ap_report));
	sprintf(sql_str,"select * from ap_report where ap_mac='%s'",node->apmac);
	sqlite3_exec(pFile,sql_str,get_old_report_value,tmp_report,0);

	while(cur_len < id_len)
	{
		tmp_buf = sub_buf;
		tmp_value = *(char *)tmp_buf;

		tmp_buf += 1;
		subid_len = *(char *)tmp_buf;
		tmp_buf += 1;
		switch(tmp_value)
		{	
			case 1:
				tmp_buf += 2;/*跳过保留字节*/
				tmp_report->eth_re +=ntohl(*(int *)tmp_buf);
				tmp_buf += 4;
				tmp_report->eth_se +=ntohl(*(int *)tmp_buf);
				tmp_buf += 4;
				tmp_report->wifi_re +=ntohl(*(int *)tmp_buf);
				tmp_buf += 4;
				tmp_report->wifi_se +=ntohl(*(int *)tmp_buf);
				tmp_buf += 4;
				tmp_report->lte_re +=ntohl(*(int *)tmp_buf);
				tmp_buf += 4;
				tmp_report->lte_se +=ntohl(*(int *)tmp_buf);
				tmp_buf += 4;
				sub_buf += subid_len + 2;cur_len += subid_len + 2;/*指向下一个subid*/
				break;
			default:
				sub_buf += subid_len + 2;cur_len += subid_len + 2;/*指向下一个subid*/
		}
	}
	sprintf(sql_str,"insert or replace into ap_report (ap_mac,eth_re,eth_se,wifi_re, \
		wifi_se,lte_re,lte_se) values ('%s',%u,%u,%u,%u,%u,%u \
		)",node->apmac,tmp_report->eth_re,tmp_report->eth_se,
		tmp_report->wifi_re,tmp_report->wifi_se,tmp_report->lte_re,tmp_report->lte_se);
	sqlite3_exec(pFile,sql_str,0,0,0);
	free(tmp_report);
	return id_len+4;
}
int parseid_sta_up(char *buf,struct ap_node *node)
{
	unsigned char tmp_str8[8] = {0};
	unsigned int tmp_value = 0,id_len  = 0,subid_len=0,cur_len=0;
	unsigned char *tmp_buf = buf;

	tmp_buf += 2;
	id_len = ntohs(*(short *)tmp_buf);
	tmp_buf += 2;
	tmp_buf += 2;

	memcpy(tmp_str8,tmp_buf,6);
	sprintf(node->sta_node.stamac,MACSTR2,MAC2STR(tmp_str8));

	node->sta_node.flag=0;
	if(id_len > 8)
	{
		tmp_buf += 6;
		node->sta_node.radio_channel = *(unsigned char *)tmp_buf;
	}

	return id_len+4;
}
int parseid_sta_down(char *buf,struct ap_node *node)
{
	unsigned char tmp_str8[8] = {0};
	unsigned int tmp_value = 0,id_len  = 0,subid_len=0,cur_len=0;
	char *tmp_buf = buf;
	
	tmp_buf += 2;
	id_len = ntohs(*(short *)tmp_buf);
	tmp_buf += 2;

	tmp_buf += 2;
	
	memcpy(tmp_str8,tmp_buf,6);
	sprintf(node->sta_node.stamac,MACSTR2,MAC2STR(tmp_str8));
	node->sta_node.flag=1;

	return id_len+4;
}
int recv_join(char * msg_buf,struct ap_node *node)
{
	#define board_data 38
	int msg_len = 0,tmp_len = 0,tmp_ele_id = 0,tmp_value= 0,cur_len = 0;
	char * tmp_buf = msg_buf;
	tmp_buf += 4 + 1;/*跳过msg type和sel num*/
	msg_len = ntohs(*(short int *)tmp_buf);
	tmp_buf += 3;
	node->ap_model = calloc(64,1);
	node->company= calloc(64,1);
	node->hard_ver= calloc(64,1);
	node->soft_ver= calloc(64,1);

	while (cur_len < msg_len)
	{
		tmp_ele_id = ntohs(*(short int *)tmp_buf);
		switch (tmp_ele_id)
		{
			case board_data:
				tmp_len = parseid_board(tmp_buf,node);
				tmp_buf += tmp_len;cur_len += tmp_len;
				break;
			default:
				/*跳过该id元素*/
				//print_info("recv_join_resp:unknow id=%d\n",tmp_ele_id);
				tmp_buf += 2;cur_len += 2;
				tmp_value =ntohs( *(short int *)tmp_buf);
				tmp_buf += tmp_value + 2;cur_len += tmp_value+2;
		}
	}
	send_capwap(join_resp,node);
	free(node->ap_model);
	free(node->company);
	free(node->hard_ver);
	return 0;
}
void keep_live_apdown(void *eloop_data, void *user_ctx,short int ap_id,unsigned int l2tp_id)
{
	char sql_str[512] = {0};
	unsigned int ret = l2tp_id;
	sprintf(sql_str, "update ap_info set status=2,sta_num=0 where id=%d ", ap_id);
	sqlite3_exec(pFile,sql_str,0,0,0);
	
	sprintf(sql_str,"select ap_mac,ap_group_name from ap_info where id=%d",ap_id);
	sqlite3_exec(pFile,sql_str,ap_down_hook,&ret,0);
}
int recv_echo(char *msg_buf,struct ap_node *node)
{
	#define echo_apmac 37
	#define echo_keeplive 4096
	#define echo_ap_report 4097
	int msg_len = 0,tmp_len = 0,tmp_ele_id = 0,tmp_value= 0,cur_len = 0;
	char sql_str[512] = {0};
	short int ret = 0;
	char * tmp_buf = msg_buf;
	tmp_buf += 4 + 1;/*跳过msg type和sel num*/
	msg_len = ntohs(*(short int *)tmp_buf);
	tmp_buf += 3;

	while (cur_len < msg_len)
	{
		tmp_ele_id = ntohs(*(short int *)tmp_buf);
		switch (tmp_ele_id)
		{
			case echo_apmac:
				tmp_len = parseid_apmac(tmp_buf,node);
				tmp_buf += tmp_len;cur_len += tmp_len;
				break;
			case echo_keeplive:
				
				tmp_len = parseid_keeplive(tmp_buf,node);
				tmp_buf += tmp_len;cur_len += tmp_len;
				break;
			case echo_ap_report:
				
				tmp_len = parseid_ap_report(tmp_buf,node);
				tmp_buf += tmp_len;cur_len += tmp_len;
				break;
			default:
				/*跳过该id元素*/
				print_info("recv_echo:unknow id \n");
				tmp_buf += 2;cur_len += 2;
				tmp_value =ntohs( *(short int *)tmp_buf);
				tmp_buf += tmp_value + 2;cur_len += tmp_value+2;
		}
	}
	#if 1/*确认ap是否属于ap列表中，且status为up*/
	sprintf(sql_str,"select id from ap_info where ap_mac='%s' and status=1",node->apmac);
	sqlite3_exec(pFile,sql_str,get_db_value,&ret,0);
	if(ret == 0)
	{/*忽略该ap*/
		print_info("\n%s need rejoin,so drop its msg!\n",node->apmac);
		return 0;
	}
	#endif
	node->ap_id = ret;
	eloop_cancel_timeout(keep_live_apdown,NULL,NULL,node);
	eloop_register_timeout(node->keeplive*2 + 2, 0, keep_live_apdown, NULL, NULL,node);
	send_capwap(echo_resp,node);
	return 0;
}

int recv_station_conf(char *msg_buf,struct ap_node *node)
{
	#define sta_apmac 37
	#define sta_up 8
	#define sta_down 18
	int msg_len = 0,tmp_len = 0,tmp_ele_id = 0,tmp_value= 0,cur_len = 0;
	char * tmp_buf = msg_buf;
	tmp_buf += 4 + 1;/*跳过msg type和sel num*/
	msg_len = ntohs(*(short int *)tmp_buf);
	tmp_buf += 3;

	while (cur_len < msg_len)
	{
		tmp_ele_id = ntohs(*(short int *)tmp_buf);
		switch (tmp_ele_id)
		{
			case sta_apmac:
				tmp_len = parseid_apmac(tmp_buf,node);
				tmp_buf += tmp_len;cur_len += tmp_len;
				break;
			case sta_up:
				tmp_len = parseid_sta_up(tmp_buf,node);
				tmp_buf += tmp_len;cur_len += tmp_len;
				break;
			case sta_down:
				tmp_len = parseid_sta_down(tmp_buf,node);
				tmp_buf += tmp_len;cur_len += tmp_len;
				break;
			default:
				/*跳过该id元素*/
				print_info("recv_sta_request: unknow id \n");
				tmp_buf += 2;cur_len += 2;
				tmp_value =ntohs( *(short int *)tmp_buf);
				tmp_buf += tmp_value + 2;cur_len += tmp_value+2;
		}
	}
	send_capwap(station_conf_resp,node);
	return 0;
}

void  receive_capwap(int sock, void *eloop_ctx,void *sock_ctx)
{
	char *capwap_msg = NULL;
	//struct capwap_header *hbuf = NULL;
	char *buf = NULL;
	struct sockaddr_storage from;
	socklen_t fromlen;
	int len,all_ele_id_len,capwap_header_len;
	unsigned int msg_type = 0;

	capwap_msg = malloc(CAPWAP_MSG_SIZE);
	if (capwap_msg == NULL) 
	{
		print_info("recv capwapd: malloc fail !\n");
	}

	fromlen = sizeof(from);
	len = recvfrom(sock, capwap_msg, CAPWAP_MSG_SIZE, 0,(struct sockaddr *) &from, &fromlen);
	if (len < 0)
	{
		print_info("recv capwapd: recv fail !\n");
	}
#if 1/*检查capwap消息长度是否合法，并找到msg_type*/
	buf = capwap_msg;
	capwap_header_len = 8;
	buf += capwap_header_len;/* 跳过capwap head的长度*/
	msg_type = ntohl(*(int *)buf);
	buf += 5;
	all_ele_id_len = ntohs(*(short int *)buf);
	if(all_ele_id_len+capwap_header_len+8 != len)
		print_info("error !!!recv capwap:the length of recv msg !=  all_element_id_len + 16\n");

	buf -= 5;/*重新指回msg_type*/
	
#endif
	if(debug == 1)/*IPV4打印*/
	{
		struct sockaddr_in *from4 = (struct sockaddr_in *) &from;
		int tmp_port = 0;
		char abuf [50];
		snprintf(abuf, sizeof(abuf), "%s", inet_ntoa(from4->sin_addr));
		tmp_port = ntohs(from4->sin_port);
		print_info("recv msg_type=%d from %s:%d, bytes=%d :\n",msg_type,abuf,tmp_port,len);
		dump_data(capwap_msg, len);
	}
	struct ap_node tmp_node;
	memset(&tmp_node,0,sizeof(tmp_node));
	tmp_node.ap_from = (struct sockaddr_in *) &from;
/*以下buf 已直接指向msg_type,跳过了capwap头*/
	switch (msg_type)
	{	
		case join_request:
			recv_join(buf,&tmp_node);
			break;
		case echo_request:
			recv_echo(buf,&tmp_node);
			break;
		case station_conf_request:
			if(event_sta_updown == 1)
				recv_station_conf(buf,&tmp_node);//用于强制下线、黑白名单等
			break;
		case conf_update_resp:
			print_info("ap conf_update success! \n");	
			break;
		default :
			print_info("unknow capwap msg! \n");	
	}
	free(capwap_msg);
}

int capwap_init()
{

#if 1/*初始化capwap 接收端口*/
	struct sockaddr_in tmp_addr;

	ac_runtime.recv_ac_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (ac_runtime.recv_ac_sock < 0) 
	{       
		print_info("register recv_ac_sock failed!\n");
		close(ac_runtime.recv_ac_sock);
		return -1;
	}    

	memset(&tmp_addr, 0, sizeof(tmp_addr));
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	tmp_addr.sin_port = htons(CAPWAP_PORT);
	if (bind(ac_runtime.recv_ac_sock, (struct sockaddr *) &tmp_addr, sizeof(tmp_addr)) < 0) 
	{      
		print_info("bind recv_ac_sock failed\n");
		close(ac_runtime.recv_ac_sock);
		return -1;
	}
	//int opt =1;
	//setsockopt(ap_info.runtime.recv_ac_sock,SOL_SOCKET,SO_REUSEADDR,&opt,1);
	eloop_register_read_sock(ac_runtime.recv_ac_sock,receive_capwap,NULL, NULL);
#endif

/*初始化capwap运行时的环境变量*/
	ac_runtime.ac_state = 0;
/*初始化capwap头部*/
	ac_runtime.capwap_head.version = 0;
	ac_runtime.capwap_head.type = 0;
	ac_runtime.capwap_head.header_len = 2;
	ac_runtime.capwap_head.radio_id = 0;
	ac_runtime.capwap_head.wireless_bind_id = 1;
	ac_runtime.capwap_head.header_flags = 0;
	ac_runtime.capwap_head.fragment_id = 0;
	ac_runtime.capwap_head.frag_offset = 0;	
	ac_runtime.capwap_head.rev = 0;	
	return 0;
}


