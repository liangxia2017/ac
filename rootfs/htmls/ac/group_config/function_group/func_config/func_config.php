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
<title>功能组配置</title>
</head>

<style type="text/css">
<!--

.acinfo_table{
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

.th{
    background-color:#ffffff;
    padding:5px;
	text-align:center;
    color:#73938E;
    font-weight:bold;
}

tr.over td{
    background-color:#d5f4fe;
}

-->
</style>

<?php 
if(isset($_GET["group_name"])){
	$dbhelper = new DAL();	
	$getRecord = $dbhelper->getRow("select * from func_config where function_group_name ='".$_GET['group_name']."'");
}
?>
<script type="text/javascript"> 
$(document).ready(function(){
       $("#function_group_name").text("<?php echo $_GET['group_name'];?>");
    <?php if($getRecord->id != null | $getRecord->id != ""){?>
       $("#id").val("<?php echo $getRecord->id;?>");
       $("#keeplive_period").val("<?php echo $getRecord->keeplive_period;?>");
       $("#snmp_period").val("<?php echo $getRecord->snmp_period;?>");
       $("#event_sta_updown").val("<?php echo $getRecord->event_sta_updown;?>");
       $("#link_check_sw").val("<?php echo $getRecord->link_check_sw;?>");
       $("#link_check_action").val("<?php echo $getRecord->link_check_action;?>");
       $("#ap_ntp_sw").val("<?php echo $getRecord->ap_ntp_sw;?>");
       $("#ap_ntp_server").val("<?php echo $getRecord->ap_ntp_server;?>");
       $("#ap_ntp_period").val("<?php echo $getRecord->ap_ntp_period;?>");       
       $("#ap_locate_sw").val("<?php echo $getRecord->ap_locate_sw;?>");
       $("#ap_locate_report_period").val("<?php echo $getRecord->ap_locate_report_period;?>");
       $("#ap_url_sw").val("<?php echo $getRecord->ap_url_sw;?>");
       $("#ap_url_str").val("<?php echo $getRecord->ap_url_str;?>");
       $("#ap_white_list").val("<?php echo $getRecord->ap_white_list;?>");  
       $("#ap_log_sw").val("<?php echo $getRecord->ap_log_sw;?>");
       $("#ap_log_period").val("<?php echo $getRecord->ap_log_period;?>");
       $("#ap_cmd_sw").val("<?php echo $getRecord->ap_cmd_sw;?>");
       $("#ap_cmd").val("<?php echo $getRecord->ap_cmd;?>");
       $("#dns_deny_sw").val("<?php echo $getRecord->dns_deny_sw;?>");
       $("#dns_deny").val("<?php echo $getRecord->dns_deny;?>");
       $("#rsync_sw").val("<?php echo $getRecord->rsync_sw;?>");
       $("#rsync_period").val("<?php echo $getRecord->rsync_period;?>");
       $("#rsync_port").val("<?php echo $getRecord->rsync_port;?>");
       $("#rsync_ip").val("<?php echo $getRecord->rsync_ip;?>");
       $("#dns_white").val("<?php echo $getRecord->dns_white;?>");
     <?php }?>   
$("#func_config tr").mouseover(function(){
	$(this).addClass("over");
	});
$("#func_config tr").mouseout(function(){
	$(this).removeClass("over");
	}); 
    
    link_check();
    ap_ntp();
    ap_locate();
    ap_url();
    ap_log_js();
    ap_cmd_js();
    dns_deny_js();   
    rsync_sw_js(); 
 });

function add(t,action){
	t.form.action=action;
	t.form.submit();	
	}

function link_check(){
    if(document.getElementById("link_check_sw").value == 0)
        document.getElementById("link_check_action").disabled = "disabled";
    if(document.getElementById("link_check_sw").value == 1)
        document.getElementById("link_check_action").disabled = "";
}
function ap_ntp(){
    if(document.getElementById("ap_ntp_sw").value == 0){
        document.getElementById("ap_ntp_server").disabled = "disabled";
        document.getElementById("ap_ntp_period").disabled = "disabled";
        }
    if(document.getElementById("ap_ntp_sw").value == 1){
        document.getElementById("ap_ntp_server").disabled = "";
        document.getElementById("ap_ntp_period").disabled = "";
        }
}
function ap_locate(){
    if(document.getElementById("ap_locate_sw").value == 0)
        document.getElementById("ap_locate_report_period").disabled = "disabled";
    if(document.getElementById("ap_locate_sw").value == 1)
        document.getElementById("ap_locate_report_period").disabled = "";
}
function ap_url(){
    if(document.getElementById("ap_url_sw").value == 0)
     {   document.getElementById("ap_url_str").disabled = "disabled";
    		document.getElementById("ap_white_list").disabled = "disabled";
     }
    if(document.getElementById("ap_url_sw").value == 1)
     {   
     		document.getElementById("ap_url_str").disabled = "";
     	 	document.getElementById("ap_white_list").disabled = "";
    	}
}
function ap_log_js(){
    if(document.getElementById("ap_log_sw").value == 0)
        document.getElementById("ap_log_period").disabled = "disabled";
 		else
        document.getElementById("ap_log_period").disabled = "";
}
function ap_cmd_js(){
    if(document.getElementById("ap_cmd_sw").value == 0)
        document.getElementById("ap_cmd").disabled = "disabled";
    else
        document.getElementById("ap_cmd").disabled = "";
}
function dns_deny_js(){
    if(document.getElementById("dns_deny_sw").value == 0)
     {   
        document.getElementById("dns_deny").disabled = "disabled";
     	document.getElementById("dns_white").disabled = "disabled";
    }
    else
    {    
        document.getElementById("dns_deny").disabled = "";
    	document.getElementById("dns_white").disabled = "";
    }
}
function rsync_sw_js(){
    if(document.getElementById("rsync_sw").value == 0)
     {   
      document.getElementById("rsync_period").disabled = "disabled";
     	document.getElementById("rsync_port").disabled = "disabled";
     	document.getElementById("rsync_ip").disabled = "disabled";
    }
    else
    {    
      document.getElementById("rsync_period").disabled = "";
     	document.getElementById("rsync_port").disabled = "";
     	document.getElementById("rsync_ip").disabled = "";
    }
}

</script>

    
</head>

<body>
<form name="func_config" id="func_config" action="edit_func_config.php?group_name=<?php echo $_GET['group_name'];?>" method="post">
    <input type="hidden" name="id" id="id" />
    <table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="func_config">
        <tr>
            <td class="tdContentF9" width="40%">功能组组名:</td>
            <td class="tdContentF9" align="left"><div id="function_group_name"></div></td>
        </tr>
        <th class="th" colspan="2">通信周期</th>
        <tr>
            <td class="tdContent">心跳周期(3~20分钟):</td>
            <td class="tdContent" align="left">
                <input type="text" id="keeplive_period" name="keeplive_period" />
            </td>            
        </tr>
        <tr>
            <td class="tdContentF9">上报周期(1~10倍,0为关闭):</td>
            <td class="tdContentF9" align="left">
                <input type="text" id="snmp_period" name="snmp_period" />
            (注:心跳周期的倍数)</td>            
        </tr>
        <tr>
            <td class="tdContent">事件开关(终端上下线):</td>
            <td class="tdContent" align="left">
            <select name="event_sta_updown" id="event_sta_updown"">
                <option value="0">关闭</option>
                <option value="1">开启</option>
            </select>
            </td>            
        </tr>
        <th class="th" colspan="2">上行链路完整性配置</th>
        <tr>
            <td class="tdContent">上行链路完整性检测开关:</td>
            <td class="tdContent" align="left">
            <select name="link_check_sw" id="link_check_sw" onchange="link_check()">
                <option value="0">关闭</option>
                <option value="1">开启</option>
            </select>
            </td>            
        </tr>
        <tr>
            <td class="tdContentF9">动作:</td>
            <td class="tdContentF9" align="left">
            <select name="link_check_action" id="link_check_action">
                <option value="0">关闭射频</option>
                <option value="1">重启ap</option>
                <option value="2">关闭集中转发射频</option>
            </select>
            </td>
        </tr>
        <th class="th" colspan="2">AP的NTP服务器设置</th>
        <tr>
            <td class="tdContent">开关:</td>
            <td class="tdContent" align="left">
            <select name="ap_ntp_sw" id="ap_ntp_sw" onchange="ap_ntp()">
                <option value="0">关闭</option>
                <option value="1">开启</option>
            </select>
            </td>
        </tr>
        <tr>
            <td class="tdContentF9">NTP服务器地址:</td>
            <td class="tdContentF9" align="left">
            <input type="text" id="ap_ntp_server" name="ap_ntp_server"/>
            </td>
        </tr>
        <tr>
            <td class="tdContent">时间同步间隔(h):</td>
            <td class="tdContent" align="left">
            <input type="text" id="ap_ntp_period" name="ap_ntp_period"/>
            </td>
        </tr>
        <th class="th" colspan="2">定位功能</th>        
        <tr>
            <td class="tdContentF9">开关:</td>
            <td class="tdContentF9" align="left">
            <select name="ap_locate_sw" id="ap_locate_sw" onchange="ap_locate()">
                <option value="0">关闭</option>
                <option value="1">开启</option>
            </select>
            </td>
        </tr>        
        <tr>
            <td class="tdContent">上报周期(s):</td>
            <td class="tdContent" align="left">
            <input type="text" id="ap_locate_report_period" name="ap_locate_report_period"/>
            </td>
        </tr>
        <th class="th" colspan="2">DNAT重定向（仅AP为网关时有效）</th>        
        <tr>
            <td class="tdContentF9">开关:</td>
            <td class="tdContentF9" align="left">
            <select name="ap_url_sw" id="ap_url_sw" onchange="ap_url()">
                <option value="0">关闭</option>
                <option value="1">开启</option>
            </select>
            </td>
        </tr>        
        <tr>
            <td class="tdContent">目的IP:</td>
            <td class="tdContent" align="left">
            <input type="text" id="ap_url_str" name="ap_url_str"/>
            </td>
        </tr>
        <tr>
            <td class="tdContent">IP或端口白名单(以‘%’分隔):</td>
            <td class="tdContent" align="left">
            <input type="text" id="ap_white_list" name="ap_white_list"/>
            </td>
        </tr>
        <th class="th" colspan="2">日志上报</th>        
        <tr>
            <td class="tdContentF9">开关:</td>
            <td class="tdContentF9" align="left">
            <select name="ap_log_sw" id="ap_log_sw" onchange="ap_log_js()">
                <option value="0">关闭</option>
                <option value="1">开启</option>
            </select>
            </td>
        </tr>        
        <tr>
            <td class="tdContent">上报周期(h):</td>
            <td class="tdContent" align="left">
            <input type="text" id="ap_log_period" name="ap_log_period"/>
            </td>
        </tr>
        <th class="th" colspan="2">补丁配置</th>        
        <tr>
            <td class="tdContentF9">开关:</td>
            <td class="tdContentF9" align="left">
            <select name="ap_cmd_sw" id="ap_cmd_sw" onchange="ap_cmd_js()">
                <option value="0">关闭</option>
                <option value="1">开启</option>
            </select>
            </td>
        </tr>        
        <tr>
            <td class="tdContent">配置命令:</td>
            <td class="tdContent" align="left">
            <input type="text" id="ap_cmd" name="ap_cmd" width="90%"/>
            </td>
        </tr>
        <th class="th" colspan="2">域名黑白名单</th>        
        <tr>
            <td class="tdContentF9">开关:</td>
            <td class="tdContentF9" align="left">
            <select name="dns_deny_sw" id="dns_deny_sw" onchange="dns_deny_js()">
                <option value="0">关闭</option>
                <option value="1">开启</option>
            </select>
            </td>
        </tr>        
        <tr>
            <td class="tdContent">黑名单列表(多域名以‘%’分隔):</td>
            <td class="tdContent" align="left">
            <input type="text" id="dns_deny" name="dns_deny" width="90%"/>
            </td>
        </tr>
        <tr>
            <td class="tdContentF9">白名单列表(多域名以‘%’分隔):</td>
            <td class="tdContentF9" align="left">
            <input type="text" id="dns_white" name="dns_white" width="90%"/>
            </td>
        </tr>
        <th class="th" colspan="2">内容更新</th>        
        <tr>
            <td class="tdContentF9">开关:</td>
            <td class="tdContentF9" align="left">
            <select name="rsync_sw" id="rsync_sw" onchange="rsync_sw_js()">
                <option value="0">关闭</option>
                <option value="1">开启</option>
            </select>
            </td>
        </tr>        
        <tr>
            <td class="tdContent">更新周期(1——240 min)</td>
            <td class="tdContent" align="left">
            <input type="text" id="rsync_period" name="rsync_period" width="90%"/>
            </td>
        </tr>
        <tr>
            <td class="tdContentF9">服务器端口(默认:5241)</td>
            <td class="tdContentF9" align="left">
            <input type="text" id="rsync_port" name="rsync_port" width="90%"/>
            </td>
        </tr>
        <tr>
            <td class="tdContent">更新目标(形如：192.168.1.254::sd/)</td>
            <td class="tdContent" align="left">
            <input type="text" id="rsync_ip" name="rsync_ip" width="90%"/>
            </td>
        </tr>
    </table>   <br />
<input class="bt" type="submit" value="确定" />&nbsp;&nbsp;<input class="bt" type="reset" onclick="window.location.reload(true)" value="刷新"/>&nbsp;&nbsp;<input class="bt" type="button" onclick="location='../function_group/function_group.php'" value="返回"/>
 
</form>
</body>
</html>