<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";

$dbhelper = new DAL();
$exsit = $dbhelper->getOne("select count(*) from ac_basic_conf");
$params = array($_POST["time_reboot_sw"],$_POST["timer"]);
$cmd = "/ac/script/init_crond_scr";

if($exsit > 0){
    $sql = "update ac_basic_conf set time_reboot_sw=?,timer=?";
    $update = $dbhelper->update($sql,$params);
    if($update > 0){
    	exec($cmd,$arr,$inter);
		if($inter != 0){
		  echo "<script>alert('操作失败!');location='ac_basic_config.php?r='+Math.random();</script>";
		}
        echo "<script>alert('操作成功!');location='ac_basic_config.php?r='+Math.random();</script>";
    }
}else{
    $sql = "insert into ac_basic_conf(time_reboot_sw,timer) values(?,?)";
    $insert = $dbhelper->insert($sql,$params);
    if($insert > 0){
        exec($cmd,$arr,$inter);
		if($inter != 0){
		  echo "<script>alert('操作失败!');location='ac_basic_config.php?r='+Math.random();</script>";
		}
        echo "<script>alert('操作成功!');location='ac_basic_config.php?r='+Math.random();</script>";
    }
}
?>