/********************************************************************
*  Copyright (C) 2005 China IWNCOMM Co.,Ltd                             *
* file: as_shm.c, the share memory manager function                     *
* author:       Dou xiangyang    : lwj_dxy@21cn.com                     *
* version:      1.0.1                                                   *
* see:          none                                                    *
* change:       2005-06-28      创建                                    *
*                                                                       *
*********************************************************************/
#ifndef __AS_SHM_H__
#define __AS_SHM_H__

#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/shm.h>

/**AS服务状态定义*/
#define AS_START       0x01    //AS服务器正在运行
#define AS_EXIT        0x00    //AS服务器已经停止

//#define USE_ATTRIBUTE_PACKED 1

#ifdef USE_ATTRIBUTE_PACKED
#define __ATTRIBUTE_PACK__   __attribute__((packed))
#else
#define __ATTRIBUTE_PACK__
#endif


/**AS的共享内存管理类型*/
struct as_shm_manager_t
{
	/**用于AS服务器的参数定义*/
	//AS的运行状态
	unsigned char as_status;

	//AS的主进程ID
	pid_t as_pid;

	/**用于CGI模块的参数定义*/
	//AS的cgi模块的登录标志，1：已登录，2：未登录
	unsigned char login;

	//Web客户端ip地址，ASCII串格式，如192.168.111.111
	unsigned char clt_ip[15];

	//登录时间，UTC时间格式
	unsigned long last_req_time;
} __ATTRIBUTE_PACK__;

typedef struct as_shm_manager_t as_shm_manager;

/**申请新的共享内存缓冲区*/
void *shm_malloc(const unsigned char *shm_param, const size_t size);

/**获取已有共享内存缓冲区首址*/
void *shm_get(const unsigned char *shm_param, const size_t size);

/**释放已有的共享内存*/
int shm_free(void *ptr);

/**保存AS服务器的运行状态*/
int save_as_status(unsigned char *shm_param, unsigned char status);

/**保存AS服务器的主进程ID*/
int save_as_pid(const unsigned char *shm_param, pid_t as_pid);

/**删除服务器占用的共享资源*/
int del_as_shm(unsigned char *shm_param);

/**判断AS服务器是否运行*/
int as_is_start(unsigned char *shm_param);

/**获取AS服务器的进程ID*/
pid_t get_as_pid(const unsigned char *shm_param);

//将字符串中的不可打印字符替换为'\0'
void strim(char *str);

/**连接超时宏定义*/
#define CGI_LOGIN 		0
#define CGI_LOGOUT   		1
#define CGI_TIMEOUT		2

#define CREATE_RANDOM		1
#define GET_RANDOM           	2

/**Web连接超时类型定义*/
struct	http_timeout_t
{
	//cgi模块的登录标志，1：已登录，2：未登录
	unsigned char is_login;

	//Web客户端ip地址，ASCII串格式，如192.168.111.111
	unsigned char clt_ip[128];

	//登录时间，UTC时间格式
	time_t begin_login_time;

	//最后一次Web请求时间
	time_t last_req_time;

	//连接超时的秒数
	unsigned long timeout;

	//随机数
	unsigned char random[20];

	//登录的错误次数
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


