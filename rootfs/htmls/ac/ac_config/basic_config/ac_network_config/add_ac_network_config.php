﻿<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
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
<title>添加/修改网络配置</title>
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
        $is_regist = $dbhelper->getOne("select type from ac_network_config where id = ".$_POST["id"]);
        if($is_regist == 0){//修改注册网卡
            $num = "select count(*) from ac_network_config where id != ".$_POST["id"]." and network_card like (select network_card from ac_network_config where id = ".$_POST["id"].") || '%'";
            $exist = $dbhelper->getOne($num);
            if($exist > 0){//该网卡下存在配置网卡或者绑定VLAN
                echo "<script>alert('该网卡下存在配置网卡或者绑定VLAN，请删除后再修改!');location='ac_network_config.php?r='+Math.random();</script>";
            }else{
                $params = array($_POST["add_network_card"],$_POST["add_get_ip_method"],$_POST["id"]);
                $sql = "update ac_network_config set network_card=?, get_ip_method=? where id=?";	
            	$update = $dbhelper->update($sql,$params);
                if($update > 0){
                    echo "<script>alert('修改成功');location='ac_network_config.php?r='+Math.random();</script>";
        			}
        			}
        }else{
            $net_id = ip2long($_POST["add_ip"])&ip2long($_POST["add_subnet_mask"]);
            if($net_id == 0){
                $net_id = rand();
        	}
            $exist2 = $dbhelper->getOne("select count(*) from ac_network_config where net_id=".$net_id." and id !=".$_POST["id"]);
            if($exist2 > 0){
                echo "<script>alert('网段已存在');location='add_ac_network_config.php?id=".$_POST["id"]."&r='+Math.random();</script>";
    		}else{
    				if($_POST["method"] == 3){
            	$params = array($_POST["add_ip"],$_POST["add_subnet_mask"],$net_id,$_POST["add_pppoe_user"],$_POST["add_pppoe_psw"],$_POST["id"]);
            }else{
            	$params = array($_POST["add_ip"],$_POST["add_subnet_mask"],$net_id,'','',$_POST["id"]);
            }
                $sql = "update ac_network_config set ip=?, mask=?, net_id=?, pppoe_user=?, pppoe_psw=? where id=?";	
        $update = $dbhelper->update($sql,$params);
   	    if($update>0){
		  echo "<script>alert('修改成功');location='ac_network_config.php?r='+Math.random();</script>";
    	}
            }
            }
    }else if($_POST["add_network_card"]){
    $dbhelper = new DAL();    
    if($_POST["type"] == 0){//注册
        $exist = $dbhelper->getOne("select count(*) from ac_network_config where network_card='".$_POST["add_network_card"]."' and type=0");
        if($exist > 0){
            echo "<script>alert('该网卡已注册');location='add_ac_network_config.php?r='+Math.random();</script>";
        }else{
            $params = array($_POST["add_network_card"],$_POST["add_get_ip_method"],0,rand());
            $sql = "insert into ac_network_config(network_card,get_ip_method,type,net_id) values(?,?,?,?)";
        	$insert = $dbhelper->insert($sql,$params);
            if($insert > 0){
                echo "<script>alert('注册成功');location='ac_network_config.php?r='+Math.random();</script>";
            }
        }
    }else{//配置
        $sql = "select get_ip_method from ac_network_config where network_card='".$_POST["add_network_card"]."' and type = 0";
        $exist = $dbhelper->getOne($sql);
        if($exist == "" || $exist == null){
            echo "<script>alert('该网卡未注册，请先添加注册');location='add_ac_network_config.php?r='+Math.random();</script>";
        }else if($exist != $_POST["add_get_ip_method"]){
            echo "<script>alert('该配置的地址获取方式与注册网卡的地址获取方式不一致');location='add_ac_network_config.php?r='+Math.random();</script>";
        }else{
    if($_POST["add_get_ip_method"] == 1){
    $net_id = ip2long($_POST["add_ip"])&ip2long($_POST["add_subnet_mask"]);
        if($net_id == 0){
            $net_id = rand();
        			}
        $exist = $dbhelper->getOne("select count(*) from ac_network_config where net_id=".$net_id);        
        if($exist > 0){
        				echo "<script>alert('网段已存在');location='add_ac_network_config.php?r='+Math.random();</script>";
		}else{
        	$params = array($_POST["add_network_card"],$_POST["add_get_ip_method"],$_POST["add_ip"],$_POST["add_subnet_mask"],$net_id);
            $sql = "insert into ac_network_config(network_card,type,get_ip_method,ip,mask,net_id) values(?,1,?,?,?,?)";	
        	$insert = $dbhelper->insert($sql,$params);
            if($insert > 0){
                echo "<script>alert('配置成功');location='ac_network_config.php?r='+Math.random();</script>";
        			}
        	}
    }elseif($_POST["add_get_ip_method"] == 3){
            $params = array($_POST["add_network_card"],$_POST["add_get_ip_method"],rand(),$_POST["add_pppoe_user"],$_POST["add_pppoe_psw"]);
            $sql = "insert into ac_network_config(network_card,get_ip_method,net_id,type,pppoe_user,pppoe_psw) values(?,?,?,1,?,?)";	
        	$insert = $dbhelper->insert($sql,$params);
            if($insert > 0){
                echo "<script>alert('配置成功');location='ac_network_config.php?r='+Math.random();</script>";
            }
        }else{
            $params = array($_POST["add_network_card"],rand(),$_POST["add_get_ip_method"]);
            $sql = "insert into ac_network_config(network_card,net_id,type,get_ip_method) values(?,?,1,?)";	
	$insert = $dbhelper->insert($sql,$params);
    if($insert > 0){
                echo "<script>alert('配置成功');location='ac_network_config.php?r='+Math.random();</script>";
            }
        }
    }
    }
        }
?>

<?php
//修改操作
if(isset($_GET["id"])){
	$dbhelper = new DAL();	
	$getRecord = $dbhelper->getRow("select * from ac_network_config where id =".$_GET['id']);
   $getRecord = (array)$getRecord;
}
?>

<script type="text/javascript">
	$(document).ready(function(){	   
        $('#id').val('<?php echo $getRecord["id"];?>');
		$('#add_network_card').val('<?php echo $getRecord["network_card"];?>');
        $('#add_get_ip_method').val('<?php echo $getRecord["get_ip_method"];?>');
        $('#method').val('<?php echo $getRecord["get_ip_method"];?>');
        <?php
            if($getRecord["type"] == 1){
                echo "$('#config').attr('checked','checked');";
                echo "$('#config').attr('disabled','disabled');";
                echo "$('#regist').attr('disabled','disabled');";
                echo "$('#add_network_card').attr('disabled','disabled');";
                echo "$('#add_get_ip_method').attr('disabled','disabled');";
            }
        ?>
        $('#add_ip').val('<?php echo $getRecord["ip"];?>');
		$('#add_subnet_mask').val('<?php echo $getRecord["mask"];?>');
        $('#add_pppoe_user').val('<?php echo $getRecord["pppoe_user"];?>');
        $('#add_pppoe_psw').val('<?php echo $getRecord["pppoe_psw"];?>');
        $("#add_ac_network_config").validate({
			rules:{
				"add_network_card":{
					required:true
				}
			}
		});
    $("#td_ac_network_config tr").mouseover(function(){
	$(this).addClass("over");
	});
	$("#td_ac_network_config tr").mouseout(function(){
	$(this).removeClass("over");
	});
    get_ip_method();
	});

function get_ip_method(){
    var sel = document.getElementById("add_get_ip_method");
    var tr_add_ip = document.getElementById("tr_add_ip");
    var tr_add_subnet_mask = document.getElementById("tr_add_subnet_mask");
    var tr_add_pppoe_user = document.getElementById("tr_add_pppoe_user");
    var tr_add_pppoe_psw = document.getElementById("tr_add_pppoe_psw");
    var type = document.getElementById("regist");
    if(!type.checked){
        if(sel.value == 0 || sel.value == 2){
            tr_add_ip.style.display = "none";
            tr_add_subnet_mask.style.display = "none";
            tr_add_pppoe_user.style.display = "none";
            tr_add_pppoe_psw.style.display = "none";
        }else if(sel.value == 1){
            tr_add_ip.style.display = "";
            tr_add_subnet_mask.style.display = "";
            tr_add_pppoe_user.style.display = "none";
            tr_add_pppoe_psw.style.display = "none";
        }else{
            tr_add_ip.style.display = "none";
            tr_add_subnet_mask.style.display = "none";
            tr_add_pppoe_user.style.display = "";
            tr_add_pppoe_psw.style.display = "";
        }
    }
}
function type_action(t){
    var tr_add_ip = document.getElementById("tr_add_ip");
    var tr_add_subnet_mask = document.getElementById("tr_add_subnet_mask");
    var tr_add_pppoe_user = document.getElementById("tr_add_pppoe_user");
    var tr_add_pppoe_psw = document.getElementById("tr_add_pppoe_psw");
    if(t.value == 0){
        tr_add_ip.style.display = "none";
        tr_add_subnet_mask.style.display = "none";
        tr_add_pppoe_user.style.display = "none";
        tr_add_pppoe_psw.style.display = "none";
    }else{
        get_ip_method();
    }
}
</script>
    
</head>

<body>
<div class="title">添加/修改网络配置</div>
<br />
<form name="add_ac_network_config" id="add_ac_network_config" action="add_ac_network_config.php" method="POST">
    <input type="hidden" name="id" id="id" value="" />
    <table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="td_ac_network_config">
        <tr>
            <td class="tdContent" width="40%">网卡：</td>
            <td class="tdContent" align="left">
            <input type="text" name="add_network_card" id="add_network_card"/>
            <input type="radio" name="type" id="regist" value="0" checked="true" onclick="type_action(this)" />注册
            <input type="radio" name="type" id="config" value="1" onclick="type_action(this)" />配置
            </td>
        </tr>
        <tr>
            <td class="tdContentF9">地址获取方式：</td>
            <input type="hidden" name="method" id="method" value="" />
            <td class="tdContentF9" align="left">
            <select name="add_get_ip_method" id="add_get_ip_method" onchange="get_ip_method()">
                <option value="1">静态配置</option>
                <option value="2">DHCP</option>
                <option value="3">PPPOE</option>
            </select>
            </td>
        </tr>
        <tr id="tr_add_ip" style="display: none;">
            <td class="tdContent">IP地址：</td>
            <td class="tdContent" align="left"><input type="text" name="add_ip" id="add_ip"/></td>
        </tr>
        <tr id="tr_add_subnet_mask" style="display: none;">
            <td class="tdContentF9">子网掩码：</td>
            <td class="tdContentF9" align="left"><input type="text" name="add_subnet_mask" id="add_subnet_mask"/></td>
        </tr>
        <tr id="tr_add_pppoe_user" style="display: none;">
            <td class="tdContent">用户名：</td>
            <td class="tdContent" align="left"><input type="text" name="add_pppoe_user" id="add_pppoe_user"/></td>
        </tr>
        <tr id="tr_add_pppoe_psw" style="display: none;">
            <td class="tdContentF9">密码：</td>
            <td class="tdContentF9" align="left"><input type="password" name="add_pppoe_psw" id="add_pppoe_psw"/></td>
        </tr>
            </table>   <br />
<input class="bt" type="submit" value="确定" />&nbsp;&nbsp;

<input class="bt" type="button" onclick="location='ac_network_config.php'" value="返回"/> 
</form>
</body>
</html>