<?php
include "/ac/htmls/db/dbhelper.php";
$dbhelper = new DAL();
$ac_ip = $dbhelper->getOne("select redirect_ip from ac_basic_conf");
//echo "<script>location='http://".$ac_ip."/index.html';</script>";
Header("HTTP/1.1 302 Moved Permanently");
Header("Location:http://".$ac_ip."/index.html");
?>