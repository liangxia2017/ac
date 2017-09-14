#include <stdio.h>
#include <time.h>

#include "as_shm.h"
#include <sys/stat.h>

/**�����µĹ����ڴ滺����*/
void *shm_malloc(const unsigned char *shm_param, const size_t size)
{
	key_t keyid = IPC_PRIVATE;
	int   shmid;
	void  *shmaddr;

	if (shm_param == NULL)
		return NULL;

	//��ù����ڴ��key
	if ((keyid = ftok(shm_param, 0)) == -1)
		return NULL;

	//��ù����ڴ��id,�����û�����Ļ�,�ͽ���
	if ((shmid = shmget(keyid, size + sizeof(shmid), IPC_CREAT | 0666)) == -1)
		return NULL;

	//��ù����ڴ��ַ
	if ((shmaddr = shmat(shmid, NULL, 0)) == (void *) - 1)
		return NULL;

	memcpy(shmaddr, &shmid, sizeof(shmid));
	return shmaddr + sizeof(shmid);
}

/**��ȡ���й����ڴ滺������ַ*/
void *shm_get(const unsigned char *shm_param, const size_t size)
{
	key_t keyid = IPC_PRIVATE;
	int   shmid;
	void  *shmaddr;
	char str[128];

	if (shm_param == NULL)
		return NULL;

	//��ù����ڴ��key
	if ((keyid = ftok(shm_param, 0)) == -1)
		return NULL;

	bzero(str, 128);

	//��ù����ڴ��id,�����û�����Ļ�,�ͽ���
	if ((shmid = shmget(keyid, size + sizeof(shmid), 0)) == -1)
		return NULL;

	//��ù����ڴ��ַ
	if ((shmaddr = shmat(shmid, NULL, 0)) == (void *) - 1)
		return NULL;

	return shmaddr + sizeof(shmid);
}

/**�ͷ����еĹ����ڴ�*/
int shm_free(void *ptr)
{
	int  shmid;

	//��ȡ�����ڴ��id
	if ((ptr == NULL) || (memcpy(&shmid, ptr - sizeof(shmid), sizeof(shmid)) == NULL) || (shmid < 0))
		return -1;

	//ɾ�������ڴ�
	return shmctl(shmid, IPC_RMID, NULL);
}

/**����AS������������״̬*/
int save_as_status(unsigned char *shm_param, unsigned char status)
{
	//����AS��״̬��Ϣ�������ڴ���
	as_shm_manager *ptr = NULL;

	//��ȡ�������ṹ����ַ
	if ((ptr = shm_malloc(shm_param, sizeof(as_shm_manager))) == NULL)
		return -1;

	ptr->as_status = status;
	return 0;
}

/**����AS��������������ID*/
int save_as_pid(const unsigned char *shm_param, pid_t as_pid)
{
	//����AS�������̵������ڴ���
	as_shm_manager *ptr = NULL;

	//��ȡ�������ṹ����ַ
	if ((ptr = shm_malloc(shm_param, sizeof(as_shm_manager))) == NULL)
		return -1;

	ptr->as_pid = as_pid;
	return 0;
}

/**ɾ��������ռ�õĹ�����Դ*/
int del_as_shm(unsigned char *shm_param)
{
	as_shm_manager *ptr = NULL;

	//��ȡ�������ṹ����ַ
	if ((ptr = shm_get(shm_param, sizeof(as_shm_manager))) == NULL)
		return -1;

	//�ͷŹ������ṹ��
	return shm_free(ptr);
}

/**�ж�AS�������Ƿ�����*/
int as_is_start(unsigned char *shm_param)
{
	as_shm_manager *ptr = NULL;

	//��ȡ�������ṹ����ַ
	if ((ptr = shm_get(shm_param, sizeof(as_shm_manager))) == NULL)
		return AS_EXIT;

	return ptr->as_status;
}

/**��ȡAS�������Ľ���ID*/
pid_t get_as_pid(const unsigned char *shm_param)
{
	as_shm_manager *ptr = NULL;

	//��ȡ�������ṹ����ַ
	if ((ptr = shm_get(shm_param, sizeof(as_shm_manager))) == NULL)
		return AS_EXIT;

	return ptr->as_pid;
}


void strim(char *str)
{
	int len = strlen(str);
	char temp_str[1024];

	if (len > 1024)
		return ;

	bzero(temp_str, 1024);
	sscanf(str, "%s", temp_str);

	if (strlen(temp_str) < len)
	{
		bzero(str, len);
		memcpy(str, temp_str, strlen(temp_str));
	}
}

int http_timeout_save(unsigned char *shm_param, unsigned char *clt_ip, unsigned long timeout)
{
	//����AS�������̵������ڴ���
	http_timeout *ptr = NULL;

	if ((shm_param == NULL) || (clt_ip == NULL))
		return -1;

	//��ȡ�������ṹ����ַ
	if ((ptr = shm_get(shm_param, sizeof(http_timeout))) == NULL)
	{
		if ((ptr = shm_malloc(shm_param, sizeof(http_timeout))) == NULL)
			return -1;
	}

	if ((strlen(clt_ip) == 0) || (strlen(clt_ip) > sizeof(ptr->clt_ip)))
		return -1;

	ptr->is_login = CGI_LOGIN;
	bzero(ptr->clt_ip, sizeof(ptr->clt_ip));
	strncpy(ptr->clt_ip, clt_ip, strlen(clt_ip));
	time(&ptr->begin_login_time);
	ptr->last_req_time = ptr->begin_login_time;
	ptr->timeout = timeout;
	memset(ptr->random, 0 , sizeof(ptr->random));
	return 0;
}

int http_timeout_update(unsigned char *shm_param, unsigned char *clt_ip)
{
	//����AS�������̵������ڴ���
	http_timeout *ptr = NULL;
	time_t t;
	time(&t);

	if ((shm_param == NULL) || (clt_ip == NULL))
		return -1;

	//��ȡ�������ṹ����ַ
	if ((ptr = shm_get(shm_param, sizeof(http_timeout))) == NULL)
	{
		return -1;
	}

	if ((ptr->is_login == CGI_LOGOUT) || (strcmp(ptr->clt_ip, clt_ip) != 0))
		return -1;

	if (ptr->is_login == CGI_TIMEOUT)
	{
		return 0;
	}

	if ((t - ptr->last_req_time) >= ptr->timeout)
	{
		ptr->is_login = CGI_TIMEOUT;
		return 0;
	}

	ptr->last_req_time = t;
	return 1;
}


int http_timeout_logout(unsigned char *shm_param, unsigned char *clt_ip)
{
	//����AS�������̵������ڴ���
	http_timeout *ptr = NULL;

	if ((shm_param == NULL) || (clt_ip == NULL))
		return -1;

	//��ȡ�������ṹ����ַ
	if ((ptr = shm_get(shm_param, sizeof(http_timeout))) == NULL)
		return -1;

	if (strcmp(ptr->clt_ip, clt_ip) != 0)
		return -1;

	ptr->is_login = CGI_LOGOUT;
	return 0;
}

http_timeout *http_timeout_ptr(unsigned char *shm_param)
{
	//��ȡ�������ṹ����ַ
	return shm_get(shm_param, sizeof(http_timeout));
}


int http_timeout_islogin(unsigned char *shm_param, unsigned char *clt_ip)
{
	//����AS�������̵������ڴ���
	http_timeout *ptr = NULL;
	time_t t;
	time(&t);

	if ((shm_param == NULL) || (clt_ip == NULL))
		return 0;

	//��ȡ�������ṹ����ַ
	if ((ptr = shm_get(shm_param, sizeof(http_timeout))) == NULL)
		return 0;

	if ((strcmp(ptr->clt_ip, clt_ip) != 0)
			&& (ptr->is_login == CGI_LOGIN)
			&& ((t - ptr->last_req_time) < ptr->timeout))
		return 1;

	return 0;
}


unsigned char *http_timeout_get_random(unsigned char *shm_param, int type)
{
	//����AS�������̵������ڴ���
	http_timeout *ptr = NULL;
	int rand_num = 0;

	if (shm_param == NULL)
		return NULL;

	//��ȡ�������ṹ����ַ
	if ((ptr = shm_get(shm_param, sizeof(http_timeout))) == NULL)
	{
		if (type == GET_RANDOM)
			return NULL;

		if ((ptr = shm_malloc(shm_param, sizeof(http_timeout))) == NULL)
			return NULL;
	}

	if (type == CREATE_RANDOM)
	{
		bzero(ptr->random, sizeof(ptr->random));
		srand(time(NULL));
		rand_num = rand();
		sprintf(ptr->random, "%u", rand_num);
		ptr->random[strlen(ptr->random)] = '\0';
	}

	return ptr->random;
}


