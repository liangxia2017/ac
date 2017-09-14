<?php
include "/ac/htmls/db/dbhelper.php";
$dbhelper = new DAL();
$ac_ip = $dbhelper->getOne("select gateway from ac_dhcp_server where option43 = '' and option60 = ''");
//echo "<script>location='http://".$ac_ip."/index.html';</script>";
Header("HTTP/1.1 302 Moved Permanently");
Header("Location:http://".$ac_ip."/index.php");
?>