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
<script src="<?php echo PATH ?>js/checkbox.js" type="text/javascript"></script>
<link rel="stylesheet" href="<?php echo PATH ?>css/body.css" type="text/css" />
<title>性能信息表</title>
</head>

<style type="text/css">
<!--
.acinfo_table{
	background:#a8c7ce;
	width:100%;
/*	border: 1px solid #ddeeff;*/
}

.tdHeader{
	background-color:#d3eaef;
/*	padding:5px; */
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

td{
    line-height: 30px;
}

td:nth-child(odd){
    background:#D3EAEF;
    width:20%;
}

td:nth-child(even){
    background: #FFFFFF;
    width:12%;
    
}

-->
</style>

<?php
    $sql="select * from ap_snmp_info where SysMacAddress='".$_GET["ap_mac"]."'";
    $dbhelper = new DAL();
    $apinfo = $dbhelper->getall($sql);
    $apinfo = $apinfo[0];
?>
<script type="text/javascript">
	$(document).ready(function(){
	$("#stripe tr").mouseover(function(){
	   $(this).addClass("over");
	});
	$("#stripe tr").mouseout(function(){
	   $(this).removeClass("over");
	});
    <?php
    while($key = key($apinfo)){
        echo "$('#".$key."').text('".$apinfo[$key]."');";
        next($apinfo);
    }
    ?>
	});
</script>
</head>
<body>
<div class="t">AP性能表</div>
<form name="user_list" id="user_list" method="post">
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table class="acinfo_table"  id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
            <td>IP地址</td><td><div id="SysIPAddress"></div></td>
            <td>子网掩码</td><td><div id="SysIPNetMask"></div></td>
            <td>网关</td><td><div id="SysGWAddr"></div></td>
        </tr>
        <tr>
            <td>MAC地址</td><td><div id="SysMacAddress"></div></td>
            <td>运行时间</td><td><div id="SysUpTime"></div></td>
            <td>上线时间</td><td><div id="SysOnlineTime"></div></td>
        </tr>
        <tr>
            <td>设备型号</td><td><div id="SysModel"></div></td>
            <td>制造厂商</td><td><div id="SysManufacture"></div></td>
            <td>软件版本</td><td><div id="SoftwareVersion"></div></td>
        </tr>
        <tr>
            <td>CPU实时利用率</td><td><div id="CPURTUsage"></div></td>
            <td>内存实时利用率</td><td><div id="MemRTUsage"></div></td>
            <td>发送的数据包数</td><td><div id="TxDataPkts"></div></td>
        </tr>
        <tr>
            <td>接收的数据包数</td><td><div id="RxDataPkts"></div></td>
            <td>接收的数据包字节数</td><td><div id="UplinkDataOctets"></div></td>
            <td>发送的数据包字节数</td><td><div id="DwlinkDataOctets"></div></td>
        </tr>
        <tr>
            <td>AP与AC的关联状态</td><td><div id="APACAssociateStatus"></div></td>
            <td>接口当前状态包括</td><td><div id="ifOperStatus"></div></td>
            <td>接口工作时间</td><td><div id="ifLastChange"></div></td>
        </tr>
        <tr>
            <td>与AP关联的终端数</td><td><div id="ApStationAssocSum"></div></td>
            <td>关联总次数</td><td><div id="AssocTimes"></div></td>
            <td>关联失败总次数</td><td><div id="AssocFailTimes"></div></td>
        </tr><tr>
            <td>终端异常断开连接的总次数</td><td><div id="ApStatsDisassociated"></div></td>
            <td>有线端口接收单播包数</td><td><div id="ifInUcastPkts"></div></td>
            <td>有线端口接收的总字节数</td><td><div id="ifInOctets"></div></td>
        </tr><tr>
            <td>有线端口发送单播包数</td><td><div id="ifOutUcastPkts"></div></td>
            <td>有线端口发送的总字节数</td><td><div id="ifOutOctets"></div></td>
            <td></td><td><div id="test"></div></td>
        </tr>
    </table>
    </td>
    </tr>
</table>
</form>
<div style="text-align: left; margin-left: 5%;">
<input class="bt" type="button"	onclick="javascript:window.location.reload(true);" value="刷新" />
</div>
</body>
</html>