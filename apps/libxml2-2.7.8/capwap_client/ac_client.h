#ifndef	__AC_CLIENT_H

#include <fcntl.h>
#include <pthread.h>

#ifndef UNUSED
#define UNUSED(x)	(void)(x)
#endif

#define __USE_COMMUNITION_LOCK	1  // use pthread_mutex.

#ifdef __USE_COMMUNITION_LOCK
//The handle of the mutex for write log,only writing by one user once
pthread_mutex_t	hCommuniLock = PTHREAD_MUTEX_INITIALIZER;

#define LOCK_Communi_INIT()		  pthread_mutex_init(&hCommuniLock, NULL)
#define LOCK_Communi_SYNC()       pthread_mutex_lock(&hCommuniLock)
#define UNLOCK_Communi_SYNC()     pthread_mutex_unlock(&hCommuniLock)
#define DESTROY_Communi_SYNC()    pthread_mutex_destroy(&hCommuniLock)

#endif
typedef struct{
	unsigned char * meg_buff;   /*用于存放页面消息*/
	short int meg_len;  /*用于存放页面消息的长度*/
	short  meg_flag;   /*用于同步2线程*/
}Message_from_web_to_capwapd;

Message_from_web_to_capwapd message_info;
int socket_connect_to_capwapd;


extern int get_wlan_config(char *mac, char * buff, int * lenght);
#endif
