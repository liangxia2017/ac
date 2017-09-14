#include "locating.h"

static void handle_term(int sig, void *eloop_ctx, void *signal_ctx)
{
	printf("Signal %d received - terminating\n", sig);
	eloop_terminate();
}static void debug_on(int sig, void *eloop_ctx, void *signal_ctx)
{
	*(int *)signal_ctx = 1;
}

static void debug_off(int sig, void *eloop_ctx, void *signal_ctx)
{
	*(int *)signal_ctx = 0;
}
int debug = 0;
int port = 9010;
int period = 3;
int mode = 1;/*0-采集模式，1-定位模式*/
int refer_mode = 1;/*参考值的选择，0-由选项指定，1-采集值决定*/
int best_rssi = 57;
/*若信号的边界值，非采集值，但与三个ap的覆盖范围成反比*/
int weak_rssi = 38;
char trace[24] = {0};
static void usage()
{
	printf(
			"    -d     show debug messages\n"
			"    -B     run daemon in the background\n"
			"    -P     PID file\n"
			"    -p     port\n"
			"    -m     0-collect rssi ,1-locating  \n "
			"    -t     interval time of update db\n"
			"    -s     trace sta mac \n"
		   );
	exit(-1);
}


static void receive_ap_msg(int sock, void *eloop_ctx, void *user_ctx)
{	
	int read_len = 0, i = 0, j;
	unsigned char  message[512] = {0};
	unsigned char  *message_tmp = NULL;
	unsigned char ap_mac[6] = {0};
	unsigned char sta_mac[6] = {0};
	unsigned short rssi ;
      struct  sockaddr_in  from;	
      unsigned int fromlen = sizeof(from);
	unsigned short sta_num = 0;
	memset(message, 0, sizeof(message));
	unsigned char stamac[16] = {0};
	
	read_len =recvfrom(sock,  message,  sizeof(message), 0, (struct sockaddr *)&from, &fromlen);//接收报文信息
	message_tmp = message ;
	message_tmp += 18;
	memcpy(ap_mac, message_tmp, 6);  
	message_tmp += 8;
	sta_num = htons(*(unsigned short *)message_tmp)/8;
	//print_info("-------------------------------------\n");
	//print_info("STA_NUM: %d    AP:",sta_num);
	//dump_data(ap_mac, 6);
	message_tmp += 2;
	for(i=0;i< sta_num;i++)
	{	
		memset(sta_mac, 0, 6);
		memcpy(sta_mac, message_tmp, 6);   
		message_tmp += 6;
		rssi = htons(*(unsigned short *)message_tmp);    //终端的rssi值
		message_tmp +=2;
	//	print_info("RSSI:%d   STA:",rssi);
	//	dump_data(sta_mac, 6);
		rssi2db(ap_mac, rssi, sta_mac);//将收到的信息写入数据库
	}
}

/*监听函数注册*/
void ac_receive_init()
{	
	int sockfd;
	struct sockaddr_in saddr;	
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		sockfd = -1;
		close(sockfd);
	}
	memset(&saddr, 0, sizeof(saddr));	
	saddr.sin_family = AF_INET;	
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
	{
		sockfd = -1;
		close(sockfd);
	}
	eloop_register_read_sock(sockfd, receive_ap_msg, NULL, NULL);		
	
}

int main(int argc, char *argv[])
{
	int ret = 1, k;
	size_t i, j;
	int c, daemonize = 0;
	const char *pid_file = NULL;
	debug = 0;

	for (;;) {
		c = getopt(argc, argv, "Bdhp:m:t:P:r:b:w:s:");
		if (c < 0)
			break;
		switch (c) {
		case 'h':
			usage();
			break;
		case 'd':
			debug++;
			break;
		case 'B':
			daemonize++;
			break;		
		case 'P':
			pid_file = optarg;
			break;
		case 'p':
			port  = atoi(optarg);
			break;
		case 'm':
			mode  = atoi(optarg);
			break;
		case 't':
			period  = atoi(optarg);
			break;
		case 'r':
			refer_mode  = atoi(optarg);
			break;
		case 'b':
			best_rssi  = atoi(optarg);
			break;
		case 'w':
			weak_rssi  = atoi(optarg);
			break;
		case 's':
			strcpy(trace,optarg);
			break;

		default:
			usage();
			break;
		}
	}

	if (eloop_init(NULL)) 
	{
		print_info("eloop_init failed\n");
		return -1;	
	}

	eloop_register_signal_terminate(handle_term, NULL);
	eloop_register_signal(SIGUSR1, debug_on, &debug);
	eloop_register_signal(SIGUSR2, debug_off, &debug);

	if (daemonize && os_daemonize(pid_file)) {
		perror("daemon");
		goto out;
	}
#if 1
	while(init_db())
	{
		print_info("init db failed,try again after 5s\n");
		sleep(5);
	}
	eloop_register_timeout(0, 0, ac_receive_init, NULL, NULL);
	eloop_register_timeout(period, 0, calc_sta_cdn, NULL, NULL);
	eloop_register_timeout(period*100, 0, delete_db, NULL, NULL);
#endif
	eloop_run();

	ret = 0;

 out:
	/* Deinitialize all interfaces */
	

	eloop_destroy();

	os_daemonize_terminate(pid_file);

	return ret;
}