/*
 * Parser library
 *
 */
#include <stdio.h>
#include <string.h>
#include "parser.h"
#ifdef MEM_DEBUG
#include "memwatch.h"
#else
#include <stdlib.h>
#endif

#if 1
#define KKKKSHOW
#define KKKK
#define KKKKSH
#define KKKK3
#define KKKK4
#define KKKK5
#define KKKK6
#define KKKK7
#else
#define KKKKSHOW {if(strstr(keyword->str,"VAPLIST")){int d; printf("\nline=%d\n",__LINE__); scanf("%d",&d);printf("txt=%s\n",keyword->changeto); scanf("%d",&d);}}
#define KKKK {int d; printf("\nline=%d\n",__LINE__); scanf("%d",&d);}
#define KKKKSH {if(strstr(keyword->str,"VAPLIST")){int d; printf("\nline=%d\n",__LINE__); scanf("%d",&d);}}
#define KKKK3  {if(strstr(string,"VAPLIST_WLAN3")){int d; printf("\nline=%d\n",__LINE__); scanf("%d",&d);}}
#define KKKK4  {if(flag){int d; printf("\nline=%d\n",__LINE__); scanf("%d",&d);}}
#define KKKK5  if(flag) printf("elem=%p,str=%p,changeto=%p,elem->local_buf=%p",elem,elem->str,elem->changeto,elem->local_buf);
#define KKKK6  {if(strstr(elem->str,"VAPLIST_WLAN3")) flag = 1; else flag = 0;}
#define KKKK7  int flag;
#endif

int parse_load (char *file_name)
{
	FILE *fid;
	char line[1024];

	if ((fid = fopen (file_name, "r")) == NULL)
		return 0;

	while (!feof (fid))
	{
		fscanf (fid, "%[^\n]\n", line);
		//printf ("%s\n", line);
	}

	fclose (fid);
	return 1;
}
void free_stringlist_element(stringlist * elem)
{
	KKKK7
	KKKK6
	KKKK4
	KKKK5
	KKKK4

	if (elem->changeto != elem->local_buf)
	{
		KKKK4

		if (elem->changeto)
		{
			KKKK4
			free(elem->changeto);
			KKKK4
		}

		KKKK4
		elem->changeto = NULL;
		KKKK4
	}

	KKKK4
	free(elem);
	KKKK4
}

stringlist * malloc_stringlist_element(const char *string, const char * changeto)
{
	size_t ct_size = strlen(changeto);
	//some changeto str is more than 2048,so no do it
#if 0

	//最大只允许2048字节
	if (ct_size >= 2048)
		ct_size = 2048;

#endif
	stringlist *tmp = NULL;
	tmp = (stringlist *) malloc (sizeof (stringlist));

	if (!tmp)
		return NULL;

	//memset(&tmp, 0, sizeof(stringlist));
	if (ct_size < sizeof(tmp->local_buf))
	{
		tmp->changeto = tmp->local_buf;
		strcpy(tmp->changeto, changeto);
	}
	else
	{
		tmp->changeto = (char*) malloc (ct_size + 2);

		if (!tmp->changeto)
		{
			free(tmp);
			return NULL;
		}

		//memset(tmp->changeto, 0, ct_size + 1);
		strncpy(tmp->changeto, changeto, ct_size);
		tmp->changeto[ct_size] = 0;
	}

	strncpy(tmp->str, string, sizeof(tmp->str) - 1);
	tmp->str[127] = 0;
	tmp->next = NULL;
	return tmp;
}

/*edit by xieshaojun 2008-05-20*/
/*void add_str (stringlist **start, char *string, char *changeto)
{
  stringlist *tmp;
  stringlist *start_curr;
  if ((*start) == NULL)
    {
     // printf("* new list *\n");
      (*start) = (stringlist *) malloc (sizeof (stringlist));
      //strcpy ((*start)->str, string);
      strncpy((*start)->str, string, sizeof((*start)->str)-1);
      //strcpy ((*start)->changeto, changeto);
      strncpy((*start)->changeto, changeto, (sizeof((*start)->changeto)-1));
      (*start)->next = NULL;
    }
  else
    {
      //we go through the list to the end
      start_curr = (*start);
      while (start_curr->next != NULL)
	start_curr = start_curr->next;
     //printf("  continue list \n");
      tmp = (stringlist *) malloc (sizeof (stringlist));
	 //memset((tmp), 0, sizeof(stringlist));
      //strcpy (tmp->str, string);
	strncpy(tmp->str, string, sizeof(tmp->str)-1);
      //strcpy (tmp->changeto, changeto);
	strncpy(tmp->changeto, changeto, (sizeof(tmp->changeto)-1));
      tmp->next = NULL;
      start_curr->next = tmp;
    }
  return;
};
*/
void add_str (stringlist **start, char *string, char *changeto)
{
	stringlist *tmp;
	stringlist *start_curr;

	if ((*start) == NULL)
	{
		(*start) = malloc_stringlist_element(string, changeto);

		if (*start)
			(*start)->next = NULL;
	}
	else
	{
		start_curr = (*start);

		while (start_curr->next != NULL)
			start_curr = start_curr->next;

		tmp = malloc_stringlist_element(string, changeto);

		if (tmp)
		{
			tmp->next = NULL;
			start_curr->next = tmp;
		}
	}
}

void free_list(stringlist **start)
{
	stringlist *tmp;
	tmp = NULL;
	tmp = *start;

	while (tmp)
	{
		tmp = tmp->next;
		//edit by xieshaojun 2008-05-26
		//free(*start);
		free_stringlist_element(*start);
		*start = tmp;
	}

	return;
}
void delete_string (stringlist **start, char *string)
{
	int i;
	stringlist *tmp;
	stringlist *rule_prev;
	tmp = NULL;
	tmp = *start;
	rule_prev = tmp;
	i = 0;
	KKKK3

	while (tmp != NULL)
	{
		KKKK3

		if (tmp->str != NULL)
		{
			KKKK3

			if (strcmp (tmp->str, string) == 0)
			{
				KKKK3

				if (i == 0)
				{
					KKKK3
					(*start) = (*start)->next;
					//edit by xieshaojun 2008-05-26
					//free(tmp);
					KKKK3
					free_stringlist_element(tmp);
					KKKK
					return;
				}
				else
				{
					KKKK3
					rule_prev->next = rule_prev->next->next;
					KKKK3
					//edit by xieshaojun 2008-05-26
					//free(tmp);
					free_stringlist_element(tmp);
					KKKK
					return;
				};
			};
		}

		KKKK3
		rule_prev = tmp;
		KKKK3
		tmp = tmp->next;
		KKKK3
		i++;
		KKKK3
	};
};
int print_list(stringlist *klst)
{
	stringlist *tmp;
	tmp = klst;

	while (tmp != NULL)
	{
		printf("tmp->str is %s\n", tmp->str);
		printf("tmp->changeto	is %s\n", tmp->changeto);
		//printf("<br>key len=%d--val len=%d</br>\n",strlen(tmp->str),strlen(tmp->changeto));
		tmp = tmp->next;
	}

	return 0;
}

stringlist *find_keyword (char *s, stringlist *klst)
{
	stringlist *tmp;
	tmp = klst;

	while (tmp != NULL)
	{
		//printf("tmp->str is %s\n", tmp->str);
		if (s == (char *) strstr (s, tmp->str))
		{
			//printf("dddddddd=%s\n",s);
			return tmp;
		}

		tmp = tmp->next;
	}

	return NULL;
}

void change_keyword (char *s, stringlist *klst)
{
	int k, t;
	t = strlen (klst->str);
	k = strlen (klst->changeto);
#if 1 //add by liuyaxiuong,主要是这里的问题
	//将 有t+1个字节被替换，${...}的全长
	//strlen (s)+1 表示包含字符串的0结束符
	memmove (s + k, s + t + 1, (strlen (s) + 1) - (t + 1) );
#else
	memmove (s + k, s + t + 1, strlen (s) + t + 1);
#endif
	memmove (s, klst->changeto, k);
}

void print_html (char *filename, stringlist **start)
{
	stringlist *keyword;
	FILE *fid;
//add by liuyaxiong,20080513
#if 1
	char *line = NULL;
	int linelen = 2048;
#else
	char line[2048];
#endif
	char *s_line;
	KKKK

	if ((fid = fopen (filename, "r")) == NULL)
		return;

//add by liuyaxiong,20080513
#if 1
	line = (char *) malloc (linelen);

	if (!line)
	{
		fclose (fid);
		return;
	}

#endif

	while (fgets (line, 1024, fid))
	{
		//add by liuyaxiong,20080513
#if 1
		line[1024] = 0;
#endif
		s_line = line;

		while ((s_line = (char *) strchr (s_line, '$')) != NULL)
		{
			KKKK
			//printf("in strchr\n");
			keyword = find_keyword ((char *) s_line + 1, (*start));

			if (keyword != NULL)
			{
				KKKKSHOW
				//add by liuyaxiong,20080513
#if 1
				{
					int len1, len2, linelennew;
					char *linenew, *s_linenew;
					KKKK
					len1 = strlen(line);

					if (keyword->changeto)
						len2 = strlen(keyword->changeto);
					else
						len2 = 0;

					KKKK

					if ((len1 + len2 + 2) > linelen)
					{
						KKKK
						linelennew = len1 + len2 + 10;
						linenew = (char *) malloc (linelennew);
						KKKK

						if (!linenew)
						{
							KKKK
							goto printend;
						}

						s_linenew = linenew + (s_line - line);
						KKKK
						strcpy(linenew, line);
						KKKK
						free(line);
						KKKK
						line = linenew;
						s_line = s_linenew;
						linelen = linelennew;
						KKKK
					}
				}
#endif
				KKKKSH
				//printf("in print_html\n");
				change_keyword (s_line, keyword);
				KKKKSH
				//printf("delete_string start\n");
				{
					char str[256];
					strcpy(str, keyword->str);
					delete_string(start, str);
				}
				//printf("delete_string ok\n");
				KKKK
			}

#if 1 //add by liuyaxiuong
			//当进行了keyword替换后，需要再次在原指针上开始关键字查找，
			//否则，指针后移一个字节继续查找
			else
#endif
				s_line++;
		}

		printf ("%s", line);
	}

//add by liuyaxiong,20080513
#if 1
printend:

	if (line)
		free(line);

#endif
	fclose (fid);
	//print_list(*start);
}


