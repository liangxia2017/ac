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
    $sql="select * from ap_report where ap_mac='".$_GET["ap_mac"]."'";
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
            <td>有线侧接收(MB)</td><td><div id="eth_re"></div></td>
            <td>有线侧发送(MB)</td><td><div id="eth_se"></div></td>
            <td>wifi侧接收(MB)</td><td><div id="wifi_re"></div></td>
        </tr>
        <tr>
            <td>wifi侧发送(MB)</td><td><div id="wifi_se"></div></td>
            <td>lte侧接收(MB)</td><td><div id="lte_re"></div></td>
           <td>lte侧发送(MB)</td><td><div id="lte_se"></div></td>
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