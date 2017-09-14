<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";

$dbhelper = new DAL();
$exsit = $dbhelper->getOne("select count(*) from ac_basic_conf");
$params = array($_POST["central_forward_mode"],$_POST["tunnel_out"],$_POST["tunnel_in_ip"]);
if($exsit > 0){
    $sql = "update ac_basic_conf set central_forward_mode=?,tunnel_out=?,tunnel_in_ip=?";
    $update = $dbhelper->update($sql,$params);
    if($update > 0){
    	exec("/ac/script/mode_sw",$arr,$inter);
		if($inter != 0){
			echo "<script>alert('操作失败!');location='ac_basic_config.php?r='+Math.random();</script>";
		}
        echo "<script>alert('操作成功!');location='ac_basic_config.php?r='+Math.random();</script>";
    }
}else{
    $sql = "insert into ac_basic_conf(central_forward_mode,tunnel_out,tunnel_in_ip) values(?,?,?)";
    $insert = $dbhelper->insert($sql,$params);
    if($insert > 0){
    	exec("/ac/script/mode_sw",$arr,$inter);
		if($inter != 0){
			echo "<script>alert('操作失败!');location='ac_basic_config.php?r='+Math.random();</script>";
		}
        echo "<script>alert('操作成功!');location='ac_basic_config.php?r='+Math.random();</script>";
    }
}
?>