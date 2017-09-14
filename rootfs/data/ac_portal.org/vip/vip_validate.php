<?php
include "../web_phone/db/dbhelper.php";
$vip_username = $_GET["vip_username"];
$vip_password = md5($_GET["vip_password"]);

$dbhelper = new DAL();

$exist = $dbhelper->getOne("select count(*) from portal_user where username = '".$vip_username."' and password = '".$vip_password."'");

session_start();

if($exist == 0){
    echo $exist;
}else{
    $ip = $_SERVER["REMOTE_ADDR"];
    $cmd = "/ac/script/ipset_do 1 ".$ip;
    exec($cmd,$a,$in);
    $_SESSION["user"] = $vip_username;
    echo "index.php";
}

?>