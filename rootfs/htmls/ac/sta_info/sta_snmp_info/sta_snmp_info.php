<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
define("PATH", "../../../");
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
<title>终端信息表</title>
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
	font-size: 22px;
	letter-spacing:4px;
	width: 96%;
	font-weight:bold;
	text-align: center;
    padding: 10px 0;
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
    $sql="select * from sta_snmp_info where sta_mac='".$_GET["sta_mac"]."'";
    $dbhelper = new DAL();
    $stainfo = $dbhelper->getall($sql);
    $stainfo = $stainfo[0];
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
    while($key = key($stainfo)){
        echo "$('#".$key."').text('".$stainfo[$key]."');";
        next($stainfo);
    }
    ?>
	});
</script>
</head>
<body>
<div class="t">终端信息表</div>
<form name="user_list" id="user_list" method="post">
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table class="acinfo_table"  id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
            <td>MAC地址</td><td><div id="sta_mac"></div></td>
            <td>所属AP MAC</td><td><div id="assc_ap_mac"></div></td>
            <td>终端关联时长</td><td><div id="sta_up_time"></div></td>
        </tr>
        <tr>
            <td>IP地址</td><td><div id="StaIPAddress"></div></td>
            <td>AP接收到的信号强度(dB)</td><td><div id="APReceivedStaSignalStrength"></div></td>
            <td>终端接收到AP信号信噪比</td><td><div id="APReceivedStaSNR"></div></td>
        </tr>
        <tr>
            <td>发送到终端的总包数</td><td><div id="StaTxPkts"></div></td>
            <td>发送到终端的总字节数</td><td><div id="StaTxBytes"></div></td>
            <td>终端收到的总包数</td><td><div id="StaRxPkts"></div></td>
        </tr>
        <tr>
            <td>从终端收到的总字节数</td><td><div id="StaRxBytes"></div></td>
            <td>终端模式</td><td><div id="StaRadioMode"></div></td>
            <td>终端所用的无线信道</td><td><div id="StaRadioChannel"></div></td>
        </tr>
        <tr>
            <td>终端当前接入速率</td><td><div id="APTxRates"></div></td>
            <td>终端所在的Vlan ID</td><td><div id="StaVlanId"></div></td>
            <td>SSID名称</td><td><div id="StaSSIDName"></div></td>
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