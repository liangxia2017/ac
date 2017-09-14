<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";

if(isset($_GET["update"])){
    $filePath = "/tmp/";//文件路径
    $fileName = $_FILES["portal_update"];
    if(empty($fileName["name"])){
    	echo "<script>alert('请选择文件！');</script>";
    }else{
        if(move_uploaded_file($fileName["tmp_name"],$filePath.'ac_portal.img'))
        {
    		exec("/ac/script/portal_update",$arr,$in);
            if($in == 128){
                echo "<script>alert('升级失败！');location='ac_basic_config.php?r='+Math.random();</script>";
            }else{
                echo "<script>alert('升级成功!');location='ac_basic_config.php?r='+Math.random();</script>";
            }
        }else{
            echo "<script>alert('升级失败！');location='ac_basic_config.php?r='+Math.random();</script>";
        }              
      }

}else{
$dbhelper = new DAL();
$exsit = $dbhelper->getOne("select count(*) from ac_basic_conf");
$params = array($_POST["ac_portal_sw"],$_POST["redirect_ip"],$_POST["portal_white_list"]);
$cmd = "/ac/script/init_iptables_scr";
/**
if($_POST["ac_portal_sw"] == 0){
    $cmd = "iptables -t nat -F PREROUTING";
}elseif($_POST["ac_portal_sw"] == 1){
    $cmd = "iptables -t nat -F PREROUTING;iptables -t nat -A PREROUTING -p tcp --dport 80 -j DNAT --to ".$_POST["redirect_ip"];
}
*/
if($exsit > 0){
    $sql = "update ac_basic_conf set ac_portal_sw=?,redirect_ip=?,portal_white_list=?";
    $update = $dbhelper->update($sql,$params);
    if($update > 0){
    	exec($cmd,$arr,$inter);
		if($inter != 0){
		  echo "<script>alert('操作失败!');location='ac_basic_config.php?r='+Math.random();</script>";
		}
        echo "<script>alert('操作成功!');location='ac_basic_config.php?r='+Math.random();</script>";
    }
}else{
    $sql = "insert into ac_basic_conf(ac_portal_sw,redirect_ip,portal_white_list) values(?,?,?)";
    $insert = $dbhelper->insert($sql,$params);
    if($insert > 0){
        exec($cmd,$arr,$inter);
		if($inter != 0){
		  echo "<script>alert('操作失败!');location='ac_basic_config.php?r='+Math.random();</script>";
		}
        echo "<script>alert('操作成功!');location='ac_basic_config.php?r='+Math.random();</script>";
    }
}
}
?>