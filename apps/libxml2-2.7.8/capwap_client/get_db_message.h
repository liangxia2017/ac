#ifndef GET_DB_MESSAGE_H

#define  WLAN_CMD 0x00000012 
#define		CW_COPY_MEMORY(dst, src, len)	bcopy(src, dst, len)
int get_wlan_config(char *mac, char * buff, int * lenght);

#endif