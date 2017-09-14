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
 * PURPOSE  :ͨ��MAC��ַ��ȡWLAN����		 
 *	 
 * RETURN   : 1 ��ȡ�ɹ���0��ȡʧ��
 *	
 * COMMENTS : 
 * CREATOR  :  ��ΰ                  DATE: 2011-02-15 15:47
 *==========================================================================*/
int get_wlan_config(char *mac, char * buff, int * lenght)
{
	int ret = 0;
	int m = 0;
	int j =0;
	int len = 0;
	int k = 0;
	int security_mode = 2; //��ȫģʽ
	ret = get_ap_accord_mac(mac);//����AP��mac��ȡAP������Ϣ
	if(ret == 0)
	{
		fprintf(stderr, "no ap config in db\a\n");	
		return 0;
	}
	ret  = get_ap_wlan_accord_gid(gapItem[0].ap_group_id); //����AP��ID��ȡwlan����

	if (ret == 0 )
	{
		fprintf(stderr, "no wlan config in db\a\n");	
		return 0;
	}
	printf("ddddd\n");
	while(j  < ret)
	{
			m = get_ap_wlan_security(gwlanItem[j].wlan_security_policy_id);
			//һ��wlan����gwlanItem[j]+gwlanSecurityPolicy[0]
					
			CWProtocolStore32(buff + len, WLAN_CMD); //��Ϣ��
			len += 4;
			k = 23 + strlen(gwlanItem[j].ssid); //wlan���ݳ���
			CWProtocolStore32(buff + len, k);
			len += 4;
			
			CWProtocolStore8(buff + len, 0x01);//radio id
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].wlan_item_id);  //wlan id
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].broadcast_ssid); //����SSID
			len += 1;
			CWProtocolStore8(buff + len, security_mode); //��ȫģʽ
			len += 1;
			CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].wlan_security_policy_id); //��ȫ����
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].max_user); //����û���
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].qos); //QOS	
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].mac_filter_policy);//MAC��ַ���˹���
			len += 1;
			CWProtocolStore16(buff + len, 0x0000); //�����ֶ�
			len += 2;
			CWProtocolStore16(buff + len, gwlanItem[j].vlan); //VLAN id
			len += 2;
			CWProtocolStore8(buff + len, 0x00);//MACģʽ
			len += 1;
			CWProtocolStore8(buff + len, gwlanItem[j].user_mode); //����ת��������ת����־
			len += 1;
			CWProtocolStore32(buff + len, gwlanItem[j].download_ssid); //����SSID����
			len += 4;
			CWProtocolStore32(buff + len, gwlanItem[j].download_user);//�����û�����
			len += 4;
			CWProtocolStore8(buff + len, strlen(gwlanItem[j].ssid)); //SSId����
			len += 1;
			CWProtocolStoreStr(buff + len, gwlanItem[j].ssid); //SSID
			len += strlen(gwlanItem[j].ssid);
			*lenght += len;
			#if  0
			if(1 == security_mode)  //��ȫģʽ
			{
				if(gwlanSecurityPolicy[0].wlan_security_policy_id == 0)
				{
					//openģʽ
				}
				if(gwlanSecurityPolicy[0].wlan_security_policy_id == 1)
				{
					// WAPI-psk
					CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].encrypt_type);//0-SMS4   ��������
					len += 1;
					CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].key_type);//������Կ����
					len += 1;
					CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].psk_key));//������Կ����
					len += 1;	
					CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].psk_key); //������Կ
					len += strlen(gwlanSecurityPolicy[0].psk_key);				
					
				}
				else if(gwlanSecurityPolicy[0].wlan_security_policy_id == 2)
				{
					//WAPI֤��
					CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].encrypt_type);//0-SMS4   ��������
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
						//��Կ���� 0��-64λ 1��-128λ 2��-152λ
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].key_len);
						len += 1;
						//��Կ����
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].key_tpye);
						len += 1;	
						//��������
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].default_key_index);
						len += 1;		
						//KEY 1 ����
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].key1));
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].key1); //��Կ1
						len += strlen(gwlanSecurityPolicy[0].key1);				
						//KEY 2 ����
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].key2));
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].key2); //��Կ2
						len += strlen(gwlanSecurityPolicy[0].key2);	
						//KEY 3 ����
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].key3));
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].key3); //��Կ3
						len += strlen(gwlanSecurityPolicy[0].key3);	
						//KEY 4 ����
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].key4));
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].key4); //��Կ4
						len += strlen(gwlanSecurityPolicy[0].key4);
						
						
						break;
					case 2:
						//11i-wpa/wpa2(EAP)
					case 7:
						//11i-wpa(EAP)
					case 8:
						//11i-wpa2(EAP)
						// ���ܷ�ʽ 0��CCMP  1��TKIP  2����ѡ����AP�Զ�ѡ��
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].encrypt_type);
						len += 1;		
						break;	
					case 3:
						//11i-wpa/wpa2-psk
						//��Կ����
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].key_len);
						len += 1;
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].key_type);//������Կ����
						len += 1;	
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].psk_key));//������Կ����
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].psk_key); //������Կ
						len += strlen(gwlanSecurityPolicy[0].psk_key);	
						
						break;
					case 4:
						//11i֤��
						
						break;
					case 5:
						//11i-wpa-psk
					case 6:
						//11i-wpa2-psk
						//���ܷ�ʽ0��-CCMP 1��-TKIP 2��-��ѡ��
						CWProtocolStore8(buff + len, gwlanSecurityPolicy[0].encrypt_type);
						len += 1;	
						CWProtocolStore8(buff + len, strlen(gwlanSecurityPolicy[0].psk_key));//������Կ����
						len += 1;	
						CWProtocolStoreStr(buff + len, gwlanSecurityPolicy[0].psk_key); //������Կ
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
				
			/*ע�ͼ��ܲ���û�ӽ���*/
			#endif
			
		j ++;
	}
	return ret;
	
}
