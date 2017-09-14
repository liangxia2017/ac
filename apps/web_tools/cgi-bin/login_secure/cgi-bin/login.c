#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <webman.h>
#include <sys/ioctl.h>

#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#ifdef ENG_VER
#define T_ACCESS_ERR    "User name or Password is incorrect."
#define T_BEEN_LOGINED 	"Only one user in support of login"
#define T_CHECK_USER	"Please check user name!"
#define T_CHECK_PASS	"Please check password!"
#else
#define	T_SYSTEM_ERR	"内部错误!请联系管理员."
#define T_BAD_ACCESS_ERR "非法的WEB请求!"
#define T_ACCESS_ERR    "用户名或口令错误"
#define T_BEEN_LOGINED  "该用户已经登录，不能重复登录"
#define T_CHECK_USER    "请检查登录用户名!"
#define T_CHECK_PASS    "请检查登录口令!"
#endif

extern struct  product_info product_info_ac[];
extern struct html_link html_link_logo[];

//add by liuyaxiong,20081221
#if 0
//将本段代码加入到程序中,可以访问系统底层的某些参数
struct	iw_point_int
{
  caddr_t	pointer;	/* Pointer to the data  (in user space) */
  unsigned short int length;		/* number of fields or size in bytes */
  unsigned short int flags;		/* Optional params */
};
#define IFNAMSIZ_INT (16)
struct	iwreq_int 
{
	union
	{
		char	ifrn_name[IFNAMSIZ_INT];	/* if name, e.g. "eth0" */
	} ifr_ifrn;

	/* Data part */
	union
	{
		char	name[IFNAMSIZ_INT];	/* if name, e.g. "eth0" */
		unsigned int         mode;		/* Operation mode */
		struct iw_point_int data;		/* Other large parameters */
		unsigned int         val[2];		/* val */
	}	u;
};

enum {	//use by ioctl_wfm_char100
	IEEE80211_PARAM_AP_HTTPSLOGIN_SET = 231, // https登录失败纪录
	IEEE80211_PARAM_AP_HTTPSLOGIN_GET = 232, // https登录许可查询
};
#define    IEEE80211_IOCTL_WFM_CHAR100_INT 	(SIOCIWFIRSTPRIV+8)  // Set型cmd,返回值在u->data中
//输入: s 为不小于100的buf，但其内部字符串长度只能小于100
//  IEEE80211_PARAM_AP_HTTPSLOGIN_SET  输入ip字符串, 返回 0/1/2的字符串表示
//  IEEE80211_PARAM_AP_HTTPSLOGIN_GET  输入ip字符串, 返回 0/1/2的字符串表示
//返回: 成功:s的指针   失败:NULL
//  输出: s 中为返回的字符串
char* ioctl_wfm_char520_do(char *ifname,unsigned int opint,char *s)
{
  char buf[100];
  int skfd;
  struct iwreq_int wrq;
  //unsigned int val;
  int ret;

  if(!s)
  return(NULL);
  
  if(strlen(s)>=100)
  return NULL;
  {
  	memcpy(buf,s,100);
	buf[99]=0;
  }
  skfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (skfd < 0)
  {
  	return(NULL);
  }

	{
	  if(strcmp(ifname,"wifi0")==0)
	  {
	  	opint +=10000;
	  }
	  else
	  if(strcmp(ifname,"wifi1")==0)
	  {
	  	opint +=15000;
	  }
	}
  
  strncpy(wrq.ifr_ifrn.ifrn_name, "wfm", IFNAMSIZ_INT);
  wrq.u.data.flags = opint;
  wrq.u.data.pointer = (unsigned char *)buf;
  wrq.u.data.length = 100;

  ret = ioctl(skfd, IEEE80211_IOCTL_WFM_CHAR100_INT, &wrq);
  if( ret < 0)
  {
      //fprintf(stdout,"\ntxdrop socket oprate error\n");
      close(skfd);
      return(NULL);
  }
  close(skfd);

  if(ret==0)
  {
  	memcpy(s,buf,100);
  	return(s);
  }
  else
  return(NULL);
}
#endif

//add by liuyaxiong,20081221
#if 0
int get_httpsloginrequest(char *addressstr,int *tm)
{
  char buf[100];
  char *pt;
  int id;
  strncpy(buf,addressstr,100);
  buf[99]=0;
  id = 0;
  if(strlen(buf)!=0)
  {
	pt = ioctl_wfm_char520_do("wfm",IEEE80211_PARAM_AP_HTTPSLOGIN_GET,buf);
	if(pt)
	{
		int it,ct;
		ct = sscanf(pt,"%d%d",&it,tm);
		if(ct>=1)
		id = it;
	}
  }
  return(id);
}
int get_httpsloginfailrecord(char *addressstr,int *tm)
{
  char buf[100];
  char *pt;
  int id;
  strncpy(buf,addressstr,100);
  buf[99]=0;
  id = 0;
  if(strlen(buf)!=0)
  {
	pt = ioctl_wfm_char520_do("wfm",IEEE80211_PARAM_AP_HTTPSLOGIN_SET,buf);
	if(pt)
	{
		int it,ct;
		ct = sscanf(pt,"%d%d",&it,tm);
		if(ct>=1)
		id = it;
	}
  }
  return(id);
}
#endif

int main(int arg, char **argv)
{
	prop_data properties[KEYS_MAX];
	int prop_count=0;
	char user[1024]={0};
	char passwd[1024]={0};
	
	char get_user[1024]={0};
	char get_passwd[1024]={0}; 
		
	stringlist *my_list=NULL;

	char *java_head = "<script language=\"JavaScript\">\n";
	char *java_tail = "</script>\n";
	char *err_msg_head = "<!--\n alert(\"";
	char *err_msg_tail = "\");\n document.form.USER.focus();\n-->";
	char err_msg_body[128] = {0};
	char err_msg[256] = {0};  
	char login_secure_file[LENGTH]={0};
	char menu_file[LENGTH]={0};
	unsigned char http_timeout[LENGTH]={0};
	unsigned long time_out = 0;
	unsigned char random[LENGTH] = {0};
	unsigned char *p_random=NULL;

	/* Output MIME header */
	printf( "Content-type: text/html\n\n" );

//add by liuyaxiong,20081221

#if 0
	char buf[100];

	int id,tm;
	bzero(buf, sizeof(buf));
	strncpy(buf, getenv("REMOTE_ADDR"), 100);
	buf[99]=0;
	tm = 1;
	//id = get_httpsloginrequest(buf,&tm);
	id = 2;
	if(id==2)
	{
		bzero(buf, sizeof(buf));
		sprintf(buf,"%d",tm);
		add_str(&my_list,"{KEEP_TM}",buf);
		/*edit by liwei , 20100406*/
		sprintf(menu_file, "%s%s", "/opt/ac/www/", "warning_illegal_login_refuse.html");
		printf(menu_file);
		print_html(menu_file, &my_list); 
		free_list(&my_list);
		return 0;
	}}

#endif

	/*edit by lwiei, 20100408*/
	//int ap_fa=0;

	//add_str(&my_list,"{AP_FACT}", "0");
	//add_str(&my_list,"{AP_TYPE}", "1"); 

	/* Load configuration */
	prop_count=load_prop(SEP_EQUAL,USER_CONF,properties);
	get_prop("user", get_user, properties, prop_count);
	get_prop("passwd", get_passwd, properties, prop_count);
	get_prop("http_timeout", http_timeout, properties,prop_count);
	strim(get_user);
	strim(get_passwd);
	strim(http_timeout);
	free_prop(properties,prop_count);
	/* next 5 lines is pwlan addtion!*/
	sprintf(login_secure_file, "%s%s", HTMLS_PATH, "login_secure_0.html");
	
	sprintf(menu_file, "%s%s", HTMLS_PATH, MENU_FILE_0);
	

	time_out = (unsigned long)atol(http_timeout);
	if (time_out == 0)
		time_out = 300;

#if 0
	struct product_info_t product_info;
	prop_count=load_prop(SEP_EQUAL, BASIC_CONF,properties);
	memset(&product_info, 0, sizeof(struct product_info_t));
	get_prop("COMPANY", product_info.company_name, properties, prop_count);
	get_prop("PRODUCT_ID", product_info.product_id, properties, prop_count);
	free_prop(properties,prop_count);
#endif
#if 1//add by xilaiang,20100620,logo都用myprog.h中定义好的
	//handle_product_info_pwlan(PATH, ap_fa, &txt_info);
	add_str(&my_list,"{AP_TITLE}","AC");

	#if 0//add by xialiang
	add_str(&my_list,"{LINK0_FACT}","www.grentech.com.cn");
	add_str(&my_list,"{LINK_FACT}","www.grentech.com.cn");
	add_str(&my_list,"{LINK_TXT_FACT}","深圳国人通信有限公司");

	add_str(&my_list,"{LOGIN_LOGO}", "logo1_ap.gif");
	#else
	char testsap_id[8] = {0};

	prop_count=load_prop(SEP_EQUAL, AC_SYSINFO_CONF,properties);
	get_prop("TESTSAP_ID",testsap_id , properties, prop_count);
	free_prop(properties,prop_count);
	if(strlen(testsap_id) == 0)
		sprintf(testsap_id,"%s","101");
	//100-200范围的id都是国人厂商信息
	short int tmpid = 0;
	tmpid = atoi(testsap_id)/100 - 1;
	if(tmpid < 0)
		tmpid = 0;
	add_str(&my_list,"{LINK0_FACT}",html_link_logo[tmpid].linkurl);
	add_str(&my_list,"{LINK_FACT}",html_link_logo[tmpid].linkurl);
	add_str(&my_list,"{LINK_TXT_FACT}",html_link_logo[tmpid].linktxt);
	add_str(&my_list,"{LOGIN_LOGO}",html_link_logo[tmpid].linklogo1);

	#endif
#endif
#if 0
	add_str(&my_list,"{T_CHECK_USER}", T_CHECK_USER);
	add_str(&my_list,"{T_CHECK_PASS}", T_CHECK_PASS);
#endif

	prop_count=process_queue(properties);

	if (prop_count > 1) 
	{	
		bzero(random, sizeof(random));
		p_random=http_timeout_get_random(TIMEOUT_FILE_PATH, GET_RANDOM);

		get_prop("USER", user, properties, prop_count);
		get_prop("PASSWD", passwd, properties, prop_count);
		get_prop("RANDOM", random, properties, prop_count);
		free_prop(properties,prop_count);
		
		if ((p_random == NULL) || (strcmp(p_random, random) != 0))
		{
			add_str(&my_list,"{USER}","");
			add_str(&my_list,"{PASSWD}","");
		}
		else if ((user[0] != '\0') && (passwd[0] != '\0'))
		{
			if (http_timeout_islogin(TIMEOUT_FILE_PATH, getenv("REMOTE_ADDR")) == 1)
			{
				add_str(&my_list,"{USER}","");
                        	add_str(&my_list,"{PASSWD}","");
				sprintf(err_msg_body,"%s", T_BEEN_LOGINED);
				sprintf(err_msg,"%s%s%s%s%s", java_head, err_msg_head, err_msg_body,err_msg_tail, java_tail);
			} 
			else
                	if(   (strcmp(get_user, user) == 0) && 
                        		(strcmp(get_passwd, passwd) == 0)) 

                	{
                    	add_str(&my_list,"{FIRST_CGI}",LOGIN_SHOW_FIRST_CGI);
                		http_timeout_save(TIMEOUT_FILE_PATH, getenv("REMOTE_ADDR"), time_out);

//add by xieshaojun 20090205
//网通：管理员web访问系统后，应记录到日志。
#if 1
				struct cfg_change_stuct ip_change_data;
				memset(&ip_change_data, 0, sizeof(struct cfg_change_stuct));
				ip_change_data.msg_id = ALARM_ADMIN_LOGINED;
				char user_name[64];
				get_logined_user(user_name);
				sprintf(ip_change_data.cfg_change_msg, "%s logined from %s.", user_name, getenv("REMOTE_ADDR"));
				ip_change_data.datalen = strlen(ip_change_data.cfg_change_msg);
				cgi_send_msgid_to_snmp_syslog((char*)&ip_change_data, sizeof(struct cfg_change_stuct));
#endif

                        	print_html(menu_file,&my_list);
                        	free_list(&my_list);
				
                        	return 0;
                	}
                	else
                	{
//add by liuyaxiong,20081221
#if 1
				{
				  char buf[100];
				  int id,tm;
				  strncpy(buf,getenv("REMOTE_ADDR"),100);
				  buf[99]=0;
				  #if 0//add by xialiang,20110115
				  id = get_httpsloginfailrecord(buf,&tm);
				  //攻击
				  if(id==2)
				  {

	//add by xieshaojun 20090205
	//网通：管理员多次web访问系统失败，应记录到日志。
#if 1
					struct cfg_change_stuct ip_change_data;
					memset(&ip_change_data, 0, sizeof(struct cfg_change_stuct));
					ip_change_data.msg_id = ALARM_LOGIN_FAILED;
					char user_name[64];
					strncpy(user_name,user,63);
					user_name[63]=0;
					sprintf(ip_change_data.cfg_change_msg, "Alarm: login attack from %s", getenv("REMOTE_ADDR"));
					ip_change_data.datalen = strlen(ip_change_data.cfg_change_msg);
					cgi_send_msgid_to_snmp_syslog((char*)&ip_change_data, sizeof(struct cfg_change_stuct));
#endif
					sprintf(menu_file, "%s%s", HTMLS_PATH, "warning_illegal_login.html");
	                        	print_html(menu_file,&my_list);
	                        	free_list(&my_list);
					
	                      	return 0;

				  }
				  #endif
				}
#endif

                        	add_str(&my_list,"{USER}","");
                        	add_str(&my_list,"{PASSWD}","");
                        	sprintf(err_msg_body,"%s", T_ACCESS_ERR);
                        	sprintf(err_msg,"%s%s%s%s%s", java_head, err_msg_head, err_msg_body,err_msg_tail, java_tail);
                	}
		}
	}
	else
	{
		if (prop_count > 0)
			free_prop(properties,prop_count);
	}
        if ((p_random=http_timeout_get_random(TIMEOUT_FILE_PATH, CREATE_RANDOM)) == NULL)
        {
		sprintf(err_msg_body,"%s", T_SYSTEM_ERR);
		sprintf(err_msg,"%s%s%s%s%s", java_head, err_msg_head, err_msg_body,err_msg_tail, java_tail);
        }

	add_str(&my_list,"{ACCESS_INFO}",err_msg);
	add_str(&my_list,"{RANDOM}",p_random);

	print_html(login_secure_file, &my_list);            
	//free_list(&my_list);
	return 0;
}


