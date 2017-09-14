#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <webman.h>
#include <signal.h>
#include <string.h>

extern struct  product_info product_info_ac[];
extern struct html_link html_link_logo[];


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
	

	//int id = 0;
	char barfile[128]={0,};
	
	unsigned char error_page[1024]={0};
        int ret = -1;


	/* Output MIME header */
	printf( "Content-type: text/html\n\n" );
	
        /* Load configuration */
#if 0		
	struct product_info_t product_info;
	prop_count=load_prop(SEP_EQUAL, BASIC_CONF,properties);
	memset(&product_info, 0, sizeof(struct product_info_t));
	get_prop("COMPANY", product_info.company_name, properties, prop_count);
	get_prop("PRODUCT_ID", product_info.product_id, properties, prop_count);
	free_prop(properties,prop_count);
#endif	
	//handle_product_info_pwlan(PATH, &product_info, &my_list);
	//struct product_info_txt_t txt_info;
#if 0
	add_str(&my_list,"{AP_FACT}", "0");
	add_str(&my_list,"{AP_TYPE}", "1"); 
#endif
	//handle_product_info_pwlan(PATH, 0, &txt_info);
	#if 0
	add_str(&my_list,"{AP_TITLE}",txt_info.title);
	add_str(&my_list,"{LINK0_FACT}",txt_info.link0);
	add_str(&my_list,"{LINK_FACT}",txt_info.link);
	add_str(&my_list,"{LINK_TXT_FACT}",txt_info.link_txt);
	#endif
//add by xieshaojun 20090214

#if 0 //edit by xialiang,20100521,此页只需要logo，其余不要
	add_str(&my_list,"{BAR_LOGO}", "logo2_ap.gif");
	add_str(&my_list,"{LINK0_FACT}","www.grentech.com.cn");
#else
	char testsap_id[8];

	prop_count=load_prop(SEP_EQUAL, AC_SYSINFO_CONF,properties);
	get_prop("TESTSAP_ID",testsap_id , properties, prop_count);
	if(strlen(testsap_id) == 0)
		sprintf(testsap_id,"%s","101");
	free_prop(properties,prop_count);
	//100-200范围的id都是国人厂商信息
	short int tmpid;
	tmpid = atoi(testsap_id)/100 - 1;
	if(tmpid < 0)
		tmpid = 0;
	add_str(&my_list,"{BAR_LOGO}",html_link_logo[tmpid].linklogo2);
	add_str(&my_list,"{LINK0_FACT}",html_link_logo[tmpid].linkurl);
	

#endif	
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
	/* Load configuration */
	sprintf(barfile, "%s%s%s%s",HTMLS_PATH, "bar", "_ap",".html");	
	print_html(barfile,&my_list);
	free_list(&my_list);
	return 0;
}
