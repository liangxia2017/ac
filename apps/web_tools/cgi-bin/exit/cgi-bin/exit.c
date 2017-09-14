#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <webman.h>

#include <string.h>
#include <time.h>


/*
* Main function,
* prints request results into stdout
*/
int main(int arg, char **argv)
{

    	stringlist *my_list=NULL;
	char index_file[LENGTH]={0};
	unsigned char error_page[1024]={0};
      int ret = -1;

    	/* Output MIME header */
    	printf( "Content-type: text/html\n\n" );
    
	sprintf(index_file, "%s/%s", HTMLS_PATH, INDEX_FILE);


	sprintf(error_page, "%s/%s", HTMLS_PATH, ERROR_PAGE);
	ret = http_timeout_update(TIMEOUT_FILE_PATH,  getenv("REMOTE_ADDR"));
        if (ret == -1)
        {
                //�����δ��¼ҳ��
                add_str(&my_list, "{ERROR_INFO}", "��δ��¼�������µ�¼");
                print_html(error_page, &my_list);
                free_list(&my_list);
                return 0;
        }
        else
        {
		if (http_timeout_logout(TIMEOUT_FILE_PATH, getenv("REMOTE_ADDR")) != 0)
		{
			add_str(&my_list, "{ERROR_INFO}", "�˳���¼ʧ��:����Sessionʱ������һ������!");
			print_html(error_page, &my_list);
                	free_list(&my_list);
                	return 0;
		}
		print_html(index_file,&my_list);
		free_list(&my_list);
		return 0;
			
        }

    	/* Load configuration */
    	print_html(index_file,&my_list);

	free_list(&my_list);

    	return 0;
}
