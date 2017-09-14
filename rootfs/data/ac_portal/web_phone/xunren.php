<?php
    session_start();
    $ip = $_SERVER["REMOTE_ADDR"];
    $cmd = "/ac/script/ipset_do 2 ".$ip;
    exec($cmd,$a,$in);
    $cmd = "/ac/script/ipset_do 1 ".$ip." 30";
    exec($cmd,$a,$in);
    echo "<script>location = 'http://eqxiu.com/s/hJNIy6Fx';</script>";
?>