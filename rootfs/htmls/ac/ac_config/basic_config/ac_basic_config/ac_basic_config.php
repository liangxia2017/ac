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
<title>AC基本配置</title>
</head>

<style type="text/css">
<!--
.acinfo_table{
/*	margin: auto; */
	background:#a8c7ce;
	width:50%;
	border: 1px solid #ddeeff;
}

.tdContent{
	background-color:#ffffff;
	padding:5px;
/*	text-align:center;*/
}

.tdContentF9{
	background-color:#f9f9f9;
	padding:5px;
/*	text-align:center;*/
}

tr.over td{
    background-color:#d5f4fe;
}
-->
</style>
<?php
$dbhelper = new DAL();
$getall = $dbhelper->getall("select * from ac_basic_conf");
$ac_ntp_server = $getall[0];
?>

<script type="text/javascript"> 
	$(document).ready(function(){
        $("#id").val("<?php echo $ac_ntp_server['id'];?>");
        $("#ntp_server_ip").val("<?php echo $ac_ntp_server['ntp_server_ip'];?>");
        $("#central_forward_mode").val("<?php echo $ac_ntp_server['central_forward_mode'];?>");
        $("#tunnel_out").val("<?php echo $ac_ntp_server['tunnel_out'];?>");
        $("#tunnel_in_ip").val("<?php echo $ac_ntp_server['tunnel_in_ip'];?>");
        $("#ac_portal_sw").val("<?php echo $ac_ntp_server['ac_portal_sw'];?>");
        $("#redirect_ip").val("<?php echo $ac_ntp_server['redirect_ip'];?>");
        $("#portal_white_list").val("<?php echo $ac_ntp_server['portal_white_list'];?>");
        $("#dns1").val("<?php echo $ac_ntp_server['dns1'];?>");
        $("#dns2").val("<?php echo $ac_ntp_server['dns2'];?>");
        $("#time_reboot_sw").val("<?php echo $ac_ntp_server['time_reboot_sw'];?>");
        $("#timer").val("<?php echo $ac_ntp_server['timer'];?>");
	});
 	</script>

<script type="text/javascript">
	$(document).ready(function(){
	$("#ac_ntp_server tr").mouseover(function(){
	$(this).addClass("over");
	});
	$("#ac_ntp_server tr").mouseout(function(){
	$(this).removeClass("over");
	});   
	});
       
</script>


</head>

<body>
<!--<div class="title">NTP服务器</div>-->
<br /><br />
<form action="ac_dns.php" name="ac_dns" id="ac_dns" method="post">
<table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="ac_ntp_server">
    <th class="tdContent" colspan="2"><div class="th">DNS服务器</div></th>      
    <tr>
        <td class="tdContentF9">首选DNS</td>
        <td class="tdContentF9" align="left"><input type="text" name="dns1" id="dns1" /></td>
      </tr>
      <tr>
        <td class="tdContent">备用DNS</td>
        <td class="tdContent" align="left"><input type="text" name="dns2" id="dns2" /></td>
      </tr>
</table>
<input class="bt" type="submit" value="确定"/>
</form>
<br /><hr /><br />
<form action="ac_ntp_server.php" name="ac_ntp_server" method="post">
<table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="ac_ntp_server">
    <th class="tdContent" colspan="2"><div class="th">NTP配置</div></th>      
	  <tr>
        <td class="tdContentF9">
        <select id="sel" name="sel" onchange="change(this)">
        		<option value="time">系统时间配置</option>
            <option value="ip">标准时钟源IP</option>
            
        </select>
        </td>
        <td class="tdContentF9" align="left"><label>
          <input type="text" name="ntp_server_ip" style=" display: none;" id="ntp_server_ip"/>
          <input type="text" style="background: lightgray; border: 1px solid #CECECE;" onblur="time_blur(this)" onfocus="time_focus(this)" name="time_config" id="time_config"/>
        </label></td>
      </tr>
</table>
<input class="bt" type="submit" value="确定"/>
</form>
<br /><hr /><br />
<form action="ac_tunnel_conf.php" name="ac_tunnel_conf" id="ac_tunnel_conf" method="post">
<table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="ac_ntp_server">
    <th class="tdContent" colspan="2"><div class="th">隧道配置</div></th>      
	  <tr>
        <td class="tdContentF9">转发模式</td>
        <td class="tdContentF9" align="left">
        <select id="central_forward_mode" name="central_forward_mode" style="width: 50%;">
        		<option value="0">二层隧道</option>
                <option value="1">三层隧道或本地转发</option>            
        </select>
        </td>
    </tr>
    <tr>
        <td class="tdContent">隧道出口</td>
        <td class="tdContent" align="left"><input type="text" name="tunnel_out" id="tunnel_out" /></td>
      </tr>
      <tr>
        <td class="tdContentF9">隧道入口ip</td>
        <td class="tdContentF9" align="left"><input type="text" name="tunnel_in_ip" id="tunnel_in_ip" /></td>
      </tr>
</table>
<input class="bt" type="submit" value="确定"/>
</form>
<br /><hr /><br />
<form action="ac_portal.php" name="ac_portal" enctype="multipart/form-data" id="ac_portal" method="post">
<table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="ac_ntp_server">
    <th class="tdContent" colspan="2"><div class="th">AC PORTAL</div></th>      
	  <tr>
        <td class="tdContentF9">强制推送</td>
        <td class="tdContentF9" align="left">
        <select id="ac_portal_sw" name="ac_portal_sw" style="width: 60px;">
        		<option value="0">关闭</option>
                <option value="1">开启</option>            
        </select>
        </td>
    </tr>
    <tr>
        <td class="tdContent">portal ip</td>
        <td class="tdContent" align="left"><input type="text" name="redirect_ip" id="redirect_ip" /></td>
      </tr>
      <tr>
        <td class="tdContentF9">ip白名单(多IP以%分隔)</td>
        <td class="tdContentF9" align="left"><input type="text" name="portal_white_list" id="portal_white_list" /></td>
      </tr>
      <tr>
        <td class="tdContent">portal升级</td>
        <td class="tdContent" align="left">
        <input type="file" name="portal_update" id="portal_update" />
        <input class="bt" type="button" onclick="portal()" value="升级"/>
        </td>
    </tr>
</table>
<input class="bt" type="submit" value="确定"/>
</form>
<br /><hr /><br />
<form action="time_reboot.php" name="time_reboot" enctype="multipart/form-data" id="time_reboot" method="post">
<table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="ac_ntp_server">
    <th class="tdContent" colspan="2"><div class="th">定时重启开关</div></th>      
	  <tr>
        <td class="tdContentF9">重启开关</td>
        <td class="tdContentF9" align="left">
        <select id="time_reboot_sw" name="time_reboot_sw" style="width: 60px;">
        		<option value="0">关闭</option>
            <option value="1">开启</option>            
        </select>
        </td>
    </tr>
    <tr>
        <td class="tdContent">重启时刻(0—23)</td>
        <td class="tdContent" align="left"><input type="text" name="timer" id="timer" /></td>
    </tr>
    
</table>
<input class="bt" type="submit" value="确定"/>
</form>
</body>
<script type="text/javascript">
    var time_config = document.getElementById("time_config");
    var ntp_server_ip = document.getElementById("ntp_server_ip");
    function change(t){
        if(t.value == "ip"){
            ntp_server_ip.style.display = "";
            time_config.style.display = "none";
        }
        if(t.value == "time"){
            ntp_server_ip.style.display = "none";
            time_config.style.display = "";
        }
    }
    var time = null;
    var s = null;
    function init(){       
        var today = new Date();
        var year = today.getFullYear();
        var month = today.getMonth()+1;
        var day = today.getDate();
        var hour = today.getHours();
        var minute = today.getMinutes();
        var second = today.getSeconds();
        time_config.value=year+"/"+month+"/"+day+" "+hour+":"+minute+":"+second;
        s = setTimeout("init()",500);
    }     
    init();    
    function time_focus(t){
        clearTimeout(s);
        time = t.value;
        t.style.background = "white";
        t.style.border = "";
    }
    function time_blur(t){
        t.style.background = "lightgray";
        t.style.border = "1px solid #CECECE";
        if(t.value == time){
            s = setTimeout("init()",500);
        }
    }    
    
    function portal(){
        var portal = document.getElementById("ac_portal");
        portal.action = portal.action + "?update=update";
        document.ac_portal.submit();
    }
    
</script>
</html>