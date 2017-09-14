/********************************************************************
*  Copyright (C) 2005 China IWNCOMM Co.,Ltd                             *
* file: as_shm.c, the share memory manager function                     *
* author:       Dou xiangyang    : lwj_dxy@21cn.com                     *
* version:      1.0.1                                                   *
* see:          none                                                    *
* change:       2005-06-28      ����                                    *
*                                                                       *
*********************************************************************/
#ifndef __AS_SHM_H__
#define __AS_SHM_H__

#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/shm.h>

/**AS����״̬����*/
#define AS_START       0x01    //AS��������������
#define AS_EXIT        0x00    //AS�������Ѿ�ֹͣ

//#define USE_ATTRIBUTE_PACKED 1

#ifdef USE_ATTRIBUTE_PACKED
#define __ATTRIBUTE_PACK__   __attribute__((packed))
#else
#define __ATTRIBUTE_PACK__
#endif


/**AS�Ĺ����ڴ��������*/
struct as_shm_manager_t
{
	/**����AS�������Ĳ�������*/
	//AS������״̬
	unsigned char as_status;

	//AS��������ID
	pid_t as_pid;

	/**����CGIģ��Ĳ�������*/
	//AS��cgiģ��ĵ�¼��־��1���ѵ�¼��2��δ��¼
	unsigned char login;

	//Web�ͻ���ip��ַ��ASCII����ʽ����192.168.111.111
	unsigned char clt_ip[15];

	//��¼ʱ�䣬UTCʱ���ʽ
	unsigned long last_req_time;
} __ATTRIBUTE_PACK__;

typedef struct as_shm_manager_t as_shm_manager;

/**�����µĹ����ڴ滺����*/
void *shm_malloc(const unsigned char *shm_param, const size_t size);

/**��ȡ���й����ڴ滺������ַ*/
void *shm_get(const unsigned char *shm_param, const size_t size);

/**�ͷ����еĹ����ڴ�*/
int shm_free(void *ptr);

/**����AS������������״̬*/
int save_as_status(unsigned char *shm_param, unsigned char status);

/**����AS��������������ID*/
int save_as_pid(const unsigned char *shm_param, pid_t as_pid);

/**ɾ��������ռ�õĹ�����Դ*/
int del_as_shm(unsigned char *shm_param);

/**�ж�AS�������Ƿ�����*/
int as_is_start(unsigned char *shm_param);

/**��ȡAS�������Ľ���ID*/
pid_t get_as_pid(const unsigned char *shm_param);

//���ַ����еĲ��ɴ�ӡ�ַ��滻Ϊ'\0'
void strim(char *str);

/**���ӳ�ʱ�궨��*/
#define CGI_LOGIN 		0
#define CGI_LOGOUT   		1
#define CGI_TIMEOUT		2

#define CREATE_RANDOM		1
#define GET_RANDOM           	2

/**Web���ӳ�ʱ���Ͷ���*/
struct	http_timeout_t
{
	//cgiģ��ĵ�¼��־��1���ѵ�¼��2��δ��¼
	unsigned char is_login;

	//Web�ͻ���ip��ַ��ASCII����ʽ����192.168.111.111
	unsigned char clt_ip[128];

	//��¼ʱ�䣬UTCʱ���ʽ
	time_t begin_login_time;

	//���һ��Web����ʱ��
	time_t last_req_time;

	//���ӳ�ʱ������
	unsigned long timeout;

	//�����
	unsigned char random[20];

	//��¼�Ĵ������
	unsigned int login_fail;

};

typedef struct http_timeout_t http_timeout;

int http_timeout_save(unsigned char *shm_param, unsigned char *clt_ip, unsigned long timeout);

int http_timeout_update(unsigned char *shm_param, unsigned char *clt_ip);

int http_timeout_logout(unsigned char *shm_param, unsigned char *clt_ip);

http_timeout *http_timeout_ptr(unsigned char *shm_param);

int http_timeout_islogin(unsigned char *shm_param, unsigned char *clt_ip);

unsigned char *http_timeout_get_random(unsigned char *shm_param, int type);

#endif


