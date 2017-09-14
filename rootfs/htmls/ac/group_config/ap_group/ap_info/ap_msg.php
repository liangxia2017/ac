<?php

define("PATH", "../../../../");
include PATH."db/dbhelper.php";

$dbhelper = new DAL();

if($_GET["tag"] == 0){
//重启
$head = pack("n","4096");
$sub_id = pack("C","1");
$value = pack("C","0");
$subid_len = pack("C",strlen($value));
$body = $sub_id.$subid_len.$value;
$len = pack("n",strlen($body));
$msg = $head.$len.$body;
}


if($_GET["tag"] == 1){
//恢复出厂
$head = pack("n","4096");
$sub_id = pack("C","1");
$value = pack("C","1");
$subid_len = pack("C",strlen($value));
$body = $sub_id.$subid_len.$value;
$len = pack("n",strlen($body));
$msg = $head.$len.$body;
}

if($_GET["tag"] == 2){
//升级
$head = pack("n","4096");
$sub_id = pack("C","2");
$value = $value.pack("C","0");//升级对象
$value = $value.pack("C","0");//升级方式
$ip = $dbhelper->getOne("select ip from ac_network_config where network_card = 'eth0'");//版本服务器ip
$value = $value.pack("N",ip2long($ip));
//用户名长度，用户名
$name = "root";
$value = $value.pack("n",strlen($name)).$name;
//密码长度，密码
$psw = "123456";
$value = $value.pack("n",strlen($psw)).$psw;
//版本全路径名及长度
$version = $_GET["version"];
$file = "/".$version;
$value = $value.pack("n",strlen($file)).$file;
$subid_len = pack("C",strlen($value));
$body = $sub_id.$subid_len.$value;
$len = pack("n",strlen($body));
$msg = $head.$len.$body;
}

$group_id="0";
foreach ($_POST["id"] as $v){
	$group_id=$group_id.",".$v;
}
$msg = unpack("H*",$msg);
//print_r($msg);
$s = "update ap_info set config_status = x'".$msg[1]."' where id in (".$group_id.")";

$update = $dbhelper->update($s);
if($update>0){
		echo "<script>alert('操作成功');location='ap_info.php?ap_group_name=".$_POST["group"]."';</script>";
	}
?>