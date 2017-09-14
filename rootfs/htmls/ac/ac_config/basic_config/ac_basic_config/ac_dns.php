<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";

$dbhelper = new DAL();
$exsit = $dbhelper->getOne("select count(*) from ac_basic_conf");
$params = array($_POST["dns1"],$_POST["dns2"]);
$cmd = "/ac/script/set_dns";

if($exsit > 0){
    $sql = "update ac_basic_conf set dns1=?,dns2=?";
    $update = $dbhelper->update($sql,$params);
    if($update > 0){
    	exec($cmd,$arr,$inter);
		if($inter != 0){
		  echo "<script>alert('操作失败!');location='ac_basic_config.php?r='+Math.random();</script>";
		}
        echo "<script>alert('操作成功!');location='ac_basic_config.php?r='+Math.random();</script>";
    }
}else{
    $sql = "insert into ac_basic_conf(dns1,dns2) values(?,?)";
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