/*
 * CGI-BIN POST method data processing library
 *
 */
#include <stdio.h>
#include <string.h>
#include "postdata.h"
#ifdef MEM_DEBUG
#include "memwatch.h"
#else
#include <stdlib.h>
#endif

/**将一个0xFF格式的字符串表示的十六进制转换为十六进制*/
char x2c (char *what)
{
	register char digit;
	digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
	digit *= 16;
	digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));
	return (digit);
}

/**将字符串URL变量转换为十六进制变量*/
char *unescape_url (char *url)
{
	register int x, y;

	for (x = 0, y = 0; url[y]; ++x, ++y)
	{
		if ((url[x] = url[y]) == '%')
		{
			url[x] = x2c (&url[y + 1]);
			y += 2;
		}
	}

	url[x] = '\0';
	return (url);
}

/**将字符串中所有+号转换为空格*/
char *plustospace (char *str)
{
	register int x;

	for (x = 0; str[x]; x++)
		if (str[x] == '+')
			str[x] = ' ';

	return (str);
}

/**以字符stop将字符串ptr分解成小段*/
char *makeword (char *buf, char *ptr, const char stop, char **newptr)
{
	int i;
	i = 0;

	while ((ptr[i] != stop) && (ptr[i] != '\0'))
	{
		buf[i] = ptr[i];
		++i;
	}

	buf[i] = '\0';
	++i;

	if (ptr[i - 1] == '\0')
		*newptr = NULL;
	else
		*newptr = ptr + i;

	return buf;
}

/*
 for upload file add by IWN @2002-10-12
*/
char *read_queue_for_upload (int *len)
{
	int cl;
	char *queue;
	queue = (char *)getenv ("REQUEST_METHOD");

	if (queue == NULL)
	{
		return NULL;
	};

	/* We won't use GET method */
	if (strcasecmp (queue, "GET") == 0)
	{
		return NULL;
	};

	/* Request method != "GET". Assuming "POST" */
	queue = (char *)getenv ("CONTENT_LENGTH");

	if (queue == NULL)
	{
		return NULL;
	}

	cl = atoi (queue);
	*((int*)len) = cl;
	queue = (char *) malloc (cl + 1);

	if (queue == NULL)
	{
		return NULL;
	}

	if (fread (queue, 1, cl, stdin) != (unsigned int)cl)
	{
		free (queue);
		return NULL;
	}

	queue[cl] = '\0';
	return queue;
};

/**读取Web事件的参数内容*/
char *read_queue (void)
{
	int cl;
	char *queue;
	/**解析事件*/
	queue = (char *)getenv ("REQUEST_METHOD");//get ,post

	if (queue == NULL)
	{
		return NULL;
	};

	/*不允许以GET方式传递数据*/

	/* We won't use GET method */
	if (strcasecmp (queue, "GET") == 0)
	{
		return NULL;
	}

	/**获取post过来的内容长度*/
	/* Request method != "GET". Assuming "POST" */
	queue = (char *)getenv ("CONTENT_LENGTH");

	if (queue == NULL)
	{
		return NULL;
	}

	cl = atoi (queue);
//读取post过来的参数
	queue = (char *) malloc (cl + 1);

	if (queue == NULL)
	{
		return NULL;
	}

	if (fread (queue, 1, cl, stdin) != cl)
	{
		free (queue);
		return NULL;
	}

	queue[cl] = '\0';
	return queue;
};


/**释放缓冲区*/
char *free_buff(char *buff, int buff_len)
{
	if (buff != NULL)
	{
		memset(buff, 0, buff_len);
		free(buff);
	}

	return NULL;
}

/**从Web Server读取并解析post过来的参数对，函数返回参数对个数*/
int process_queue (prop_data result[])
{
	char *indata, *dataptr;
	char *nbuf;	//变量名
	char *vbuf;	//变量值
	int count = 0; //变量数目计数器
	nbuf = (char *) malloc (MAX_NAME);  /* Variable name buffer */

	if (nbuf == NULL) return 0;

	vbuf = (char *) malloc (MAX_VALUE);	/* Variable value buffer */

	if (vbuf == NULL) return 0;

	/**从Web Server获取参数集的内容*/
	indata = read_queue ();	/* Get all indata */

	if (indata == NULL)
	{
		nbuf = free_buff(nbuf, MAX_NAME);
		vbuf = free_buff(vbuf, MAX_VALUE); /* add by iwn @2003-06-23*/
		return 0;
	}

	//解析参数集为参数对
	dataptr = indata;

	while (dataptr != NULL)
	{
		unescape_url (plustospace (makeword (nbuf, dataptr, '=', &dataptr)));
		unescape_url (plustospace (makeword (vbuf, dataptr, '&', &dataptr)));

		if ((result[count].key = (char *)malloc (strlen (nbuf) + 1)) != NULL &&
				(result[count].value = (char *)malloc (strlen (vbuf) + 1)) != NULL)
		{
			sprintf (result[count].key, "%s", nbuf);
			sprintf (result[count++].value, "%s", vbuf);
		}
	};

	free (indata);

	free (nbuf);

	free (vbuf);

	return count;
}


