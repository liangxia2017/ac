/*==========================================================================*
 *    Copyright(c) 2011, XiongDi., Ltd.
 *                     ALL RIGHTS RESERVED
 *
 *  PRODUCT  : AC 与CAPWAPS 以及HTTPS 通讯的应用程序
 *
 *  CREATOR  : 李伟               DATE: 2011-02-15 10:11
 *  VERSION  : V1.00
 *  PURPOSE  :
 *
 *
 *  HISTORY  :
 *
 *==========================================================================*/
 #include <stdio.h>
 #include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <sys/ioctl.h> 
#include <net/if.h>
#include <netdb.h>

#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "ac_client.h"
#include "main.h"

#include "myproc.h"
#include "capwap_msg.h"

pthread_t ClientId,ServerId;
struct thread_param paClientId;
struct thread_param paServerId;
struct thread_param
{
	char info;
	int num;
};

//add by lzc,20110223
#if 1
void print_mem (unsigned char *p, int k)
{ 
int i; 
printf("%s\n", __func__); 
for (i = 0; i < k; i++) 
{ 
printf("%02x ", *p++); 
if ((i + 1) % 16 == 0) 
printf("\n"); 
} 
printf("\n\n");
}
#endif

//得到消息体长度(不含消息头)
unsigned long get_msg_len_int32(void *buff)
{
	unsigned long ret;

	if (!buff)
		return 0;

	memcpy(&ret, buff, 4);
	ret = ntohl(ret);
	return ret;
}

unsigned long get_msg_len_int16(void *buff)
{
	unsigned short ret;

	if (!buff)
		return 0;

	memcpy(&ret, buff, 2);
	ret = ntohs(ret);
	return ret;
}
/*==========================================================================*
 * FUNCTION :void sig_handler(int sig)
 * PURPOSE  :信号处理函数	用于转发页面消息	 
 *	 
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  李伟                  DATE: 2011-03-8 15:47
 *==========================================================================*/

void sig_handler(int sig)
{
	/*这里把页面消息转发给CAPWAPS*/
	printf("CCCCCCCC\n");
	
	unsigned char buff[MESSAGE_SIZE];
	
	
	unsigned short type;
	unsigned short lenght;
	int i = 0;
	int n;
	LOCK_Communi_SYNC();
	if((message_info.meg_flag == 0) || (message_info.meg_buff == NULL))
	{
		UNLOCK_Communi_SYNC();
		fprintf(stderr, "no message to send\a\n");
		return;
		
	}
	unsigned char * p = message_info.meg_buff;
	unsigned char  flag = p[2];
	unsigned long len = get_msg_len_int32(p + 4);
	
	if(flag == 0)
	{
		//下发全局配置	
		
	}
	else if(flag == 1)
	{
		
		type = get_msg_len_int16(p + 8);
		if(1 != type)
		{
			message_info.meg_flag = 0;
			UNLOCK_Communi_SYNC();
			fprintf(stderr, "get  message type error\a\n");
			return;		
		}
		lenght = get_msg_len_int16(p + 10);
		n = send(socket_connect_to_capwapd, message_info.meg_buff + 12, message_info.meg_len - 12, 0);
		if(n != message_info.meg_len)
		{
			message_info.meg_flag = 0;
			UNLOCK_Communi_SYNC();
			fprintf(stderr, "send  message error\a\n");
			return;
			
		}	
		
	}
	else if(flag == 2)
	{	
		type = get_msg_len_int16(p + 8);
		
		if(1 != type)
		{
			message_info.meg_flag = 0;
			UNLOCK_Communi_SYNC();
			fprintf(stderr, "get  message type error\a\n");
			return;		
		}
		
	  	lenght = get_msg_len_int16(p + 10);
		lenght = lenght /6;
		if(lenght <= 1)
		{
			message_info.meg_flag = 0;
			UNLOCK_Communi_SYNC();
			fprintf(stderr, "get  FLAG from WEB error\a\n");
			return;			
		}
		for(i = 0; i < lenght; i ++)
		{
			memset(buff, 0, MESSAGE_SIZE);
			memcpy(buff, message_info.meg_buff + 12 + 6 * i, 6);
			
			memcpy(buff + 6, message_info.meg_buff + 12 + 6 * lenght, message_info.meg_len - 12 - 6 * lenght );
			n = send(socket_connect_to_capwapd, buff,  message_info.meg_len - 6 - 6 * lenght, 0);
			if(n != message_info.meg_len)
			{
				message_info.meg_flag = 0;
				UNLOCK_Communi_SYNC();
				fprintf(stderr, "send  message to MAC:%02X-%02X-%02X-%02X-%02X-%02X\a\n",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5]);
				return;
				
			}		
			
		}
		
	}
	else
	{
			message_info.meg_flag = 0;
			UNLOCK_Communi_SYNC();
			fprintf(stderr, "get  FLAG from WEB error\a\n");
			return;	
	}
	message_info.meg_flag = 0;
	UNLOCK_Communi_SYNC();
	printf("**************\r\n");
	print_mem(message_info.meg_buff, message_info.meg_len);
	printf("**************\r\n");
	return;
}


/*==========================================================================*
 * FUNCTION :void AC_client_to_capwaps(void)
 * PURPOSE  :和CAPWAPS通信的线程函数		 
 *	 
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  李伟                  DATE: 2011-02-15 15:47
 *==========================================================================*/
void  * AC_client_to_capwaps(void* param)
{
	UNUSED(param);
	
	
	//char buffer[1024];	
	struct sockaddr_in server_addr;
	unsigned char cmd_msg;
	int n = 0; 
	int m =0;
	char MAC[18];
	
	/* 客户程序开始建立 sockfd 描述符 */	
	if((socket_connect_to_capwapd = socket(AF_INET, SOCK_STREAM, 0)) == -1)	
	{		
		fprintf(stderr, "Socket Error：%s\a\n", strerror(errno));		
		exit(1);	
	}	
	/* 客户程序填充服务端的资料 */	
	bzero(&server_addr, sizeof(server_addr));	
	server_addr.sin_family = AF_INET;	
	server_addr.sin_port  = htons(AC_CLEINT_PORT);	
	server_addr.sin_addr.s_addr = inet_addr(CAPWAPD_IP);

	
	/* 客户程序发起连接请求 */
	if(connect(socket_connect_to_capwapd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)	
	{		
		printf("connect error!!!!\n");
		fprintf(stderr, "Connect Error：%s\a\n", strerror(errno));		
		exit(1);	
	}	

	printf("连接成功\n");

	struct sigaction meg_signal;
	sigemptyset(&meg_signal.sa_mask);
	meg_signal.sa_handler = sig_handler;
 	//meg_signal.sa_flags = SA_RESTART;
	meg_signal.sa_flags = 0;
	/*处理页面消息*/
	if(sigaction(SIGUSR1, &meg_signal, NULL) == -1)
	{
		perror("fail to set handler\n");
		exit(1);
	}
	char buff[MESSAGE_SIZE]; 
	while(1)
	{
		if((n = recv(socket_connect_to_capwapd, &cmd_msg, sizeof(unsigned char), 0)) > 0)
		{
			switch(cmd_msg)
			{
				case CONNECTION_OK:
					
					printf("!!!CONNECTION_OK\n");

					capwaps_client_flag = 1;
						int i = 0;
                                   unsigned char msg_buf[256];

						  
//add by lzc,20110225
#if 1	
					int icount = 0;
					//NTP_CLIENT_CONFIG_T ntpglobconfig;
					//get ntp config
					icount = get_ntp_client_config();
					if (icount != 2)
							{
						printf("ERROR,must be at least 2 ntp client config entry!\n");
						goto get_date_err;
						  }
					
					if(gNtpClientConfig[0].flag != 1)
					{
						printf("ERROR,ntp client flag must be  1 !\n");
						goto get_date_err;
					}		
					

					memset(msg_buf,0,256);
					//发送NTP_CLIENT_CONFIG
					msg_buf[0] = GLOB_CONF_MSG;
					msg_buf[1] = NTP_CLIENT_CONFIG_MSG;
					memcpy((msg_buf+2),&gNtpClientConfig[0],sizeof(NTP_CLIENT_CONFIG_T));
					send(socket_connect_to_capwapd,msg_buf,sizeof(NTP_CLIENT_CONFIG_T)+2,0);
										


#if 0
					//get snmp config
					icount =  get_snmp_config();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 snmp config entry!\n");
						goto get_date_err;
					}
  
					cfg_msg  = SNMP_CONFIG_MSG;

					//发送SNMP_CLIENT_CONFIG
					send(socket_connect_to_capwapd,&ret_golbconfig_cmd,sizeof(unsigned char),0);
					send(socket_connect_to_capwapd,&cfg_msg , sizeof(unsigned char), 0);
					send(socket_connect_to_capwapd,&(gsnmpConfigInfo[0]),sizeof(gsnmpConfigInfo[0]),0);

#endif

					//get radius config
					icount = get_radius_config();
					if (icount != 2)
					{
						printf("ERROR,must be at least 2 radius config entry!\n");
						goto get_date_err;
					}

					memset(msg_buf,0,256);
					//发送NTP_CLIENT_CONFIG
					msg_buf[0] = GLOB_CONF_MSG;
					msg_buf[1] = RADIUS_CONFIG_MSG;
					
					memcpy((msg_buf+2),&gradiusConfigInfo[0],sizeof(RADIUS_CONFIG_T));
					memcpy((msg_buf+2+sizeof(RADIUS_CONFIG_T)),&gradiusConfigInfo[1],sizeof(RADIUS_CONFIG_T));
					send(socket_connect_to_capwapd,msg_buf,2*sizeof(RADIUS_CONFIG_T)+2,0);

					

					//get as config
					icount =  get_as_config();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 as config entry!\n");
						goto get_date_err;
					}
					memset(msg_buf,0,256);
					//发送NTP_CLIENT_CONFIG
					msg_buf[0] = GLOB_CONF_MSG;
					msg_buf[1] = AS_CONFIG_MSG;
					
					memcpy((msg_buf+2),&(gasConfigInfo[0]),sizeof(AS_CONFIG_T));
					send(socket_connect_to_capwapd,msg_buf,sizeof(AS_CONFIG_T)+2,0);


					//get version config
					icount =  get_version_config();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 version config entry!\n");
						goto get_date_err;
					}
					memset(msg_buf,0,256);
					//发送NTP_CLIENT_CONFIG
					msg_buf[0] = GLOB_CONF_MSG;
					msg_buf[1] = VERSION_CONFIG_MSG;
					
					memcpy((msg_buf+2),&(gVersionInfo[0]),sizeof(VERSION_INFO_T));
					send(socket_connect_to_capwapd,msg_buf,sizeof(VERSION_INFO_T)+2,0);

					

					//get syslog config
					icount =  get_syslog_config();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 syslog config entry!\n");
						goto get_date_err;
					}
					memset(msg_buf,0,256);
					//发送NTP_CLIENT_CONFIG
					msg_buf[0] = GLOB_CONF_MSG;
					msg_buf[1] = SYSLOG_CONFIG_MSG;
					
					memcpy((msg_buf+2),&(gSyslogConfig[0]),sizeof(SYSLOG_CONFIG_T));
					send(socket_connect_to_capwapd, msg_buf,sizeof(SYSLOG_CONFIG_T)+2,0);


					//get hotbak config
					icount =  get_hotbak_config();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 hot back config entry!\n");
						goto get_date_err;
					}
					memset(msg_buf,0,256);
					//发送NTP_CLIENT_CONFIG
					msg_buf[0] = GLOB_CONF_MSG;
					msg_buf[1] = HOT_BACK_CONFIG_MSG;
					
					memcpy((msg_buf+2),&(gHotbakConfig[0]),sizeof(HOTBAK_CONFIG_T));
					send(socket_connect_to_capwapd, msg_buf,sizeof(HOTBAK_CONFIG_T)+2,0);


					//get mac filter config
					icount =  get_mac_filter();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 mac filter config entry!\n");
						goto get_date_err;
					}
					memset(msg_buf,0,256);
					//发送NTP_CLIENT_CONFIG
					msg_buf[0] = GLOB_CONF_MSG;
					msg_buf[1] = MAC_FILTER_CONFIG_MSG;

					struct mac_filter
					{
						unsigned short icount;
						unsigned char mac[6];
					}filter_mac;
					
	
					int ifor = 0;
					filter_mac.icount = 1;
					for (ifor = 0; ifor < 6; ifor++)
					{

						filter_mac.mac[ifor] = strtoul(&(gMacFilter[0].mac[ifor*3]),0,16);

					}

					
					memcpy((msg_buf+2),&filter_mac,sizeof(filter_mac));
					send(socket_connect_to_capwapd,msg_buf,2+sizeof(filter_mac),0);
					print_mem(msg_buf, 10);
												
					
#if 0


					//get route config
					icount =  get_route_config();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 route config entry!\n");
						goto get_date_err;
					}
					//memcpy(&(globconfig.version_info),&gVersionInfo[0],sizeof(VERSION_INFO_T));


					//get net info
					icount =  get_net_info();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 net info entry!\n");
						goto get_date_err;
					}
					//memcpy(&(globconfig.version_info),&gVersionInfo[0],sizeof(VERSION_INFO_T));


					//get radio base
					icount =  get_radio_base();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 radio base entry!\n");
						goto get_date_err;
					}
					//memcpy(&(globconfig.version_info),&gVersionInfo[0],sizeof(VERSION_INFO_T));
				//get radio advance
					icount =  get_radio_advance();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 radio advance entry!\n");
						goto get_date_err;
					}
					//memcpy(&(globconfig.version_info),&gVersionInfo[0],sizeof(VERSION_INFO_T));

					//get channel config 
					icount =  get_chan_config();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 channel config entry!\n");
						goto get_date_err;
					}
					//memcpy(&(globconfig.version_info),&gVersionInfo[0],sizeof(VERSION_INFO_T));
					

	
					//get load  balance
					icount =  get_load_balance();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 load balance config entry!\n");
						goto get_date_err;
					}
					//memcpy(&(globconfig.version_info),&gVersionInfo[0],sizeof(VERSION_INFO_T));

					//get scan  config
					icount =  get_scan_config();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 scan config entry!\n");
						goto get_date_err;
					}
					//memcpy(&(globconfig.version_info),&gVersionInfo[0],sizeof(VERSION_INFO_T));

					//get capwap  timer config
					icount =  get_capwap_timer_config();
					if (icount < 1)
					{
						printf("ERROR,must be at least 1 capwap timer config entry!\n");
						goto get_date_err;
					}
					//memcpy(&(globconfig.version_info),&gVersionInfo[0],sizeof(VERSION_INFO_T));
#endif

       

				
					break;

	get_date_err:	
					printf("Get date from db error!!!\n ");
#endif				
					break;
				case JOIN_CHECK:
					/*这里要把加入AP的信息写入数据库*/
					/*发送默认配置给CAPWAPS*/
					printf("######Join_check\n");


					
					if((m= recv(socket_connect_to_capwapd, MAC, 17, 0)) > 0)
					{
						print_mem(MAC, 17);
						
						struct mymsg {
								int res;
						} joincheck;

						unsigned char joincheck_res_cmd  = JOIN_CHECK_RESULT;


						joincheck.res = get_ap_accord_mac(MAC);
						//success:1,fail:2
						if (0 != joincheck.res)
							joincheck.res = 1 ;
						else
							joincheck.res = 2;

						printf("join result = %d\n",joincheck.res);
						
						send(socket_connect_to_capwapd, &joincheck_res_cmd , sizeof(unsigned char), 0);
						send(socket_connect_to_capwapd, &joincheck , sizeof(joincheck), 0);
						//加入WLAN配置，发送给capwapd
						#if 1
						if(1 == joincheck.res)
						{
							memset(buff, 0, MESSAGE_SIZE);
							m = 0;
							n = get_wlan_config(MAC, buff, &m);
							print_mem(buff, m);
							send(socket_connect_to_capwapd, buff , m, 0);
							
						}
						#endif
						
					 }
					break;
				default :
					fprintf(stderr, "unknow message type：%d\a\n", cmd_msg);	
					break;
			}
		}
	};
}
//4 


int receive_web_msg(int socket , unsigned char * recv_buf)
{
	int n;
	unsigned char cmd_msg = 0;
	unsigned long len = 0;
	
	if(recv_buf != NULL)
		memset(recv_buf, 0, MESSAGE_SIZE);
	
	if((n = recv(socket, recv_buf, HEND_LEN, MSG_PEEK)) > 0)
	{
		len = get_msg_len_int32(recv_buf + 4);
		len += HEND_LEN;

		if (len > MESSAGE_SIZE)   //if (msg_con_len > sizeof(buf))
		{
			printf("@@@msg too long!\n");
			recv(socket, recv_buf, HEND_LEN, 0);
			return -1;
		}
		

		memset(recv_buf, 0, MESSAGE_SIZE); //memset(buf, 0, sizeof(buf));
		n = recv(socket, recv_buf, len, MSG_WAITALL);
		if(len != n)
		{
			fprintf(stderr, "receive Error message lenght,message lenght = %d, %ul\a\n", n, len);
			return -1;		
		}	
		
		cmd_msg = recv_buf[0];
		printf("sssssssssssssssss = %d\n", cmd_msg);
		switch(cmd_msg)
		{
			case GET_UPDATE_MESSAGE_FROM_WEB:
				LOCK_Communi_SYNC();
				message_info.meg_flag = 1;	
				//message_info.meg_buff = recv_buf;
				message_info.meg_len = len;
				UNLOCK_Communi_SYNC();
				pthread_kill(ClientId, SIGUSR1);
				break;
			default:
				fprintf(stderr, "receive Error message ,message number = %d\a\n", cmd_msg);
				printf("sssssssssssssssss\n");
				break;	
					
				
		}	
	}
}
/*==========================================================================*
 * FUNCTION :void AC_server_to_web(void)
 * PURPOSE  :和WEB页面通信的线程函数		 
 *	             注释:使用UDP协议
 * RETURN   :
 *	
 * COMMENTS : 
 * CREATOR  :  李伟                  DATE: 2011-02-15 15:47
 *==========================================================================*/
void  * AC_server_to_web(void* param)
{
	UNUSED(param);
	
	int listen_sock;
	
	struct sockaddr_in servaddr;
	int optValue = 1;
	if ( ( listen_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{
		//fprintf("Error on socket creation on Interface");
		fprintf(stderr, "Socket Error：%s\a\n", strerror(errno));		
		exit(1);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(CAPWAPS_IP); /* Not Extern: INADDR_ANY */
	servaddr.sin_port = htons(AC_SERVER_PORT); 


	if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &optValue, sizeof(int)) == -1) {
		fprintf(stderr, "Error on socket creation on Interface\a\n");
		exit(1);
	}

	/************************************
	 * Binding socket and Listen call	*
	 ************************************/
	
	if (  bind(listen_sock, (struct sockaddr *) &servaddr, sizeof(struct sockaddr_in)) < 0 ) {
		fprintf(stderr, "Error on Binding\a\n");
		exit(1);
	}


	int numfds = 0;
	int ret;
#if 1
	fd_set readfds;
			/*设置超时时间10秒*/
			
	struct timeval tiv;
	tiv.tv_sec = 10;/*超时时间为10秒*/
	tiv.tv_usec = 0;
	
	FD_SET(listen_sock, &readfds);
	numfds = (listen_sock >= numfds) ? (listen_sock + 1) : numfds;
	unsigned char recv_buf[MESSAGE_SIZE];
#endif	

	LOCK_Communi_SYNC();
	message_info.meg_flag = 0;	
	message_info.meg_buff = recv_buf;
	UNLOCK_Communi_SYNC();
	
	while(1)
	{

		setsockopt(listen_sock, SOL_SOCKET, SO_RCVTIMEO, &tiv, sizeof(struct timeval));
		
		FD_SET(listen_sock, &readfds);
		/*准备接收数据*/
		ret = select(numfds, &readfds, NULL, NULL, NULL);
		
		if (0 == ret)
			continue;
		else if (-1 == ret)
			break;

		if (FD_ISSET(listen_sock, &readfds))
		{
			LOCK_Communi_SYNC();
			if(message_info.meg_flag == 1)
			{
				UNLOCK_Communi_SYNC();
				sleep(1);
				printf("wait for a moment\n");
				continue;
			}
			UNLOCK_Communi_SYNC();
			receive_web_msg(listen_sock, recv_buf);
			
			if (listen_sock > 0)	FD_CLR(listen_sock, &readfds);
				continue;
		}
	};
}

int main(int argc, char *argv[])
{
//	INT8S bSimMsg[200];
//	INT8U cRe;
//	HANDLE	hPort;
#if 0
 	char buff[MESSAGE_SIZE];
	char MAC[20];
	strcat(MAC, "00:11:B5:22:44:66");
	memset(buff, 0, MESSAGE_SIZE);
	int m =0;
	int n = get_wlan_config(MAC, buff, &m);
	printf("sss:%d\n", n);
	printf("LLLLL:%d\n", m);
	print_mem(buff, m);
#endif

#if 1
	UNUSED(argc);
	UNUSED(argv);
	
	LOCK_Communi_INIT();

	if(pthread_create(&ClientId, NULL,AC_client_to_capwaps, &paClientId) == -1){
		perror("Create AC_client_to_capwaps pthread ER");
		sleep(1);
		DESTROY_Communi_SYNC();
		return 0;
	}
	if(pthread_create(&ServerId, NULL,AC_server_to_web, &paServerId) == -1){
		perror("Create AC_server_to_http pthread ER");
		sleep(1);
		DESTROY_Communi_SYNC();
		return 0;
	}
	if(pthread_join(ClientId,NULL)!=0){
		perror("call PTh_CardProcess fail");
		DESTROY_Communi_SYNC();
		return 1;
	}
	
	if(pthread_join(ServerId,NULL)!=0){
		perror("call pTh_ComProcess fail");
		DESTROY_Communi_SYNC();
		return 1;
	}
	DESTROY_Communi_SYNC();
	#endif
	return 0;
}
