<?php
class Page{
	//当前页
	private static $pageNow=1;
	//页面大小
	private static $pageSize=20;
	//总记录数
	private static $count=0;
	//总页数
	private static $pageCount=1;
	
	//查询语句
	private static $sql;
	
	//计算总记录是sql
	private static $sqlCount;
	
	//sql参数 类型为array
	private static $param;
	
	private static $result;
	public function Page($sql,$pageNow,$sqlCount=null,$param=null){
		//self::$dbhelper = $dbhelper;
		self::$sql = $sql;
		self::$pageNow=$pageNow==''?1:(int)$pageNow;
		self::$sqlCount = $sqlCount==null?"select count(*) from (".$sql.")":$sqlCount;
		self::$param = $param;
		self::$count = $count;
		
	}

	public function setPageNow($pageNow){
		self::$pageNow=$pageNow;
	}
	public function getPageNow(){
		return self::$pageNow;
	} 
	public function setPageSize($pageSize){
		self::$pageSize=$pageSize;
	}
	public function getPageSize(){
		return self::$pageSize;
	} 
	public function setCount($count){
		self::$count=$count;
	}
	public function getCount(){
		return self::$count;
	} 
	public function setPageCount($pageCount){
		self::$pageCount=$pageCount;
	}
	public function getPageCount(){
		return self::$pageCount;
	}
	public function getResult(){
		$dbhelper = new DAL();
		$query = $dbhelper->pagination(self::$sqlCount, self::$sql, self::$param,self::$pageNow, self::$pageSize, self::$count);
		self::$pageCount=(int)((self::$count-1)/self::$pageSize)+1;
		if(self::$pageNow>self::$pageCount){
			self::$pageNow=self::$pageCount;
		}
		return $query->getAllRows();
	}
}

?>