<?php
include "../lib/dbhelper.php";
$vip_username = $_GET["vip_username"];
$vip_password = md5($_GET["vip_password"]);

$dbhelper = new DAL();

$exist = $dbhelper->getOne("select id from portal_user where username = '".$vip_username."' and password = '".$vip_password."'");

//session_start();

if($exist == ""){
     Header("Content-type:application/json");
			echo json_encode(array('result'=>1)); 
}else{
    $ip = $_SERVER["REMOTE_ADDR"];
		$dbhelper->update("update portal_user set sign_in_time=datetime('now','localtime'),user_ip='".$ip."' where username = '".$vip_username."'");
   // $_SESSION["user"] = $vip_username;
     Header("Content-type:application/json");
			echo json_encode(array('result'=>0)); 
}

?>