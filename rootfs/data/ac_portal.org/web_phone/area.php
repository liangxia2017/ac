<?php
    include "../../../htmls/db/dbhelper.php";
    define("FLAG","flag");
    $cmd = 'ip neigh |grep '.$_SERVER["REMOTE_ADDR"].' -w |cut -d " " -f 5';
    exec($cmd,$arr,$inter);
    $mac = strtolower(str_ireplace(":","",implode($arr)));
    $dbhelper1 = new DAL();
    $area_name = $dbhelper1->getOne("select area_name from file_data where sta_mac='".$mac."'");
    echo "<script>location='show_sta.php?sta_mac=".$mac."&area_name=".$area_name."';</script>";
?>