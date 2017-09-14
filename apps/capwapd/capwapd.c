#include "capwapd.h"

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
int port = 5246;
int event_sta_updown = 1;
struct runtime ac_runtime;
char trace[24] = {0};
static void usage()
{
	printf(
			"    -d     show debug messages\n"
			"    -B     run daemon in the background\n"
			"    -P     PID file\n"
			"    -p     port\n"
			"    -s     trace ap mac \n"
			"    -e     support event \n"
		   );
	exit(-1);
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
		case 'e':
			event_sta_updown  = atoi(optarg);
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
	if(capwap_init())
	{	
		print_info("init sock failed,try again after 5s\n");
		return 0;
	}
#endif
	eloop_run();
 out:
	/* Deinitialize all interfaces */
	eloop_destroy();

	os_daemonize_terminate(pid_file);

	return ret;
}