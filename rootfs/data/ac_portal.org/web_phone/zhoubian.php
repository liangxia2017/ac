<?php
$cmd = 'ip neigh |grep '.$_SERVER["REMOTE_ADDR"].'|cut -d " " -f 5';
exec($cmd,$arr,$inter);
$mac = strtolower(str_ireplace(":","",implode($arr)));
var_dump($mac);
?>