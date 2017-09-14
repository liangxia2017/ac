
 #include "locating.h"

   int __init_calc_ap_num(void *arg, int col_num, char **col_value, char **col_name) 
   {
   	*(int *)arg=atoi(col_value[0]);
	return 0;
   }
  int __insert_ap_cb(void *arg, int col_num, char **col_value, char **col_name) 
  {
  	 int  ret,length,width = 0;
	char sql_string[1024] = {0};
	char ap1[256] = {0};
	char ap2[256] = {0};
	char ap3[256] = {0};
	sprintf(sql_string,"alter table local.%s add column '%s_%s_%s_%s' integer default 0",arg,col_value[0],col_value[1],col_value[2],col_value[3]);
	sqlite3_exec(pFile,sql_string,0,0,0);
	return 0;
  }

 int _init_area_tbs_cb(void *arg, int col_num, char **col_value, char **col_name)  
 {
 	int  ret,length,width,multiple = 0;
	sqlite3_stmt  *stmt = NULL;
	char *sql_str = NULL;
	char sql_string[1024] = {0};
	char sum_rssi_str[100] = {0};
	sprintf(sql_string, "select count(*) from ac_db.ap_info where ac_db.ap_info.ap_locate_area= '%s'", col_value[0]); 
	sqlite3_exec(pFile,sql_string,__init_calc_ap_num,&ret,0);
	if(ret != 3)
		return 0;/*该定位区域的ap数不足三个，不建立该区域表*/
	#if 1/*计算坐标与实际面积比例关系*/
	length = atoi(col_value[1]);
	width = atoi(col_value[2]);
	multiple = (1024/length > 768/width) ? 768/width : 1024/length ;
	#endif
	sprintf(sql_string, "create table local.%s (id integer primary key, sta_mac text unique,'%d' integer)", col_value[0],multiple);/*multiple值直接做为列名，该列不存放值*/

	sqlite3_exec(pFile,sql_string,0,0,0);	
	sprintf(sql_string, "select ap_mac, ap_x, ap_y,refer_rssi from ac_db.ap_info where ac_db.ap_info.ap_locate_area='%s'", col_value[0]); 

	sqlite3_exec(pFile,sql_string,__insert_ap_cb,col_value[0],0);
	return 0;  
	
 }

 int init_db()
 {
 	int ret ;
	char *tmp_str = "/tmp/sta_coordinate.s3db";/*该db存放计算后的终端坐标*/
	char *sql_str = NULL;
	system("rm -rf /tmp/sta_coordinate.s3db /tmp/debug_local.s3db");
	system("touch /tmp/sta_coordinate.s3db /tmp/debug_local.s3db");
	system("cp -f  /ac/db/ac.s3db /tmp/");
	
	ret = sqlite3_open(tmp_str, &pFile); 
	if(ret != SQLITE_OK)
	{
			sqlite3_close(pFile);
			return -1;
	}
	//sqlite3_exec(pFile,"drop table if exists file_data",0,0,0);/*删除坐标表中的老数据*/
	sql_str = "create table if not exists file_data (id integer primary key, sta_mac text unique, area_name text, sta_x integer, sta_y integer,sum_rssi integer) ";
	sqlite3_exec(pFile,sql_str,0,0,NULL);
	if(debug==0)
		sqlite3_exec(pFile,"attach database ':memory:' as local",NULL,NULL,NULL); /*内存数据库，用于存放各定位区域的上报数据*/
	else
		sqlite3_exec(pFile,"attach database '/tmp/debug_local.s3db' AS local",NULL,NULL,NULL); 
	if(mode == 1)
		sqlite3_exec(pFile,"attach database '/tmp/ac.s3db' AS ac_db",NULL,NULL,NULL);
	else
		sqlite3_exec(pFile,"attach database '/ac/db/ac.s3db' AS ac_db",NULL,NULL,NULL);
	/*创建区域表*/
	sqlite3_exec(pFile,"select area_name, map_x, map_y from ac_db.ap_locate_edit",_init_area_tbs_cb,0,0);
	if(mode == 0)
		sqlite3_exec(pFile,"update ac_db.ap_info set refer_rssi=45 where id in (select id from ac_db.ap_info)",0,0,0);
	return 0;  
 }
int __update_rssi_cb(void *arg, int col_num, char **col_value, char **col_name)  
{
	char **para = arg;
	char sql_str[1024] = {0};
	int i,sum;
	for(i=0;i<6;i++)/*第456列名与ap的mac有关*/
	{
		if(strstr(col_name[i],para[0]) != NULL)
		break;	
	}
	sprintf(sql_str, "update  local.%s set '%s' = %s where sta_mac = '%s' ", para[3], col_name[i], para[1], para[2]);
	sqlite3_exec(pFile,sql_str,0,0,0);

	return 0;
}
int _insert_info2area_tb(void *arg, int col_num, char **col_value, char **col_name)  
{
	if(col_value[0]==NULL)
		return 0;

	unsigned char **para = arg;
	
	char sql_str[1024] = {0};

	sprintf(sql_str, "insert or ignore into local.%s (sta_mac) values ('%s')", col_value[0], para[2]);
	sqlite3_exec(pFile,sql_str,NULL,NULL,&errmsg);

	sprintf(sql_str, "select * from local.%s where sta_mac='%s'", col_value[0], para[2]);
	para[3] = col_value[0];
	sqlite3_exec(pFile,sql_str,__update_rssi_cb,para,0);
	return 0;
}

int rssi2db(unsigned char *apmac, short rssi, unsigned char *stamac)
{
	unsigned char ap[16] = {0};
	unsigned char sta[16] = {0};
	unsigned char tmp_rssi[8] = {0};
	unsigned char *para[3];
	int ret;

	char sql_str[512] = {0};
	sprintf(ap,MACSTR2,MAC2STR(apmac));
	sprintf(sta,MACSTR2,MAC2STR(stamac));
	sprintf(tmp_rssi,"%d",rssi);
	para[0] = ap;
	//para[1] = (char *)&rssi;
	para[1] = tmp_rssi;
	para[2] = sta;

	if(mode == 0)
	{
		sprintf(sql_str, "select count(*) from ac_db.ap_info where ac_db.ap_info.ap_mac='%s' and ac_db.ap_info.refer_rssi>=%d", ap,rssi);
		sqlite3_exec(pFile,sql_str,__init_calc_ap_num,&ret,0);
		if(ret == 0)/*表中ap不存在或sum值偏小时才插入*/
		{
			sprintf(sql_str, "update ac_db.ap_info set refer_rssi=%d where ap_mac='%s'",rssi,ap);
			sqlite3_exec(pFile,sql_str,0,0,0);
		}
	}else
	{
		sprintf(sql_str, "select ap_locate_area from ac_db.ap_info where ac_db.ap_info.ap_mac = '%s' ", ap);
		sqlite3_exec(pFile,sql_str,_insert_info2area_tb,para,0);
	}
	return 0;
	
}

int __insert_sta2filedata(void *arg, int col_num, char **col_value, char **col_name)  
{
	int ret;
	char sql_str[1024] = {0};
	int tmp_sum_rssi = atoi(col_value[3]) + atoi(col_value[4]) + atoi(col_value[5]);
	sprintf(sql_str, "select count(*) from file_data where sta_mac='%s' and sum_rssi>=%d", col_value[1],tmp_sum_rssi);
	sqlite3_exec(pFile,sql_str,__init_calc_ap_num,&ret,0);
	if(ret == 0)/*表中用户不存在或sum值偏小时才插入*/
	{
		sprintf(sql_str, " insert or replace into file_data \
			(sta_mac,area_name,sum_rssi) values ('%s','%s',%d)",\
			col_value[1],arg,tmp_sum_rssi);
		sqlite3_exec(pFile,sql_str,0,0,0);
	}
	return 0;	
}

int _calc_area_cb(void *arg, int col_num, char **col_value, char **col_name)  
{
	char sql_str[512] = {0};
	sprintf(sql_str, "select * from local.%s", col_value[0]);
	sqlite3_exec(pFile,sql_str,__insert_sta2filedata,col_value[0],0);
	return 0;	
}

int __calc_staxy_cb(void *arg, int col_num, char **col_value, char **col_name)  
{
	char tmp[64]={0};
	char sql_str[512] = {0};
	int multipe = atoi(col_name[2]);
	int rssi[3],x[3],y[3],sta_x_y[2],refer_rssi[3];
	rssi[0] = atoi(col_value[3]);
	rssi[1] = atoi(col_value[4]);
	rssi[2] = atoi(col_value[5]);
	
	strcpy(tmp,col_name[3]);
	strtok(tmp,"_");
	x[0] = atoi(strtok(NULL,"_"));
	y[0] = atoi(strtok(NULL,"_"));
	refer_rssi[0] = atoi(strtok(NULL,"_"));

	strcpy(tmp,col_name[4]);
	strtok(tmp,"_");
	x[1] = atoi(strtok(NULL,"_"));
	y[1] = atoi(strtok(NULL,"_"));
	refer_rssi[1] = atoi(strtok(NULL,"_"));

	strcpy(tmp,col_name[5]);
	strtok(tmp,"_");
	x[2] = atoi(strtok(NULL,"_"));
	y[2] = atoi(strtok(NULL,"_"));
	refer_rssi[2] = atoi(strtok(NULL,"_"));
	if(debug==1 && strcmp(trace,col_value[1]) == 0)
	{	
		int i;
		for(i=0;i<3;i++)
			print_info("------rssi%d=%d\n",i,rssi[i]);
	}
	if(calc_sta_coordinate(rssi,x,y,sta_x_y,multipe,refer_rssi) == -1)
		return 0;
	
	sprintf(sql_str, "update file_data set sta_x=%d, sta_y=%d where sta_mac='%s'", sta_x_y[0],sta_x_y[1],col_value[1]);
	if(debug==1 && strcmp(trace,col_value[1]) == 0)
		print_info("%s\n",sql_str);
	sqlite3_exec(pFile,sql_str,0,0,0);

		
	return 0;
}

int _calc_xy_cb(void *arg, int col_num, char **col_value, char **col_name)  
{
	char sql_str[1024] = {0};
	sprintf(sql_str, "select * from local.%s where sta_mac='%s'", col_value[1],col_value[0]);
	sqlite3_exec(pFile,sql_str,__calc_staxy_cb,0,0);
}

void calc_sta_cdn()
{
	if(mode != 0)
	{
		/*锁定终端的区域*/
		
		sqlite3_exec(pFile,"delete from file_data",0,0,0);
		sqlite3_exec(pFile,"select area_name from ac_db.ap_locate_edit",_calc_area_cb,0,0);

		sqlite3_exec(pFile,"select sta_mac,area_name from file_data",_calc_xy_cb,0,0);
	}
	eloop_register_timeout(period, 0, calc_sta_cdn, NULL, NULL);
}
int _del_area_cb(void *arg, int col_num, char **col_value, char **col_name)  
{
	char sql_str[512] = {0};
	sprintf(sql_str, "delete from local.%s", col_value[0]);
	sqlite3_exec(pFile,sql_str,0,0,0);
	return 0;	
}
/*定时清空数据为了防止数据产生垃圾数据*/
void delete_db()
{
	if(mode != 0)
	{
		sqlite3_exec(pFile,"select area_name from ac_db.ap_locate_edit",_del_area_cb,0,0);
	}
	eloop_register_timeout(period*100, 0, delete_db, NULL, NULL);
}


