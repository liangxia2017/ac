<?php
include "../lib/dbhelper.php";
$regist_username = $_GET["regist_username"];
$regist_password = md5($_GET["regist_password"]);

$dbhelper = new DAL();

$exist = $dbhelper->getOne("select id from portal_user where username = '".$regist_username."'");

if($exist == ""){
		$host = $_SERVER["REMOTE_HOST"];
    $dbhelper->insert("insert into portal_user(username,password,register_time,user_host) values('".$regist_username."','".$regist_password."',datetime('now','localtime'),'".$host."')");
     Header("Content-type:application/json");
			echo json_encode(array('result'=>0)); 
}else{
     Header("Content-type:application/json");
			echo json_encode(array('result'=>1)); 
}

?>