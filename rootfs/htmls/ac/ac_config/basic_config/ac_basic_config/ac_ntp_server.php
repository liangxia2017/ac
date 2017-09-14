<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";
if($_POST["sel"] == "time"){
    $time = $_POST["time_config"];
    list($year,$hour) = preg_split("/ /",$time);
    // date 月日时分年.秒
    $cmd = "date -s".$year." && date -s ".$hour;
    exec($cmd,$arr,$retval);
//    var_dump($cmd);
//    var_dump($retval);
    if($retval > 0){
        echo "<script>alert('时间设置失败');</script>";    
    }else{
  		echo "<script>alert('时间设置成功');</script>";    
    }
    echo "<script>location='ac_basic_config.php?r='+Math.random();</script>";
}
if($_POST["sel"] == "ip"){
$dbhelper = new DAL();
$exsit = $dbhelper->getOne("select count(*) from ac_basic_conf");
if($exsit > 0){
    $update = $dbhelper->update("update ac_basic_conf set ntp_server_ip='".$_POST["ntp_server_ip"]."'");  
	if($update>0){
		echo "<script>alert('标准时间源IP修改成功');location='ac_basic_config.php?r='+Math.random();</script>";
	}
}else{
    $insert = $dbhelper->insert("insert into ac_basic_conf(ntp_server_ip) values('".$_POST["ntp_server_ip"]."')");  
	if($insert>0){
		echo "<script>alert('操作成功');location='ac_basic_config.php?r='+Math.random();</script>";
	}
}

}
?>