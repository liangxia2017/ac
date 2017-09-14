<?php
session_start();
include 'db/dbhelper.php';
$user_name = $_POST['user_name'];
$user_password = $_POST['user_password'];
if($user_name==null||$user_name==''){
	echo "<script>location='login.php';</script>";
}
if($user_password==null||$user_password==''){
	echo "<script>location='login.php';</script>";
}


$user_password_crypt = crypt($user_password,$user_password);
$params = array($user_name,$user_password_crypt);
$dbhelper = new DAL();
$userlist = $dbhelper->getall("select * from ac_user_info where user_name=? and password=?",$params);
if(count($userlist)==0){
	echo "<script>location='login.php?action=error';</script>";
}else{
    $_SESSION["USER_ID"]=$userlist[0]["id"];
	$_SESSION["USER_NAME"]=$user_name;
    $_SESSION["USER_TYPE"]=$userlist[0]["user_type"];
    $_SESSION["LISENCE"]=$userlist[0]["lisence"];
	//$_SESSION["TEST"]="test";
	//$ip=$_SERVER["REMOTE_ADDR"];
	//$params1=array($userlist[0]['last_login_time'],date('Y-m-d H:i:s'),$userlist[0]['last_login_ip'],$ip,$userlist[0]['id']);
	//$dbhelper->update("update sys_user set previous_login_time=?,last_login_time=?,previous_login_ip=?,last_login_ip=? where id=?",$params1);
	echo "<script>location='main.php';</script>";
}
?>