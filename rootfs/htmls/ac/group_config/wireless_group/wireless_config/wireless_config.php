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
<title>无线组配置</title>
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

tr.over td{
    background-color:#d5f4fe;
}

-->
</style>

<?php 

if(isset($_GET["group_name"])){
	$dbhelper = new DAL();	
	$getRecord = $dbhelper->getRow("select * from wireless_config where wireless_group_name ='".$_GET['group_name']."'");
}

?>




<script type="text/javascript"> 
	$(document).ready(function(){
	   $("#id").val("<?php echo $getRecord->id;?>");
       $("#wireless_group_name").text("<?php echo $_GET['group_name'];?>");
       <?php 
       if($getRecord->beacon_intval != "" | $getRecord->beacon_intval != null){
       	?>
       $("#beacon_intval").val("<?php echo $getRecord->beacon_intval;?>");
       <?php
        }if($getRecord->rts != "" | $getRecord->rts != null){
        ?>
       $("#rts").val("<?php echo $getRecord->rts;?>");
        <?php 
       }if($getRecord->weak_rssi_refuse != "" | $getRecord->weak_rssi_refuse != null){
       	?>
       	$("#weak_rssi").val("<?php echo $getRecord->weak_rssi_refuse;?>");
       <?php }?>
       $("#auto_channel_sw").val("<?php echo $getRecord->auto_channel_sw;?>");
       $("#auto_channel_mode").val("<?php echo $getRecord->auto_channel_mode;?>");
       $("#auto_channel_period").val("<?php echo $getRecord->auto_channel_period;?>");
       $("#first_5G").val("<?php echo $getRecord->first_5G;?>");

       $("#radar_sw").val("<?php echo $getRecord->close_radar;?>");
       card_id();
        
$("#wireless_config tr").mouseover(function(){
	$(this).addClass("over");
	});
$("#wireless_config tr").mouseout(function(){
	$(this).removeClass("over");
	}); 
        });

function add(t,action){
	t.form.action=action;
	t.form.submit();	
	}


function card_id(){
    if(document.getElementById("radio_card_id").value == 1){
        document.getElementById("tr_a_beacon_rate_set").style.display = "none";
        document.getElementById("tr_bg_beacon_rate_set").style.display = "";
        document.getElementById("tr_a_wireless_mode").style.display = "none";
        document.getElementById("tr_bg_wireless_mode").style.display = "";
        
        //bg 卡1 
        <?php if($getRecord->id != "" && $getRecord->id != null){?>
        $("#txpower").val("<?php echo $getRecord->bg_txpower;?>");
        $("#bg_wireless_mode").val("<?php echo $getRecord->bg_wireless_mode;?>");
        $("#data_stream").val("<?php echo $getRecord->bg_data_stream;?>");
        <?php }?>
		$("#auto_power_time").val("<?php echo $getRecord->bg_auto_power_time;?>");
        $("#channel_width").val("<?php echo $getRecord->bg_channel_width;?>");
        <?php if($getRecord->bg_short_gi != "" | $getRecord->bg_short_gi != null){?>
        $("#short_gi").val("<?php echo $getRecord->bg_short_gi;?>");
        $("#ampdu").val("<?php echo $getRecord->bg_ampdu;?>");
        $("#amsdu").val("<?php echo $getRecord->bg_amsdu;?>");        
        <?php }?>
        
        rate_set = "<?php echo $getRecord->bg_beacon_rate_set;?>";
        if(rate_set != ""){
        bg_rate_set = parseInt(rate_set,16).toString(2);
        len = bg_rate_set.length;
                for(var j=0; j<12-len; j++){
                    bg_rate_set = "0" + bg_rate_set;
                }
        for(var i=bg_rate_set.length-1; i>=0; i--)	
		  if(bg_rate_set.charAt(bg_rate_set.length-1-i)==1){
			$("#td_bg_beacon_rate_set input:checkbox")[i].checked="checked";
		  }else
            $("#td_bg_beacon_rate_set input:checkbox")[i].checked="";;        

    }
    power_time();
    }
    if(document.getElementById("radio_card_id").value == 2){
        document.getElementById("tr_a_beacon_rate_set").style.display = "";
        document.getElementById("tr_bg_beacon_rate_set").style.display = "none";
        document.getElementById("tr_a_wireless_mode").style.display = "";
        document.getElementById("tr_bg_wireless_mode").style.display = "none";
        $("#td_a_beacon_rate_set input:checkbox").attr("display","none");
        //屏蔽a卡选择低速率
        document.getElementById("tr_a_beacon_rate_set").style.display = "none";
        
        //a 卡2
        <?php if($getRecord->id != "" && $getRecord->id != null){?>
        $("#txpower").val("<?php echo $getRecord->a_txpower;?>");
        $("#a_wireless_mode").val("<?php echo $getRecord->a_wireless_mode;?>");
        $("#data_stream").val("<?php echo $getRecord->a_data_stream;?>");
        <?php }?> 
		$("#auto_power_time").val("<?php echo $getRecord->a_auto_power_time;?>");
        $("#channel_width").val("<?php echo $getRecord->a_channel_width;?>");
        <?php if($getRecord->a_short_gi != "" | $getRecord->a_short_gi != null){?>
        $("#short_gi").val("<?php echo $getRecord->a_short_gi;?>");
        $("#ampdu").val("<?php echo $getRecord->a_ampdu;?>");
        $("#amsdu").val("<?php echo $getRecord->a_amsdu;?>");  
        <?php }?>      
        
        rate_set = "<?php echo $getRecord->a_beacon_rate_set;?>";
        if(rate_set != ""){
        a_rate_set = parseInt(rate_set,16).toString(2);
        len = a_rate_set.length;
                for(var j=0; j<8-len; j++){
                    a_rate_set = "0" + a_rate_set;
                }
        for(var i=a_rate_set.length-1; i>=0; i--)	
		  if(a_rate_set.charAt(i)==1){
			$("#td_a_beacon_rate_set input:checkbox")[i].checked="checked";
		  }else
            $("#td_a_beacon_rate_set input:checkbox")[i].checked="";
       
        }
        power_time();
    }
    channel_sw();
}

function power_time(){
	if(document.getElementById("txpower").value == 0){
        document.getElementById("auto_power_time").disabled = "";
    }else
	   document.getElementById("auto_power_time").disabled = "disabled";
}

function channel_sw(){
    if(document.getElementById("auto_channel_sw").value == 0){
        document.getElementById("auto_channel_mode").disabled = "disabled";
        document.getElementById("auto_channel_period").disabled = "disabled";
    }
    if(document.getElementById("auto_channel_sw").value == 1){
        document.getElementById("auto_channel_mode").disabled = "";
        channel_mode();
    }
}

function channel_mode(){
    if(document.getElementById("auto_channel_mode").value == 0){
        document.getElementById("auto_channel_period").disabled = "disabled";
    }
    if(document.getElementById("auto_channel_mode").value == 1){
        document.getElementById("auto_channel_period").disabled = "";
    }
}


//提交表格时处理低速率
function beacon_rate_set(){
    rate_set = "";
    if(document.getElementById("radio_card_id").value == 1){
    for(var i=$("#td_bg_beacon_rate_set input:checkbox").length-1; i>=0; i--){
        if($("#td_bg_beacon_rate_set input:checkbox")[i].checked == true){
            rate_set = rate_set + "1";
        }else{
            rate_set = rate_set + "0";
            }
        }
    rate = parseInt(rate_set,2).toString(16);
    len = rate.length;
    for(var i=0; i<3-len; i++)
        rate = "0" + rate;
    $("#bg_beacon_rate_set").val(rate);		
	}
    if(document.getElementById("radio_card_id").value == 2){
    for(var i=$("#td_a_beacon_rate_set input:checkbox").length-1; i>=0; i--){
        if($("#td_a_beacon_rate_set input:checkbox")[i].checked == true){
            rate_set = rate_set + "1";
        }else{
            rate_set = rate_set + "0";
            }
        }
    rate = parseInt(rate_set,2).toString(16);
    len = rate.length;
    for(var i=0; i<2-len; i++)
        rate = "0" + rate;
    $("#a_beacon_rate_set").val(rate);
    }
}

</script>

    
</head>

<body>
<form name="wireless_config" id="wireless_config" action="edit_wireless_config.php?group_name=<?php echo $_GET['group_name'];?>" method="post">
    <input type="hidden" name="id" id="id" value="" />
    <table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="wireless_config">
        <tr>
            <td class="tdContentF9" width="40%">无线组组名:</td>
            <td class="tdContentF9" align="left"><div id="wireless_group_name"></div></td>
        </tr>
        <tr>
            <td class="tdContent">网卡ID:</td>
            <td class="tdContent" align="left">
            <select name="radio_card_id" id="radio_card_id" onchange="card_id();">
                <option value="1">1</option>
                <option value="2">2</option>
            </select>
            </td>            
        </tr>
        <tr>
            <td class="tdContentF9">功率级别：</td>
            <td class="tdContentF9" align="left">
            <select name="txpower" id="txpower" onchange="power_time();">
                <option value="0">自动功率调整</option>
                <option value="1">12%</option>
                <option value="2">25%</option>
                <option value="3">37%</option>
                <option value="4">50%</option>
                <option value="5">62%</option>
                <option value="6" selected="true">75%</option>
                <option value="7">87%</option>
                <option value="8">100%</option>
                <option value="9">关闭射频</option>
            </select>
            </td>
        </tr>
		<tr>
            <td class="tdContentF9">自动功率调整周期(分钟)：</td>
            <td class="tdContentF9" align="left"><input type="text" name="auto_power_time" id="auto_power_time" />
            </td>
    </tr>
        <tr id="tr_bg_wireless_mode">
            <td class="tdContent">无线模式：</td>
            <td class="tdContent" align="left">
            <select name="bg_wireless_mode" id="bg_wireless_mode">
                <option value="0">11b only</option>
                <option value="1">11g only</option>
                <option value="2">11n only</option>
                <option value="3">11b/g</option>
                <option value="4" selected="true">11b/g/n</option>
                <option value="13">11g/n</option>
            </select>
            </td>
        </tr>
        <tr id="tr_a_wireless_mode">
            <td class="tdContent">无线模式：</td>
            <td class="tdContent" align="left">
            <select name="a_wireless_mode" id="a_wireless_mode">
                <option value="5">11a only</option>
                <option value="7" selected="true">11a/n</option>
                <option value="14">11ac only</option>
                <option value="15">11ac/a</option>
            </select>
            </td>
        </tr>
        <tr>
            <td class="tdContentF9">空间流（11N）：</td>
            <td class="tdContentF9" align="left">
            <select name="data_stream" id="data_stream">
                <option value="1">1*1</option>
                <option value="3" selected="true">2*2</option>
            </select>
            </td>
        </tr>
        <tr>
            <td class="tdContent">信道带宽：</td>
            <td class="tdContent" align="left">
            <select name="channel_width" id="channel_width">
                <option value="0">ht20</option>
                <option value="1">ht40</option>
                <option value="3">ht80</option>
            </select>
            </td>
        </tr>
        
        <tr id="tr_bg_beacon_rate_set">
            <td class="tdContentF9">低速率：</td>
            <td class="tdContentF9" align="left" id="td_bg_beacon_rate_set">
            <input type="hidden" id="bg_beacon_rate_set" name="bg_beacon_rate_set" value=""/>
            <input type="checkbox" checked="checked"/>1
            <input type="checkbox" checked="checked"/>2
            <input type="checkbox" checked="checked"/>5.5
            <input type="checkbox" checked="checked"/>11
            <input type="checkbox" checked="checked"/>6
            <input type="checkbox" checked="checked"/>9
            <input type="checkbox" checked="checked"/>12
            <input type="checkbox" checked="checked"/>18
            <input type="checkbox" checked="checked"/>24
            <input type="checkbox" checked="checked"/>36
            <input type="checkbox" checked="checked"/>48
            <input type="checkbox" checked="checked"/>54
            </td>
        </tr>
        
        <tr id="tr_a_beacon_rate_set">
            <td class="tdContentF9">低速率：</td>
            <td class="tdContentF9" align="left" id="td_a_beacon_rate_set">
            <input type="hidden" id="a_beacon_rate_set" name="a_beacon_rate_set" value=""/>            
            <input type="checkbox" checked="checked"/>6
            <input type="checkbox" checked="checked"/>9
            <input type="checkbox" checked="checked"/>12
            <input type="checkbox" checked="checked"/>18
            <input type="checkbox" checked="checked"/>24
            <input type="checkbox" checked="checked"/>36
            <input type="checkbox" checked="checked"/>48
            <input type="checkbox" checked="checked"/>54
            </td>
        </tr>  
        
        <tr>
            <td class="tdContent">Short GI：</td>
            <td class="tdContent" align="left">
            <select name="short_gi" id="short_gi">
                <option value="0">关闭</option>
                <option value="1" selected="true">开启</option>
            </select>
            </td>
        </tr>
        <tr>
            <td class="tdContentF9">A-MPDU：</td>
            <td class="tdContentF9" align="left">
            <select name="ampdu" id="ampdu">
                <option value="0">关闭</option>
                <option value="1" selected="true">开启</option>
            </select>
            </td>
        </tr>
        <tr>
            <td class="tdContent">A-MSDU：</td>
            <td class="tdContent" align="left">
            <select name="amsdu" id="amsdu">
                <option value="0">关闭</option>
                <option value="1" selected="true">开启</option>
            </select>
            </td>
        </tr>
       
        <tr>
            <td class="tdContentF9">beacon帧发送间隔(ms):</td>
            <td class="tdContentF9" align="left"><input type="text" name="beacon_intval" id="beacon_intval" value="150"/>(100-1000)
            </td>            
        </tr>
        <tr>
            <td class="tdContent">RTS/CTS阈值:</td>
            <td class="tdContent" align="left"><input type="text" name="rts" id="rts" value="2346" />(1-2346)
        </tr>
        <tr>
            <td class="tdContentF9">自动信道调整:</td>
            <td class="tdContentF9" align="left">
            <select name="auto_channel_sw" id="auto_channel_sw" onchange="channel_sw()">
                <option value="0" selected="true">关闭</option>
                <option value="1">开启</option>
            </select>
            </td>            
        </tr>
        <tr>
            <td class="tdContent">信道调整方式:</td>
            <td class="tdContent" align="left">
            <select name="auto_channel_mode" id="auto_channel_mode" onchange="channel_mode();">
                <option value="0">启动时调整</option>
                <option value="1" selected="true">周期性调整</option>
            </select>
            </td>
        </tr>
        <tr>
            <td class="tdContentF9">信道调整周期(分钟):</td>
            <td class="tdContentF9" align="left"><input type="text" name="auto_channel_period" id="auto_channel_period" />(5-1440)
            </td>            
        </tr>
        <tr>
            <td class="tdContent">5G优先接入:</td>
            <td class="tdContent" align="left">
            <select name="first_5G" id="first_5G">
                <option value="0" selected="true">关闭</option>
                <option value="1">开启</option>
            </select>
            </td>
        </tr>
        <tr>
            <td class="tdContentF9">弱信号止接入阈值:</td>
            <td class="tdContentF9" align="left"><input type="text" name="weak_rssi" id="weak_rssi" value="0" />(rssi:0-90)
        </tr>
				<tr>
            <td class="tdContent">5.1G频段开关:</td>
            <td class="tdContent" align="left">
            <select name="radar_sw" id="radar_sw">
                <option value="0">关闭</option>
                <option value="1" selected="true">开启</option>
            </select>
            </td>
        </tr>
    </table>   <br />
<input class="bt" type="submit" onclick="beacon_rate_set()" value="确定" />&nbsp;&nbsp;<input class="bt" type="reset" onclick="window.location.reload(true)" value="刷新"/>&nbsp;&nbsp;<input class="bt" type="button" onclick="location='../wireless_group/wireless_group.php'" value="返回"/>
 
</form>
</body>
</html>