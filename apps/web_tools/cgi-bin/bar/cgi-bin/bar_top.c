#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <webman.h>
#include <signal.h>
#include <string.h>

#include "config.h"


extern char *product_mode[4];

/*
* Main function,
* prints request results into stdout
*/
int main(int arg, char **argv)
{
	prop_data properties[KEYS_MAX];
	int prop_count=0;
	stringlist *my_list=NULL;
	struct product_info_t product_info;
	
	int sel_num = 0;
	int id = 0;
	char barfile[128]={0,};
	char left_path[LENGTH]={0};
	char select_num[LENGTH]={0};
	unsigned char error_page[1024]={0};
        int ret = -1;


	/* Output MIME header */
	printf( "Content-type: text/html\n\n" );

	prop_count=load_prop(SEP_EQUAL, BASIC_CONF,properties);
	memset(&product_info, 0, sizeof(struct product_info_t));
	get_prop("COMPANY", product_info.company_name, properties, prop_count);
	get_prop("PRODUCT_ID", product_info.product_id, properties, prop_count);
	free_prop(properties,prop_count);
	//handle_product_info(PATH, &product_info, &my_list);
    
        sprintf(error_page, "%s/%s", HTMLS_PATH, ERROR_PAGE);
        ret = http_timeout_update(TIMEOUT_FILE_PATH, getenv("REMOTE_ADDR"));
        if (ret == -1)
        {
                printf( "Content-type: text/html\n\n" );

                //输出尚未登录页面
                add_str(&my_list, "{ERROR_INFO}", "尚未登录，请重新登录");
                print_html(error_page, &my_list);
                free_list(&my_list);
                return 0;
        }
        else if (ret == 0)
        {
                printf( "Content-type: text/html\n\n" );

                //输出连接超时页面
                add_str(&my_list, "{ERROR_INFO}", "连接超时，请重新登录");
                print_html(error_page, &my_list);
                free_list(&my_list);
                return 0;
        }

	id = atoi(product_info.product_id)%4;
	sprintf(barfile, "%s%s",html_path, "bar_ap_top.html");
	print_html(barfile,&my_list);
	free_list(&my_list);
	return 0;
}
