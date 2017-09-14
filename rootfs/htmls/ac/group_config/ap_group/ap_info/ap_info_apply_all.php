<?php
	define("PATH", "../../../../");
	include PATH."db/dbhelper.php";
	include PATH."db/page.php";
	$dbhelper = new DAL();
	$sql="select * from ap_info where ap_group_name='".$_GET["ap_group_name"]."'";
	$ap_info = $dbhelper->getall($sql);
	$num = 0;
	foreach($ap_info as $ap){
		$head = pack("n","4096");
		$sub_id = pack("C","3");
		$value = pack("C",$ap["bg_channel"]);
		$value = $value.pack("C",$ap["a_channel"]);
		$subid_len = pack("C",strlen($value));
		$body = $sub_id.$subid_len.$value;
		$len = pack("n",strlen($body));
		$msg = $head.$len.$body;
		$msg = unpack("H*",$msg);
		$dbhelper->update("update ap_info set config_status=x'".$msg[1]."' where id=".$ap["id"]);
		$num++;
	}
	if(count($ap_info) == $num){
		echo "<script>alert('配置成功!');location='ap_info.php?ap_group_name=".$_GET["ap_group_name"]."';</script>";
	}
?>