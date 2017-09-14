<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";
?>
<head>
<META   HTTP-EQUIV="pragma"   CONTENT="no-cache">         
<META   HTTP-EQUIV="Cache-Control"   CONTENT="no-cache,   must-revalidate">         
<META   HTTP-EQUIV="expires"   CONTENT="0"> 
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script type="text/javascript" src="<?php echo PATH ?>js/jquery.js"></script>
<script src="<?php echo PATH ?>js/jquery.validate.js" type="text/javascript"></script>
<link rel="stylesheet" href="<?php echo PATH ?>css/body.css" type="text/css" />
<title>AP信息</title>
</head>

<style type="text/css">
<!--
.acinfo_table{
/*	margin: auto; */
	background:#a8c7ce;
	width:60%;
	border: 1px solid #ddeeff;
}

.tdContent{
	background-color:#ffffff;
	padding:5px;
	text-align:center;
}

.tdContentF9{
	background-color:#f9f9f9;
	padding:5px;
	text-align:center;
}

tr.over td{
    background-color:#d5f4fe;
}

.t {
	font-size: 18px;
	letter-spacing:4px;
	width: 96%;
	font-weight:bold;
	text-align: center;
    padding-top: 0;
	color:#0099CC;
}
-->
</style>
<?php
$dbhelper = new DAL();
$getall = $dbhelper->getall("select * from ap_info where ap_mac ='".$_GET["ap_mac"]."'");
$ap_info = $getall[0];
?>

<script type="text/javascript"> 
	$(document).ready(function(){
		$("#ap_group_name").text("<?php echo $ap_info['ap_group_name'];?>");
        <?php 
         $ap_mac = $ap_info["ap_mac"];
         $mac = "";
         for($k = 0; $k<strlen($ap_mac); $k++){
            if($k != 0 && $k%2 == 0)
                $mac = $mac.":".$ap_mac[$k];
            else
                $mac = $mac.$ap_mac[$k];
            }
        ?>
        $("#ap_mac").text("<?php echo $mac;?>");
		$("#ap_ip").text("<?php echo $ap_info['ap_ip'];?>");
		$("#ap_remark").text("<?php echo $ap_info['ap_remark'];?>");
        <?php
            $status = "";
            if($ap_info["status"] == 0)
                $status="未加入";
            if($ap_info["status"] == 1)
                $status="up";
            if($ap_info["status"] == 2)
                $status="idle";
        ?>
		$("#status").text("<?php echo $status;?>");
        $("#last_join_time").text("<?php echo $ap_info['last_join_time'];?>");
        $("#bg_channel").text("<?php echo $ap_info['bg_channel'];?>");
        $("#a_channel").text("<?php echo $ap_info['a_channel'];?>");
        <?php
            $config = "配置完成";
            if($ap_info["config_mask"] > 0 | $ap_info["config_status"] != null | $ap_info["config_status"] != "")
                $config="配置中...";
        ?>
        $("#config").text("<?php echo $config;?>");
        $("#ap_locate_area").text("<?php echo $ap_info['ap_locate_area'];?>");
        $("#refer_rssi").text("<?php echo $ap_info['refer_rssi'];?>");
        $("#ap_soft_ver").text("<?php echo $ap_info['soft_ver'];?>");
        $("#sta_num").text("<?php echo $ap_info['sta_num'];?>");

	});
</script>

<script type="text/javascript">
	$(document).ready(function(){
	$("#ap_info tr").mouseover(function(){
	$(this).addClass("over");
	});
	$("#ap_info tr").mouseout(function(){
	$(this).removeClass("over");
	});
	});
</script>


</head>

<body>
<div class="t">AP基本信息</div>
<form>
	<table align="center" id="ap_info" class="acinfo_table" cellpadding="0" cellspacing="1px">
      <tr>
        <td class="tdContent" width="40%">所属AP组：</td>
        <td class="tdContent"><div id="ap_group_name"></div></td>
      </tr>
	  <tr>
        <td class="tdContentF9">MAC地址：</td>
        <td class="tdContentF9"><div id="ap_mac"></div></td>
      </tr>
	  <tr>
        <td class="tdContent">IP地址：</td>
        <td class="tdContent"><div id="ap_ip"></div></td>
      </tr>
	  <tr>
        <td class="tdContentF9">备注信息：</td>
        <td class="tdContentF9"><div id="ap_remark"></div></td>
      </tr>
	  <tr>
        <td class="tdContent">状态：</td>
        <td class="tdContent"><div id="status"></div></td>
      </tr>
      <tr>
        <td  class="tdContentF9">加入时间：</td>
        <td class="tdContentF9"><div id="last_join_time"></div></td>
      </tr>
      <tr>
        <td class="tdContent">bg卡信道：</td>
        <td class="tdContent"><div id="bg_channel"></div></td>
      </tr>
      <tr>
        <td  class="tdContentF9">a卡信道：</td>
        <td class="tdContentF9"><div id="a_channel"></div></td>
      </tr>
      <tr>
        <td  class="tdContent">配置状态：</td>
        <td class="tdContent"><div id="config"></div></td>
      </tr>
      <tr>
        <td  class="tdContentF9">定位区域：</td>
        <td class="tdContentF9"><div id="ap_locate_area"></div></td>
      </tr>
      <tr>
        <td  class="tdContent">定位参考值：</td>
        <td class="tdContent"><div id="refer_rssi"></div></td>
      </tr>
      <tr>
        <td  class="tdContentF9">软件版本号：</td>
        <td class="tdContentF9"><div id="ap_soft_ver"></div></td>
      </tr>
      <tr>
        <td  class="tdContent">终端数：</td>
        <td class="tdContent"><div id="sta_num"></div></td>
      </tr>
</table>
</form>
<table style="width: 50%; padding-right: 30px;">
<tr>
<td><input class="bt" type="button"	onclick="javascript:window.location.reload(true);" value="刷新" /></td>
</tr>
</table>
</body>
</html>