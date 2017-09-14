<?php
include "../web_phone/db/dbhelper.php";
$regist_username = $_GET["regist_username"];
$regist_password = md5($_GET["regist_password"]);

$dbhelper = new DAL();

$exist = $dbhelper->getOne("select count(*) from portal_user where username = '".$regist_username."'");

if($exist == 0){
    $dbhelper->insert("insert into portal_user(username,password) values('".$regist_username."','".$regist_password."')");
    echo "success";
}else{
    echo $exist;
}

?>