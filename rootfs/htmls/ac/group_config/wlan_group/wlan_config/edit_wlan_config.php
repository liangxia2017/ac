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
<title>添加/修改SSID</title>
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

.input{
    background: transparent;
    border:1px solid transparent;
}


-->
</style>
<?php
if(isset($_POST["id"]) && $_POST["id"]!=""){
        $dbhelper = new DAL();
        $params = array($_POST["ssid"],$_POST["security_policy"],$_POST["ssid_hide_sw"],
        $_POST["max_user"],$_POST["vlan_id"],$_POST["ssid_up_traffic"],$_POST["ssid_down_traffic"],$_POST["user_up_traffic"],
        $_POST["user_down_traffic"],$_POST["forward_mode"],$_POST["id"]);	
        $sql = "update wlan_config set ssid=?, security_policy=?,ssid_hide_sw=?,max_user=?,vlan_id=?,ssid_up_traffic=?,ssid_down_traffic=?,user_up_traffic=?,user_down_traffic=?,forward_mode=? where id=?";
        $update = $dbhelper->update($sql,$params);
   	    if($update>0){
            echo "<script>alert('修改成功');window.location.href='wlan_config.php?group_name=".$_GET["group_name"]."';</script>";
    	}
    }else if(isset($_POST["wlan_id"])){
        $dbhelper = new DAL();
        $params = array($_POST["wlan_id"],$_POST["vlan_id"],$_GET["group_name"],$_POST["ssid"],$_POST["security_policy"],
            $_POST["ssid_hide_sw"],$_POST["max_user"],$_POST["ssid_up_traffic"],
            $_POST["ssid_down_traffic"],$_POST["user_up_traffic"],$_POST["user_down_traffic"],$_POST["forward_mode"]);
        $sql = "insert into wlan_config(wlan_id,vlan_id,wlan_group_name,ssid,security_policy,ssid_hide_sw,
        max_user,ssid_up_traffic,ssid_down_traffic,user_up_traffic,user_down_traffic,forward_mode) values(?,?,?,?,?,?,?,?,?,?,?,?)";
        $insert = $dbhelper->insert($sql,$params);
        if($insert > 0){
            echo "<script>alert('添加成功');location='wlan_config.php?group_name=".$_GET["group_name"]."';</script>";
        }
    }

?>



<?php
//修改操作
if(isset($_GET["id"])){
	$dbhelper = new DAL();	
	$getRecord = $dbhelper->getRow("select * from wlan_config where id =".$_GET['id']);
    //var_dump($getRecord->security_policy);
}else{
    $dbhelper = new DAL();
    $getWlanId = $dbhelper->getall("select wlan_id from wlan_config where wlan_group_name='".$_GET["group_name"]."'");
    $array = range(1,8);
    for($i = 0; $i < count($getWlanId); $i++){
        unset($array[$getWlanId[$i]["wlan_id"]-1]);
    }
}
?>
<script type="text/javascript"> 
	$(document).ready(function(){	   
        $("#id").val("<?php echo $getRecord->id;?>");
		$("#wlan_id").val("<?php echo $getRecord->wlan_id;?>");        
		$("#ssid").val("<?php echo $getRecord->ssid;?>");
		$("#security_policy").val("<?php echo $getRecord->security_policy;?>");
        $("#ssid_hide_sw").val("<?php echo $getRecord->ssid_hide_sw;?>");
		<?php if($getRecord->max_user != "" | $getRecord->max_user != null){?>
        $("#max_user").val("<?php echo $getRecord->max_user;?>");
        $("#vlan_id").val("<?php echo $getRecord->vlan_id;?>");		
        $("#ssid_up_traffic").val("<?php echo $getRecord->ssid_up_traffic;?>");
		$("#ssid_down_traffic").val("<?php echo $getRecord->ssid_down_traffic;?>");
        $("#user_up_traffic").val("<?php echo $getRecord->user_up_traffic;?>");
		$("#user_down_traffic").val("<?php echo $getRecord->user_down_traffic;?>");
		$("#forward_mode").val("<?php echo $getRecord->forward_mode;?>");
		<?php }?>
	});
</script>

<script type="text/javascript">
	$(document).ready(function(){
	$("#wlan_config tr").mouseover(function(){
	$(this).addClass("over");
	});
	$("#wlan_config tr").mouseout(function(){
	$(this).removeClass("over");
	});
	});

	function add(t,action){
	t.form.action=action;
	t.form.submit();	
	}
</script>
    
</head>

<body>
<!--<div class="title">添加/修改SSID</div>-->
<br />
<form name="wlan_config" id="wlan_config" method="post">
    <input type="hidden" name="id" id="id" value="" />
    <table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="wlan_config">
        <tr>
            <td class="tdContent" width="40%">WLAN组名称</td>
            <td class="tdContent" align="left">
            <input class="input" name="wlan_group_name" id="wlan_group_name" value="<?php echo $_GET["group_name"];?>" disabled="disabled"/>
            </td>
        </tr>
        <tr>
            <td class="tdContentF9">WLAN ID</td>
            <td class="tdContentF9" align="left">
                <?php
                    if(isset($_GET["id"])){
                ?>
               <input class="input" type="text" name="wlan_id" id="wlan_id" disabled="disabled"/>
                <?php
                    }else{
                ?>
                <select id="wlan_id" name="wlan_id">
                    <?php
                        foreach($array as $ar)
                            echo "<option value=\"".$ar."\" >".$ar."</option>";
                    ?>
                </select>   
                <?php
                    }
                ?>             
            </td>
        </tr>
        <tr>
            <td class="tdContent">SSID名称</td>
            <td class="tdContent" align="left"><input type="text" name="ssid" id="ssid"/></td>
        </tr>
        <tr>
            <td class="tdContentF9">安全策略名称</td>
            <td class="tdContentF9" align="left">
            <select name="security_policy" id="security_policy">            
            <?php 
                $security_policy = $dbhelper->getall("select security_policy_name from wlan_security_policy");
                echo "<option value=''>OPEN</option>";
                foreach($security_policy as $sp)
                    echo "<option value='".$sp["security_policy_name"]."'>".$sp["security_policy_name"]."</option>";
            ?>
            </select>
            </td>
        </tr>
        <tr>
            <td class="tdContent" width="40%">SSID隐藏</td>
            <td class="tdContent" align="left">
                <select id="ssid_hide_sw" name="ssid_hide_sw">
                    <option value="0">不隐藏</option>
                    <option value="1">隐藏</option>
                </select>
            </td>
        </tr>
        <tr>
            <td class="tdContentF9">最大用户数</td>
            <td class="tdContentF9" align="left"><input type="text" name="max_user" id="max_user" value="32"/>(0~255,0:关闭)</td>
        </tr>
        <tr>
            <td class="tdContentF9">VLAN_ID</td>
            <td class="tdContentF9" align="left"><input type="text" name="vlan_id" id="vlan_id" value="0"/>(0~4096)</td>
        </tr>
        <tr>
            <td class="tdContent">上行SSID流量限制</td>
            <td class="tdContent" align="left"><input type="text" name="ssid_up_traffic" id="ssid_up_traffic"  value="0"/>(单位:kB)</td>
        </tr>
        <tr>
            <td class="tdContentF9" width="40%">下行SSID流量限制</td>
            <td class="tdContentF9" align="left"><input type="text" name="ssid_down_traffic" id="ssid_down_traffic"  value="0"/>(单位:kB)</td>
        </tr>
        <tr>
            <td class="tdContent">上行用户流量限制</td>
            <td class="tdContent" align="left"><input type="text" name="user_up_traffic" id="user_up_traffic"  value="0"/>(单位:kB)</td>
        </tr>
        <tr>
            <td class="tdContentF9">下行用户流量限制</td>
            <td class="tdContentF9" align="left"><input type="text" name="user_down_traffic" id="user_down_traffic"  value="0"/>(单位:kB)</td>
        </tr>
        <tr>
            <td class="tdContentF9">转发模式</td>
            <td class="tdContentF9" align="left">
                <select name="forward_mode" id="forward_mode">
                    <option value="0">本地</option>
                    <option value="1">集中</option>
                </select>
            </td>
        </tr>
     </table>
<br />
<input class="bt" type="submit" value="确定" />
<input class="bt" type="button"	onclick="window.location.href='wlan_config.php?group_name=<?php echo $_GET["group_name"]?>'" value="返回" /> 
</form>
</body>
</html>