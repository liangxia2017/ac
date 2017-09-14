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

/**��һ��0xFF��ʽ���ַ�����ʾ��ʮ������ת��Ϊʮ������*/
char x2c (char *what)
{
	register char digit;
	digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
	digit *= 16;
	digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));
	return (digit);
}

/**���ַ���URL����ת��Ϊʮ�����Ʊ���*/
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

/**���ַ���������+��ת��Ϊ�ո�*/
char *plustospace (char *str)
{
	register int x;

	for (x = 0; str[x]; x++)
		if (str[x] == '+')
			str[x] = ' ';

	return (str);
}

/**���ַ�stop���ַ���ptr�ֽ��С��*/
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

/**��ȡWeb�¼��Ĳ�������*/
char *read_queue (void)
{
	int cl;
	char *queue;
	/**�����¼�*/
	queue = (char *)getenv ("REQUEST_METHOD");//get ,post

	if (queue == NULL)
	{
		return NULL;
	};

	/*��������GET��ʽ��������*/

	/* We won't use GET method */
	if (strcasecmp (queue, "GET") == 0)
	{
		return NULL;
	}

	/**��ȡpost���������ݳ���*/
	/* Request method != "GET". Assuming "POST" */
	queue = (char *)getenv ("CONTENT_LENGTH");

	if (queue == NULL)
	{
		return NULL;
	}

	cl = atoi (queue);
//��ȡpost�����Ĳ���
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


/**�ͷŻ�����*/
char *free_buff(char *buff, int buff_len)
{
	if (buff != NULL)
	{
		memset(buff, 0, buff_len);
		free(buff);
	}

	return NULL;
}

/**��Web Server��ȡ������post�����Ĳ����ԣ��������ز����Ը���*/
int process_queue (prop_data result[])
{
	char *indata, *dataptr;
	char *nbuf;	//������
	char *vbuf;	//����ֵ
	int count = 0; //������Ŀ������
	nbuf = (char *) malloc (MAX_NAME);  /* Variable name buffer */

	if (nbuf == NULL) return 0;

	vbuf = (char *) malloc (MAX_VALUE);	/* Variable value buffer */

	if (vbuf == NULL) return 0;

	/**��Web Server��ȡ������������*/
	indata = read_queue ();	/* Get all indata */

	if (indata == NULL)
	{
		nbuf = free_buff(nbuf, MAX_NAME);
		vbuf = free_buff(vbuf, MAX_VALUE); /* add by iwn @2003-06-23*/
		return 0;
	}

	//����������Ϊ������
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


