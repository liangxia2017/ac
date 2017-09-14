
#include <sqlite3.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "eloop.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdarg.h>
 #include <signal.h>
 void print_info(const char *fmt, ...);
 void calc_sta_cdn();
 void delete_db();

extern int debug ;
extern int port ;
extern int period ;
extern int mode;
extern int refer_mode;
extern int best_rssi ;
extern int weak_rssi ;
extern char trace[24] ;
char *errmsg;
 sqlite3 *pFile;

#ifndef MAC2STR
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#define MACSTR2 "%02x%02x%02x%02x%02x%02x"
#endif

