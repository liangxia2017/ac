/*
 * CGI-BIN library of functions
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <sys/ioctl.h>
//#include <net/if.h>
#include <arpa/inet.h>
#include <errno.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include "prop.h"
#include "config.h"

#ifdef MEM_DEBUG
#include "memwatch.h"
#else
#include <stdlib.h>
#endif


/************************** 2012-06-11 START *****************************/
/* 
	yangzhicong 2012-06-11 解决多个进程同时并发访问、
	 读写某个配置文件导致该文件内容被破坏的问题

TODO: 
1.
	虽然在使能文件锁的前提下，函数load_prop_safe 和
	函数save_global_conf_safe 各自本身具有原子性，但是
	如果写文件依赖于读文件的结果，那么要谨防
	出现下面代码序列的并发问题(下面的代码只是示例):

		load_prop_safe(SEP_EQUAL, CONF_FILE, properties);
		get_prop("ESSID", essid, properties, prop_count);
		free_prop(properties, prop_count);
		memset(new_essid, 0, sizeof(new_essid));
		snprintf(new_essid, sizeof(new_essid), "essid - %d - %d",getpid(), time(NULL));
		if(strlen(essid)) {
			save_global_conf_safe(SEP_EQUAL, CONF_FILE, "", "ESSID", new_essid);
		}
	上面的代码序列中，函数load_prop_safe 和函数save_global_conf_safe
	之间可能有新的进程修改了配置文件。

2. 
	函数load_prop_safe 和函数save_global_conf_safe 使用的是非强制锁，也
	就是说，如果有进程没有调用这2 个函数来访问文件，该进
	程访问文件是不受函数load_prop_safe 和函数save_global_conf_safe中使用
	的锁的限制的。
*/

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FILELOCK_ENABLE  1
#define USE_NEW_VERSION 1
#define DEBUGGING 0
#define LINE_BUF_LEN 1024
#define FILE_BUF_LEN 65535

#define READ_LINE_OK 0
#define READ_LINE_ERROR -1
#define READ_LINE_EOF 1


#define	READ_LOCK(fd, offset, whence, len) \
			lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define	READW_LOCK(fd, offset, whence, len) \
			lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define	WRITE_LOCK(fd, offset, whence, len) \
			lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define	WRITEW_LOCK(fd, offset, whence, len) \
			lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define	UNLOCK(fd, offset, whence, len) \
			lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))
			

#define	READFILE_LOCK(fd) \
			lockfile_reg((fd), F_SETLK, F_RDLCK)
#define	READFILEW_LOCK(fd) \
			lockfile_reg((fd), F_SETLKW, F_RDLCK)
#define	WRITEFILE_LOCK(fd) \
			lockfile_reg((fd), F_SETLK, F_WRLCK)
#define	WRITEFILEW_LOCK(fd) \
			lockfile_reg((fd), F_SETLKW, F_WRLCK)
#define	FILE_UNLOCK(fd) \
			lockfile_reg((fd), F_SETLK, F_UNLCK)


#if DEBUGGING
#define DEBUG_FILE "/tmp/lockfile_debug.log"
static FILE *debug_log_file = NULL;

static void open_file_for_debug(void)
{
	static int is_try_open = 0;
	if(!is_try_open) {
		debug_log_file = fopen(DEBUG_FILE, "w");
		is_try_open = 1;
	}
}
#define DEBUG_PRINTF(_fmt, ...) \
do {\
	open_file_for_debug();\
	if(debug_log_file) {\
		fprintf(debug_log_file, _fmt, __VA_ARGS__);\
	} \
} while(0)


#else  /* !DEBUGGING */
#define DEBUG_PRINTF(_fmt, ...) 
#endif



#if FILELOCK_ENABLE
static int
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock	lock;

	lock.l_type = type;		/* F_RDLCK, F_WRLCK, F_UNLCK */
	lock.l_start = offset;	/* byte offset, relative to l_whence */
	lock.l_whence = whence;	/* SEEK_SET, SEEK_CUR, SEEK_END */
	lock.l_len = len;		/* #bytes (0 means to EOF) */

	return(fcntl(fd, cmd, &lock));
}

static int
lockfile_reg(int fd, int cmd, int type)
{
	struct flock	lock;

	lock.l_type = type;		/* F_RDLCK, F_WRLCK, F_UNLCK */
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;

	return(fcntl(fd, cmd, &lock));
}
#else  /* !FILELOCK_ENABLE */
static int
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	return 0;
}

static int
lockfile_reg(int fd, int cmd, int type)
{
	return 0;
}
#endif

static int 
readline_with_nobuffer(int fd, char *buf, int len)
{
	char ch;
	int index = 0;	
	ssize_t cnt;

	if(fd <0 || len <= 0 || buf == NULL)  {
		DEBUG_PRINTF("%s, %d: Error: fd = %d, len = %d, buf = %p\n",
			__func__, __LINE__, fd, len, buf);
		return READ_LINE_ERROR;
	}

	while (((cnt = read(fd, &ch, sizeof(ch))) == sizeof(ch)) && ch != '\n') {
		if(index >= len -1) { 
			DEBUG_PRINTF("%s, %d: Warnning: No enough room\n", __func__, __LINE__);
			buf[len-1] = '\0';
			return READ_LINE_ERROR;
		} else {
			buf[index++] = ch;
		}
	}

	buf[index] = '\0'; 
	return (cnt == 0 ? READ_LINE_EOF : READ_LINE_OK);
	
}

#if USE_NEW_VERSION
/**将ini配置文件中的配置信息读取到prop_data类型数组中*/
int load_prop(char type, char *file_name, prop_data properties[])
{
	int fd;
	char line[LINE_BUF_LEN];
	int prop_count = 0;
	int ret;

	DEBUG_PRINTF("PID %d: I will open file [%s] for load prop\n", getpid(), file_name);
	fd = open(file_name, O_RDONLY, S_IRUSR);
	if(fd < 0) {
		DEBUG_PRINTF("%s, %d: PID %d: open [%s] fail <errno = %d>\n", 
			__func__, __LINE__, getpid(), file_name, errno);
		return 0;
	} 

	memset(line, 0, sizeof(line));

	if(READFILEW_LOCK(fd) == -1) {
		DEBUG_PRINTF("%s, %d: Lock [%s] for read fail !!\n", __func__, __LINE__, file_name);
		close(fd); 
		return 0;
	} 

#if FILELOCK_ENABLE
	DEBUG_PRINTF("PID %d: LOCKed %s for read\n", getpid(), file_name);
#endif
	while ( ((ret = readline_with_nobuffer(fd, line, sizeof(line))) == READ_LINE_OK)
			|| (ret == READ_LINE_EOF && strlen(line)) )  {
		if (strchr(line, '\n') != NULL)
			*(strchr(line, '\n')) = '\0';

		if (strchr(line, '\r') != NULL)
			*(strchr(line, '\r')) = '\0';
		
		//解析单行配置中的Key和Value
		if (get_pr (type, line, (prop_data *) & properties[prop_count]))
		{
			prop_count++;
		}
		
		memset(line, 0, sizeof(line));
	}

	if (ret == READ_LINE_ERROR) {
		DEBUG_PRINTF("%s, %d: some error happen\n", __func__, __LINE__);
		close(fd);  // imply FILE_UNLOCK(fd);
		return 0;
	}

	if (ret != READ_LINE_EOF) {
		DEBUG_PRINTF("%s, %d: Warnning: EOF lost\n", __func__, __LINE__);
	}

	//返回总共解析成功的参数对个数
	close(fd);  // imply FILE_UNLOCK(fd);
#if FILELOCK_ENABLE
	DEBUG_PRINTF("PID %d: UNLOCKed %s for read\n", getpid(), file_name);
#endif
	return prop_count;
}


int save_global_conf(char type, char* filename, char *command, char *key, const char *value)
{
	char line[LINE_BUF_LEN], tmp_line[LINE_BUF_LEN];
	char file_buf[FILE_BUF_LEN];
	char *index;
	char old_value[1024];
	char separator[2];
	int is_exist = 0;
	int changed = 0;
	int ret;
	int fd;

	DEBUG_PRINTF("PID %d: I will open file [%s] for save global conf\n", getpid(), filename);
	fd = open(filename, O_RDWR, S_IRUSR | S_IWUSR);
	if(fd < 0) {
		DEBUG_PRINTF("%s, %d: PID %d: open [%s] fail <errno = %d>\n", 
			__func__, __LINE__, getpid(), filename, errno);
		return 0;
	}

	strcpy(separator, (type == SEP_EQUAL ? "=" : type == SEP_SPACE ? " " : "\t"));
	type = separator[0];

	strcpy(tmp_line, command);
	strcat(tmp_line, key);
	//add by xieshaojun 080703
	sprintf(tmp_line + strlen(tmp_line), "%c", type);
	
	memset(file_buf, 0, sizeof(file_buf));
	memset(line, 0, sizeof(line));
	
	if(WRITEFILEW_LOCK(fd) == -1) {
		DEBUG_PRINTF("%s, %d: Lock [%s] for write fail !!\n",__func__, __LINE__, filename);
		close(fd); 
		return 0;
	}
	
#if FILELOCK_ENABLE	
	DEBUG_PRINTF("PID %d: LOCKed %s for write\n", getpid(), filename);
#endif

	while ( ((ret = readline_with_nobuffer(fd, line, sizeof(line))) == READ_LINE_OK)
			|| (ret == READ_LINE_EOF && strlen(line)) )  {
		if (((index = strstr(line, tmp_line)) != NULL) && (index == line))
		{
			is_exist = 1;
//add by xieshaojun 20090206
//网通：日志文件中应记录用户接入信息、配置更改信息。
#if 1	
			strcpy(old_value, index + strlen(tmp_line));
			char * t = strrchr(old_value, SEP_Q_MARK);

			if (t)		*t = '\0';

			t = strchr(old_value, SEP_Q_MARK);

			if (t)		t++;
			else		t = old_value;

			if (0 != strcmp(t, value))
				changed = 1;

#endif
			if (type != '0')
				snprintf(file_buf + strlen(file_buf), sizeof(file_buf) - strlen(file_buf), 
					"%s%s%c%c%s%c\n", command, key, type, SEP_Q_MARK, value, SEP_Q_MARK);
			else
				snprintf(file_buf + strlen(file_buf), sizeof(file_buf) - strlen(file_buf), 
					 "%s%s%c%s%c\n", command, key, SEP_Q_MARK, value, SEP_Q_MARK);
		} else {
			snprintf(file_buf + strlen(file_buf), sizeof(file_buf) - strlen(file_buf), "%s\n", line);
		}

		memset(line, 0, sizeof(line));
	}

	if (ret == READ_LINE_ERROR) {
		DEBUG_PRINTF("%s, %d: some error happen\n", __func__, __LINE__);
		close(fd);  // imply FILE_UNLOCK(fd);
		return 0;
	}

	if (!is_exist)
	{
		if (type != '0')
		{
			//modify xieshaojun 2008-05-12
			//fprintf(fidOut,"%s%s%c%s\n",command,key,type,value);
			snprintf(file_buf + strlen(file_buf), sizeof(file_buf) - strlen(file_buf), 
				 "%s%s%c\"%s\"\n", command, key, type, value);
		}
		else
		{
			snprintf(file_buf + strlen(file_buf), sizeof(file_buf) - strlen(file_buf), 
				"%s%s%s\n", command, key, value);
		}
		
		changed = 1;
	}

	if (ret != READ_LINE_EOF) {
		DEBUG_PRINTF("%s, %d: Warnning: EOF lost\n", __func__, __LINE__);
	}

	if (changed) {
		ftruncate(fd, 0);
		lseek(fd, 0, SEEK_SET);  // ftruncate 不会将offset 复位
		write(fd, file_buf, strlen(file_buf));
	}

	close(fd);  // imply FILE_UNLOCK(fd);
#if FILELOCK_ENABLE		
	DEBUG_PRINTF("PID %d: UNLOCKed %s for write\n", getpid(), filename);
#endif
	
	return 1;
}

int save_prop (char type, char *filename, prop_data properties[], int count)
{
	int i;
	int fd;
	int ret;
	int is_changed = 0;
	char line[LINE_BUF_LEN];
	char file_buf[FILE_BUF_LEN];
	char tmp_line[LINE_BUF_LEN];
	char tmp_str[LINE_BUF_LEN];
	char tmp_fname[LINE_BUF_LEN];
	char separator[2] = {type, 0};

	DEBUG_PRINTF("PID %d: I will open file [%s] for save prop\n", getpid(), filename);
	fd = open(filename, O_RDWR, S_IRUSR | S_IWUSR);
	if(fd < 0) {
		DEBUG_PRINTF("%s, %d: PID %d: open [%s] fail <errno = %d>\n", 
			__func__, __LINE__, getpid(), filename, errno);
		return 0;
	} 

	memset(file_buf, 0, sizeof(file_buf));
	memset(line, 0, sizeof(line));

	if(WRITEFILEW_LOCK(fd) == -1) {
		DEBUG_PRINTF("%s, %d: Lock [%s] for write fail !!\n", __func__, __LINE__, filename);
		close(fd); 
		return 0;
	}
	
#if FILELOCK_ENABLE	
	DEBUG_PRINTF("PID %d: LOCKed %s for write\n", getpid(), filename);
#endif
	while ( ((ret = readline_with_nobuffer(fd, line, sizeof(line))) == READ_LINE_OK)
			|| (ret == READ_LINE_EOF && strlen(line)) )  {
			
		i = 0;

		while ((i < strlen (line)) && (line[i] != type))
		{
			i++;
		};

		memset (tmp_str, 0, sizeof (tmp_str));
		strncpy (tmp_str, line, i);

		for (i = 0; i < count; i++)
		{
			if (strcmp (tmp_str, properties[i].key) == 0)
			{
				memset(tmp_line, 0, sizeof(tmp_line));
				sprintf (tmp_line, "%s%s%c%s%c",
					 properties[i].key, separator,  SEP_Q_MARK, properties[i].value, SEP_Q_MARK);

				if (0 != strcmp(line, tmp_line))
				{
					strcpy(line, tmp_line);
					is_changed  =1;
				}

				break;
			}
		}

		snprintf(file_buf + strlen(file_buf), sizeof(file_buf) - strlen(file_buf), "%s\n", line);
		memset(line, 0, sizeof(line));
	}


	if (is_changed) {
		ftruncate(fd, 0);
		lseek(fd, 0, SEEK_SET);  // ftruncate 不会将offset 复位
		write(fd, file_buf, strlen(file_buf));
	}

	close(fd);  // imply FILE_UNLOCK(fd);
#if FILELOCK_ENABLE	
	DEBUG_PRINTF("PID %d: UNLOCKed %s for write\n", getpid(), filename);
#endif
	return 1;

}
#endif  /* USE_NEW_VERSION */

/************************** 2012-06-11  END *****************************/

char *strip (char *string, char delimiter)
{
	register int x, y;
	y = strlen(string);
	x = 0;

	while ((x < y) && (string[x] == delimiter))
		x++;

	while ((y > 0) && (string[y - 1] == delimiter))
		y--;

	string[y] = 0;
	return string + x;
}

/**解析Key=Value或Key Value形式的参数对*/
int get_pr (char type, char *line, prop_data *prop)
{
	char *index;
	char *keyptr;

	//假如该行是注释行，则返回0
	if (line[0] != '#')
	{
		// modified by kWlW, separator changed to this  vvv  from "="
		//跳过配置头信息setenv
		keyptr = (char *)strstr(line, "setenv ");	//finds the first occurrence of the substring 'setenv' in line

		if (keyptr != NULL) line = keyptr + 7;   /* Add size of 'setenv ' string */

		//查找分割符'='或' '的位置
		if ((index = strstr (line,
				     type == SEP_EQUAL ? "=" : type == SEP_SPACE ? " " : "\t")) != NULL)
		{
			*index = '\0';

			if ((prop->key = malloc (strlen (line) + 1)) != NULL &&
					(prop->value = malloc (strlen ( index + 1) + 1)) != NULL)
			{
				index++;
				sscanf(line, "%s", prop->key);
				strcpy (prop->value, index);
				return 1;
			}
		}
	}

	return 0;
}
#if 0
int load_from_str (char type, char *str, prop_data properties[])
{
	char line[254];
	int len;
	int prop_count = 0;

	while (str != NULL)
	{
		len = sscanf (str, "%[^\n]\n", line);

		if (get_pr (type, line, (prop_data *) & properties[prop_count]))
		{
			prop_count++;
		}

		str += len;
	}

	return prop_count;
}
#endif

#if (!USE_NEW_VERSION)
/**将ini配置文件中的配置信息读取到prop_data类型数组中*/
int load_prop (char type, char *file_name, prop_data properties[])
{
	FILE *fid;
	char line[1024];
	int prop_count = 0;

	//打开配置文件
	if ((fid = fopen (file_name, "r")) == NULL)
		return 0;

	while (!feof (fid))
	{
		//读取每一行
		fgets(line, 1024, fid);

		if (feof(fid)) break;

		if (strchr(line, '\n') != NULL)
			*(strchr(line, '\n')) = '\0';

		if (strchr(line, '\r') != NULL)
			*(strchr(line, '\r')) = '\0';

		//解析单行配置中的Key和Value
		if (get_pr (type, line, (prop_data *) & properties[prop_count]))
		{
			prop_count++;
		}
	}

	//关闭文件
	fclose (fid);
	//返回总共解析成功的参数对个数
	return prop_count;
}
#endif /* #if (!USE_NEW_VERSION) */
#if 0
int load_prop_old (char type, char *file_name, prop_data properties[])
{
	FILE *fid;
	char line[1024];//jwchang
	int prop_count = 0;

	if ((fid = fopen (file_name, "r")) == NULL)
		return 0;

	while (!feof (fid))
	{
		//fscanf (fid, "%[^\n]\n", line);
		fgets(line, 1024, fid);
		line[strlen(line)-1] = '\0';

		if (!feof(fid))
		{
			if (get_pr (type, line, (prop_data *) & properties[prop_count]))
			{
				prop_count++;
			}
		}
	}

	fclose (fid);
	return prop_count;
}

int service_action (char type, char *filename, char *service, char action)
{
	FILE *fidIn, *fidOut = NULL;
	char line[254];
	char tmp_str[512];
	char tmp_str2[254];
	char tmp_fname[254];
	char service_commented[32];
	char separator[2];
	int i;
	separator[0] = type;
	separator[1] = 0;

	if ((fidIn = fopen (filename, "r")) == NULL)
		return 0;

	strcpy (tmp_fname, filename);
	strcat (tmp_fname, ".cgi");

	if (action != WEBVERIFY)
		if ((fidOut = fopen (tmp_fname, "w")) == NULL)
			return 0;

	while (!feof (fidIn))
	{
		fscanf (fidIn, "%[^\n]\n", line);
		i = 0;

		while ((i < strlen (line)) && (line[i] != type))
		{
			i++;
		};

		memset (tmp_str, 0, sizeof (tmp_str));

		strncpy (tmp_str, line, i);

		switch (action)
		{
		case ENABLE:
			{
				sprintf (service_commented, "#%s", service);

				/* printf("[%s]\n",tmp_str); */
				if (strcmp (tmp_str, service_commented) == 0)
				{
					for (i = 0; i <= (strlen (line) - 2); i++)
					{
						tmp_str2[i] = line[i + 1];
					};

					tmp_str2[strlen (line) - 1] = '\0';

					//sprintf(tmp_str2,"#%s",line);
					strcpy (line, tmp_str2);
				}
			}
			break;
		case DISABLE:
			{
				if (strcmp (tmp_str, service) == 0)
				{
					sprintf (tmp_str2, "#%s", line);
					strcpy (line, tmp_str2);
				}
			}
			break;
		case WEBVERIFY:
			{
				if (strcmp (tmp_str, service) == 0)
				{
					return (SERVICE_ON);
				}
			}
			break;
		default:
			printf (":)");
			break;
		};

		if (action != WEBVERIFY)
			fprintf (fidOut, "%s\n", line);
	}

	fclose (fidIn);

	if (action != WEBVERIFY)
	{
		fclose (fidOut);
		unlink (filename);
		rename (tmp_fname, filename);
	};

	return 1;
}

int save_prop_old (char type, char *filename, prop_data properties[], int count)
{
	FILE *fidIn, *fidOut;
	char line[1024];
	char tmp_str[2048];
	char tmp_fname[1024];
	char separator[2];
	int i;
	separator[0] = type;
	separator[1] = 0;

	if ((fidIn = fopen (filename, "r")) == NULL)
		return 0;

	strcpy (tmp_fname, filename);
	strcat (tmp_fname, ".cgi");

	if ((fidOut = fopen (tmp_fname, "w")) == NULL)
		return 0;

	while (!feof (fidIn))
	{
		fscanf (fidIn, "%[^\n]\n", line);
		i = 0;

		while ((i < strlen (line)) && (line[i] != type))
		{
			i++;
		};

		memset (tmp_str, 0, sizeof (tmp_str));

		strncpy (tmp_str, line, i);

		for (i = 0; i < count; i++)
		{
			/* printf("[%s]\n",tmp_str); */
			if (strcmp (tmp_str, properties[i].key) == 0)
			{
				sprintf (line, "%s%s%s", properties[i].key, separator,
					 properties[i].value);
			}
		}

		fprintf (fidOut, "%s\n", line);
	}

	fclose (fidIn);
	fclose (fidOut);
	unlink (filename);
	rename (tmp_fname, filename);
	return 1;
}
#endif
#if 1
void get_logined_user(char * user_name)
{
	prop_data properties[KEYS_MAX];
	int prop_count = 0;

	if (!user_name)
		return;

	strcpy(user_name, "");
	prop_count = load_prop(SEP_EQUAL, PASSWD_CONF, properties);
	get_prop("user", user_name, properties, prop_count);
	free_prop(properties, prop_count);
}
#endif

#if (!USE_NEW_VERSION)
int save_prop (char type, char *filename, prop_data properties[], int count)
{
	FILE *fidIn, *fidOut;
	char line[1024];
	char tmp_line[1024];
	char change_msg[1024];
	char tmp_str[2048];
	char tmp_fname[1024];
	char separator[2];
	int i;
	separator[0] = type;
	separator[1] = 0;
	memset(change_msg, 0, sizeof(change_msg));
	char user_name[64];
	get_logined_user(user_name);
#if 0                                                                               /*edit by liwei, 20100406 */
	snprintf(change_msg + strlen(change_msg), (sizeof(change_msg) - strlen(change_msg)),
		 "[user:%s] %s changes:", user_name, filename + strlen("/etc/IWNCOMM/"));
	//snprintf(change_msg + strlen(change_msg), (sizeof(change_msg)-strlen(change_msg)), "config change to:");
#else
	snprintf(change_msg + strlen(change_msg), (sizeof(change_msg) - strlen(change_msg)),
		 "[user:%s] %s changes:", user_name, filename + strlen("/etc/ap11n/"));
#endif

	if ((fidIn = fopen (filename, "r")) == NULL)
		return 0;

	strcpy (tmp_fname, filename);
	strcat (tmp_fname, ".cgi");

	if ((fidOut = fopen (tmp_fname, "w")) == NULL)
	{
		ap_snmpd_id(FLASH_WRITE_FAIL);
		return 0;
	}

	while (!feof (fidIn))
	{
		//fscanf (fidIn, "%[^\n]\n", line);
		memset(line, 0, sizeof(line));
		fgets(line, 1024, fidIn);
		if(strlen(line) > 0)   // yangzhicong 2012-07-07
		line[strlen(line)-1] = '\0';
		if (line[0] == '\0') break;
		if (!feof(fidIn))
		{
			i = 0;

			while ((i < strlen (line)) && (line[i] != type))
			{
				i++;
			};

			memset (tmp_str, 0, sizeof (tmp_str));

			strncpy (tmp_str, line, i);

			for (i = 0; i < count; i++)
			{
				/* printf("[%s]\n",tmp_str); */
				if (strcmp (tmp_str, properties[i].key) == 0)
				{
//edit by xieshaojun 20090206
#if 0
					sprintf (line, "%s%s%c%s%c",
						 properties[i].key, separator,  SEP_Q_MARK, properties[i].value, SEP_Q_MARK);
#else
					memset(tmp_line, 0, sizeof(tmp_line));
					sprintf (tmp_line, "%s%s%c%s%c",
						 properties[i].key, separator,  SEP_Q_MARK, properties[i].value, SEP_Q_MARK);

					if (0 != strcmp(line, tmp_line))
					{
						snprintf(change_msg + strlen(change_msg),
							 sizeof(change_msg) - strlen(change_msg),
							 "%s%s%s,", properties[i].key, separator,  properties[i].value);
						strcpy(line, tmp_line);
					}

#endif
				}
			}

			fprintf (fidOut, "%s\n", line);
		}
	}

	fclose (fidIn);
	fclose (fidOut);
	unlink (filename);
	rename (tmp_fname, filename);
//add by xieshaojun 20090206
#if 0
	char *p = strrchr(change_msg, ',');

	if (p)	(*p) = '\0';

	snprintf(change_msg + strlen(change_msg), (sizeof(change_msg) - strlen(change_msg)), ".");
	struct cfg_change_stuct cfg_change;
	memset(&cfg_change, 0, sizeof(struct cfg_change_stuct));
	cfg_change.msg_id = CONFIG_FILE_CHANGED;
	cfg_change.datalen = strlen(change_msg);
	strcpy(cfg_change.cfg_change_msg, change_msg);
	cgi_send_msgid_to_snmp_syslog((char*)&cfg_change, sizeof(struct cfg_change_stuct));
#endif
	return 1;
}
#endif /* endof  #if (!USE_NEW_VERSION) */

char *get_prop (char *key, char *result, prop_data properties[], int count)
{
	int index;
	char tmp_str[256] = {0};
#if 0	/*xieshaojun，仅调试时使用*/

	if (strlen(key) == 0)
	{
		printf("\nerror%s-%d\n", __FILE__, __LINE__);
		return NULL;
	}

#endif
	//add by xialiang,20100621,拷贝前先清空
#if 0
	memset(result, 0, sizeof(result));
#else
	result[0] = '\0'; //edit by xieshaojun,20100701
#endif

	for (index = 0; index < count; index++)
	{
		if (strcmp (properties[index].key, key) == 0)
		{
			strcpy(tmp_str, properties[index].value);
			strcpy(result, strip(tmp_str, SEP_Q_MARK));
			return result;
		}
	}

#if 0 /*add by xieshaojun,20100426,防止页面显示NULL,NaN*/
	strcpy(result, "0");
#endif
	return NULL;
}

#if (!USE_NEW_VERSION)
int save_global_conf(char type, char* filename, char *command, char *key, const char *value)
{
	FILE* fidIn, *fidOut;
	char line[1024], tmp_line[1024];
	char tmp_fname[255], *index;
	char separator[2];
	int i = 0;
	int changed = 0;

	//printf("save_global_conf!!!!\n");
	if ((fidIn = fopen(filename, "r")) == NULL) return 0;

	strcpy(tmp_fname, filename);
	strcat(tmp_fname, ".cgi");

	if ((fidOut = fopen(tmp_fname, "w")) == NULL)
	{
		ap_snmpd_id(FLASH_WRITE_FAIL);
		return 0;
	}

	strcpy(separator, (type == SEP_EQUAL ? "=" : type == SEP_SPACE ? " " : "\t"));
	type = separator[0];

	while (!feof(fidIn))
	{
		memset(line, 0, sizeof(line));
		fgets(line, 1024, fidIn);
		strcpy(tmp_line, command);
		strcat(tmp_line, key);
		//add by xieshaojun 080703
		sprintf(tmp_line + strlen(tmp_line), "%c", type);

		if (((index = strstr(line, tmp_line)) != NULL) && (index == line))
		{
			i = 1;
//add by xieshaojun 20090206
//网通：日志文件中应记录用户接入信息、配置更改信息。
#if 1
			char old_value[1024];
			strcpy(old_value, index + strlen(tmp_line));
			char * t = strrchr(old_value, SEP_Q_MARK);

			if (t)		*t = '\0';

			t = strchr(old_value, SEP_Q_MARK);

			if (t)		t++;
			else		t = old_value;

			if (0 == strcmp(t, value))
				changed = -1;

#endif

			if (!feof(fidIn))
			{
				if (type != '0')
					fprintf(fidOut, "%s%s%c%c%s%c\n", command, key, type, SEP_Q_MARK, value, SEP_Q_MARK);
				else
					fprintf(fidOut, "%s%s%c%s%c\n", command, key, SEP_Q_MARK, value, SEP_Q_MARK);
			}
		}
		else if (!feof(fidIn)) fprintf(fidOut, "%s", line);
	}

	if (0 == i)
	{
		if (type != '0')
		{
			//modify xieshaojun 2008-05-12
			//fprintf(fidOut,"%s%s%c%s\n",command,key,type,value);
			fprintf(fidOut, "%s%s%c\"%s\"\n", command, key, type, value);
		}
		else
		{
			fprintf(fidOut, "%s%s%s\n", command, key, value);
		}
	}

	fclose(fidIn);
	fclose(fidOut);
	unlink(filename);
	rename(tmp_fname, filename);
//add by xieshaojun 20090206
#if 0

	if (0 == changed)
	{
#if 0                                    /*edit by liwei, 20100406 */
		char *p_f = strstr(filename, "/etc/IWNCOMM/");
#else
		char *p_f = strstr(filename, "/etc/ap11n/");
#endif

		if (p_f == filename)	//临时文件改变时不发syslog
		{
			char change_msg[1024];
			memset(change_msg, 0, sizeof(change_msg));
			char user_name[64];
			get_logined_user(user_name);
#if 0                                    /*edit by liwei, 20100406 */
			snprintf(change_msg + strlen(change_msg), (sizeof(change_msg) - strlen(change_msg)), "[user:%s] %s changes:",
				 user_name, filename + strlen("/etc/IWNCOMM/"));
#else
			snprintf(change_msg + strlen(change_msg), (sizeof(change_msg) - strlen(change_msg)), "[user:%s] %s changes:",
				 user_name, filename + strlen("/etc/ap11n/"));
#endif
			//snprintf(change_msg + strlen(change_msg), (sizeof(change_msg)-strlen(change_msg)), "config change to:");
			snprintf(change_msg + strlen(change_msg), sizeof(change_msg) - strlen(change_msg),
				 "%s%s%s,", key, separator,  value);
			char *p = strrchr(change_msg, ',');

			if (p)	(*p) = '\0';

			snprintf(change_msg + strlen(change_msg), (sizeof(change_msg) - strlen(change_msg)), ".");
			struct cfg_change_stuct cfg_change;
			memset(&cfg_change, 0, sizeof(struct cfg_change_stuct));
			cfg_change.msg_id = CONFIG_FILE_CHANGED;
			cfg_change.datalen = strlen(change_msg);
			strcpy(cfg_change.cfg_change_msg, change_msg);
			cgi_send_msgid_to_snmp_syslog((char*)&cfg_change, sizeof(struct cfg_change_stuct));
		}
	}

#endif
	return 1;
}
#endif  /* #if (!USE_NEW_VERSION) */

void free_prop (prop_data properties[], int count)
{
	int index;
	if (0 == count)
		return;
	for (index = 0; index < count; index++)
	{
		if (properties[index].key)
		{
			free (properties[index].key);
			properties[index].key = NULL;
		}

		if (properties[index].value)
		{
			free (properties[index].value);
			properties[index].value = NULL;
		}
	}
}

