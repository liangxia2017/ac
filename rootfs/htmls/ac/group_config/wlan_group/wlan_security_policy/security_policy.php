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
<title>安全策略组配置</title>
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
if(isset($_POST["id"]) && $_POST["id"]!=""){
        $dbhelper = new DAL();
        $security_policy_name = $dbhelper->getall("select security_policy_name,id from wlan_security_policy");
        foreach($security_policy_name as $name){
            if($name["security_policy_name"] == $_POST["security_policy_name"] && $name["id"] != $_POST["id"])
                echo "<script>alert('名称已存在');location='security_policy.php?id='+".$_POST["id"].";</script>";
        }
        if($_POST["auth_mode"] === "1"){
            $params = array(trim($_POST["security_policy_name"]),$_POST["auth_mode"],$_POST["encryption_mode"],$_POST["psk_key"],$_POST["id"]);	
            $sql = "update wlan_security_policy set security_policy_name=?, auth_mode=?, encryption_mode=?,psk_key=? where id=?";
        }else if($_POST["auth_mode"] === "2"){
            $params = array(trim($_POST["security_policy_name"]),$_POST["auth_mode"],$_POST["radius_auth_server"],$_POST["radius_auth_port"],$_POST["radius_account_server"],$_POST["radius_account_port"],$_POST["radius_key"],$_POST["encryption_mode"],$_POST["id"]);	
            $sql = "update wlan_security_policy set security_policy_name=?, auth_mode=?, radius_auth_server=?, radius_auth_port=?, radius_account_server=?, radius_account_port=?, radius_key=?, encryption_mode=? where id=?";
        }
        $update = $dbhelper->update($sql,$params);
   	    if($update>0){
		  echo "<script>alert('修改成功');location='security_policy.php?id='+".$_POST["id"].";</script>";
    	}
    }
?>



<?php
//修改操作
if(isset($_GET["id"])){
	$dbhelper = new DAL();	
	$getRecord = $dbhelper->getRow("select * from wlan_security_policy where id =".$_GET['id']);
}
?>
<script type="text/javascript"> 
	$(document).ready(function(){	   
        $("#id").val("<?php echo $getRecord->id;?>");
        $("#security_policy_name").val("<?php echo $getRecord->security_policy_name;?>"); 
		$("#auth_mode").val("<?php echo $getRecord->auth_mode;?>");
        $("#radius_auth_server").val("<?php echo $getRecord->radius_auth_server;?>");
		$("#radius_auth_port").val("<?php echo $getRecord->radius_auth_port;?>");
		$("#radius_account_server").val("<?php echo $getRecord->radius_account_server;?>");
        $("#radius_account_port").val("<?php echo $getRecord->radius_account_port;?>");
        $("#radius_key").val("<?php echo $getRecord->radius_key;?>");
        $("#encryption_mode").val("<?php echo $getRecord->encryption_mode;?>");
        $("#psk_key").val("<?php echo $getRecord->psk_key;?>");
        change();
        });


function change(){
    if(security_policy.auth_mode.selectedIndex == 0){
        document.getElementById("tr_radius_auth_server").style.display = "none";
        document.getElementById("tr_radius_auth_port").style.display = "none";
        document.getElementById("tr_radius_account_server").style.display = "none";
        document.getElementById("tr_radius_account_port").style.display = "none";
        document.getElementById("tr_radius_key").style.display = "none";
        document.getElementById("tr_encryption_mode").style.display = "";
        document.getElementById("tr_psk_key").style.display = "";
    }
    if(security_policy.auth_mode.selectedIndex == 1){
        document.getElementById("tr_radius_auth_server").style.display = "";
        document.getElementById("tr_radius_auth_port").style.display = "";
        document.getElementById("tr_radius_account_server").style.display = "";
        document.getElementById("tr_radius_account_port").style.display = "";
        document.getElementById("tr_radius_key").style.display = "";
        document.getElementById("tr_encryption_mode").style.display = "";
        document.getElementById("tr_psk_key").style.display = "none";
    }
}



</script>


<script type="text/javascript">
	$(document).ready(function(){
	$("#security_policy_config tr").mouseover(function(){
	$(this).addClass("over");
	});
	$("#security_policy_config tr").mouseout(function(){
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
<br />
<form name="security_policy" id="security_policy" action="security_policy.php" method="post">
    <input type="hidden" name="id" id="id" value="" />
    <table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="security_policy_config">
        <tr>
            <td class="tdContent" width="40%">安全策略名称：</td>
            <td class="tdContent" align="left">
            <input type="text" name="security_policy_name" id="security_policy_name" /></td>
        </tr>
        <tr>
            <td class="tdContentF9">鉴权模式：</td>
            <td class="tdContentF9" align="left">
            <select name="auth_mode" id="auth_mode" onchange="javascript:change();">
                <option value="1">PSK2/PSK</option>
                <option value="2">PEAP</option>
            </select>
        </tr>
        <tr id="tr_radius_auth_server">
            <td class="tdContent" width="40%">认证服务器IP：</td>
            <td class="tdContent" align="left">
            <input type="text" name="radius_auth_server" id="radius_auth_server" /></td>
        </tr>
        <tr id="tr_radius_auth_port">
            <td class="tdContentF9" width="40%">认证端口号：</td>
            <td class="tdContentF9" align="left">
            <input type="text" name="radius_auth_port" id="radius_auth_port" /></td>
        </tr>
        <tr id="tr_radius_account_server">
            <td class="tdContent" width="40%">计费服务器IP：</td>
            <td class="tdContent" align="left">
            <input type="text" name="radius_account_server" id="radius_account_server" /></td>
        </tr>
        <tr id="tr_radius_account_port">
            <td class="tdContentF9" width="40%">计费端口号：</td>
            <td class="tdContentF9" align="left">
            <input type="text" name="radius_account_port" id="radius_account_port" /></td>
        </tr>
        <tr id="tr_radius_key">
            <td class="tdContent" width="40%">radius认证密码：</td>
            <td class="tdContent" align="left">
            <input type="password" name="radius_key" id="radius_key" /></td>
        </tr>
        <tr id="tr_encryption_mode">
            <td class="tdContentF9">加密方式：</td>
            <td class="tdContentF9" align="left">
            <select name="encryption_mode" id="encryption_mode">
                <option value="0">AES</option>
                <option value="1">TKIP</option>
            </select>
            </td>
        </tr>
        <tr id="tr_psk_key">
            <td class="tdContent">密钥：</td>
            <td class="tdContent" align="left"><input type="password" name="psk_key" id="psk_key"/></td>
        </tr>
    </table>   <br />
<input class="bt" type="submit" value="确定" />&nbsp;&nbsp;<input class="bt" type="reset" value="重置"/>&nbsp;&nbsp;<input class="bt" type="button" onclick="location='wlan_security_policy.php'" value="返回"/>
 
</form>
</body>
</html>