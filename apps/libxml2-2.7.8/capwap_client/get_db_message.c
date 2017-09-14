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



#include "myproc.h"
#include "capwap_msg.h"
#include "get_db_message.h"



// stores 8 bits in the message, increments the current offset in bytes
void CWProtocolStore8(char *msgPtr, unsigned char val) {
	CW_COPY_MEMORY(msgPtr,  &(val), 1);
}

// stores 16 bits in the message, increments the current offset in bytes
void CWProtocolStore16(char *msgPtr, unsigned short val) {
	val = htons(val);
	CW_COPY_MEMORY(msgPtr,  &(val), 2);
}

// stores 32 bits in the message, increments the current offset in bytes
void CWProtocolStore32(char *msgPtr, unsigned int val) {
	val = htonl(val);
	CW_COPY_MEMORY(msgPtr, &(val), 4);
}

void CWProtocolStoreStr(char *msgPtr, char *str) {
	int len = strlen(str);
	CW_COPY_MEMORY(msgPtr, str, len);
}

/*==========================================================================*
 * FUNCTION :int get_wlan_config(char *mac)
 * PURPOSE  :通过MAC地址获取WLAN配置		 
 *	 
 * RETURN   : 1 获取成功；0获取失败
 *	
 * COMMENTS : 
 * CREATOR  :  李伟                  DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_wlan_config(char *mac, char * buff, int * lenght)
{
	int ret = 0;
	int m = 0;
	int j =0;
	int len = 0;
	int k = 0;
	int security_mode = 2; //安全模式
	ret = get_ap_accord_mac(mac);//根据AP的mac获取AP基本信息
	if(ret == 0)
	{
		fprintf(stderr, "no ap config in db\a\n");	
		return 0;
	}
	ret  = get_ap_wlan_accord_gid(gapItem[0].ap_group_id); //根据AP组ID获取wlan配置

	if (ret == 0 )
	{
		fprintf(stderr, "no wlan config in db\a\n");	
		return 0;
	}
	printf("ddddd\n");
	while(j  < ret)
	{
			m = get_ap_wlan_security(gwlanItem[j].wlan_security_policy_id);
			//一个wlan配置gwlanItem[j]+gwlanSecurityPolicy[0]
					
			CWProtocolStore32(buff + len, WLAN_CMD); //消息号
			len += 4;
			k = 23 + strlen(gwlanItem[j].ssid); //wlan数据长度
			CWProtocolStore32(buff + len, k);
			len += 4;
			
			CWProtocolStore8(buff + len, 0x01);//radio id
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].wlan_item_id);  //wlan id
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].broadcast_ssid); //隐藏SSID
			len += 1;
			CWProtocolStore8(buff + len, security_mode); //安全模式
			len += 1;
			CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].wlan_security_policy_id); //安全策略
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].max_user); //最大用户数
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].qos); //QOS	
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].mac_filter_policy);//MAC地址过滤规则
			len += 1;
			CWProtocolStore16(buff + len, 0x0000); //保留字段
			len += 2;
			CWProtocolStore16(buff + len, gwlanItem[j].vlan); //VLAN id
			len += 2;
			CWProtocolStore8(buff + len, 0x00);//MAC模式
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].user_mode); //本地转发，集中转发标志
			len += 1;
			CWProtocolStore32(buff + len, gwlanItem[j].download_ssid); //基于SSID流控
			len += 4;
			CWProtocolStore32(buff + len, gwlanItem[j].download_user);//基于用户流控
			len += 4;
			CWProtocolStore8(buff + len, strlen(gwlanItem[j].ssid)); //SSId长度
			len += 1;
			CWProtocolStoreStr(buff + len, gwlanItem[j].ssid); //SSID
			len += strlen(gwlanItem[j].ssid);
			*lenght += len;
			#if  0
			if(1 == security_mode)  //安全模式
			{
				if(gwlanSecurityPolicy[0].wlan_security_policy_id == 0)
				{
					//open模式
				}
				if(gwlanSecurityPolicy[0].wlan_security_policy_id == 1)
				{
					// WAPI-psk
					CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].encrypt_type);//0-SMS4   加密类型
					len += 1;
					CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].key_type);//共享密钥类型
					len += 1;
					CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].psk_key));//共享密钥长度
					len += 1;	
					CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].psk_key); //共享密钥
					len += strlen(gwlanSecurityPolicy[0].psk_key);				
					
				}
				else if(gwlanSecurityPolicy[0].wlan_security_policy_id == 2)
				{
					//WAPI证书
					CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].encrypt_type);//0-SMS4   加密类型
					len += 1;
				}
	
					
			}
			else if(2 == security_mode)
			{
				switch(gwlanSecurityPolicy[0].wlan_security_policy_id )
					case 0:
						//open
						break;
					case 1:
						//11i-wep
						//密钥长度 0：-64位 1：-128位 2：-152位
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].key_len);
						len += 1;
						//密钥类型
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].key_tpye);
						len += 1;	
						//激活索引
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].default_key_index);
						len += 1;		
						//KEY 1 长度
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].key1));
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].key1); //密钥1
						len += strlen(gwlanSecurityPolicy[0].key1);				
						//KEY 2 长度
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].key2));
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].key2); //密钥2
						len += strlen(gwlanSecurityPolicy[0].key2);	
						//KEY 3 长度
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].key3));
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].key3); //密钥3
						len += strlen(gwlanSecurityPolicy[0].key3);	
						//KEY 4 长度
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].key4));
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].key4); //密钥4
						len += strlen(gwlanSecurityPolicy[0].key4);
						
						
						break;
					case 2:
						//11i-wpa/wpa2(EAP)
					case 7:
						//11i-wpa(EAP)
					case 8:
						//11i-wpa2(EAP)
						// 加密方式 0：CCMP  1：TKIP  2：自选择，由AP自动选择
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].encrypt_type);
						len += 1;		
						break;	
					case 3:
						//11i-wpa/wpa2-psk
						//密钥长度
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].key_len);
						len += 1;
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].key_type);//共享密钥类型
						len += 1;	
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].psk_key));//共享密钥长度
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].psk_key); //共享密钥
						len += strlen(gwlanSecurityPolicy[0].psk_key);	
						
						break;
					case 4:
						//11i证书
						
						break;
					case 5:
						//11i-wpa-psk
					case 6:
						//11i-wpa2-psk
						//加密方式0：-CCMP 1：-TKIP 2：-自选择
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].encrypt_type);
						len += 1;	
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].psk_key));//共享密钥长度
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].psk_key); //共享密钥
						len += strlen(gwlanSecurityPolicy[0].psk_key);	
						
						break;

					default:
							{
								fprintf(stderr, "get security_mode error in db\a\n");
								return 0;
							}
							
							
				
			}
			else
			{
				fprintf(stderr, "get security_mode error in db\a\n");
				return 0;
			}
				
			/*注释加密部分没加进来*/
			#endif
			
		j ++;
	}
	return ret;
	
}
