<?php
include "/ac/htmls/db/dbhelper.php";
$dbhelper = new DAL();
$cmd = 'ip neigh |grep '.$_SERVER["REMOTE_ADDR"].' -w |cut -d " " -f 5 | tr -d ":"';
exec($cmd,$arr,$inter);
$mac = implode($arr);
$ac_ip = $dbhelper->getOne("select redirect_ip from ac_basic_conf");
//echo "<script>location='http://".$ac_ip."/index.html';</script>";
Header("HTTP/1.1 302 Moved Permanently");
Header("Location:http://".$ac_ip."/index.php?sta_ip=".$_SERVER["REMOTE_ADDR"]."&mac=".$mac);
?>