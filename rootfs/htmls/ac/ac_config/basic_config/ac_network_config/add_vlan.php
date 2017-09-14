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
<title>添加/修改VLAN</title>
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


-->
</style>

<?php
if(isset($_POST["id"]) && $_POST["id"]!=""){
    $network_card = $_POST["add_network_card"].".".$_POST["add_vlan_id"];
    $dbhelper = new DAL();
    
    $net_id = ip2long($_POST["add_ip"])&ip2long($_POST["add_subnet_mask"]);
    if($net_id == 0){
        $net_id = rand();
    }
    $exist = $dbhelper->getOne("select count(*) from ac_network_config where net_id=".$net_id." and id !=".$_POST["id"]);
    if($exist > 0){
        echo "<script>alert('网段已存在');location='add_vlan.php?action=modify&id=".$_POST["id"]."&r='+Math.random();</script>";
	}else{
    	$params = array($network_card,$_POST["add_ip"],$_POST["add_subnet_mask"],$net_id,$_POST["add_pppoe_user"],$_POST["add_pppoe_psw"],$_POST["id"]);
        $sql = "update ac_network_config set network_card=?, ip=?, mask=?, net_id=?, pppoe_user=?, pppoe_psw=? where id=?";	
    	$update = $dbhelper->update($sql,$params);
        if($update > 0){
            echo "<script>alert('修改成功');location='ac_network_config.php?r='+Math.random();</script>";
        }
    }

}elseif(isset($_POST["add_vlan_id"])){
    $network_card = $_POST["add_network_card"].".".$_POST["add_vlan_id"];
    $dbhelper = new DAL();
    if($_POST["get_ip_method"] == 1){
        $net_id = ip2long($_POST["add_ip"])&ip2long($_POST["add_subnet_mask"]);
        if($net_id == 0){
            $net_id = rand();
        }
        $exist = $dbhelper->getOne("select count(*) from ac_network_config where net_id=".$net_id);        
        if($exist > 0){
			echo "<script>alert('网段已存在');location='add_vlan.php?action=add&id=".$_POST["add"]."&r='+Math.random();</script>";
		}else{
        	$params = array($network_card,$_POST["get_ip_method"],$_POST["add_ip"],$_POST["add_subnet_mask"],$net_id);
            $sql = "insert into ac_network_config(network_card,type,get_ip_method,ip,mask,net_id) values(?,1,?,?,?,?)";	
        	$insert = $dbhelper->insert($sql,$params);
            if($insert > 0){
                echo "<script>alert('添加成功');location='ac_network_config.php?r='+Math.random();</script>";
            }
        }
    }else{
        $params = array($network_card,$_POST["get_ip_method"],$_POST["add_pppoe_user"],$_POST["add_pppoe_psw"]);
        $sql = "insert into ac_network_config(network_card,get_ip_method,pppoe_user,pppoe_psw,type) values(?,?,?,?,1)";	
    	$insert = $dbhelper->insert($sql,$params);
        if($insert > 0){
            echo "<script>alert('添加成功');location='ac_network_config.php?r='+Math.random();</script>";
        }
    }
}

?>

<?php

if($_GET["action"] == "add"){//添加
    $vlan = '';
    $add = $_GET['id'];
   $dbhelper = new DAL();	
   $getRecord = $dbhelper->getRow("select * from ac_network_config where id =".$_GET['id']);
   $getRecord = (array)$getRecord;
   list($network_card,$vlan_id) = array($getRecord["network_card"],'');
}elseif($_GET["action"] == "modify"){//修改
    $vlan = $_GET["id"];
    $add ='';
    $dbhelper = new DAL();	
   $getRecord = $dbhelper->getRow("select * from ac_network_config where id =".$_GET['id']);
   $getRecord = (array)$getRecord;
   list($network_card,$vlan_id) = explode(".",$getRecord["network_card"]);
}
?>   

<script type="text/javascript">
	$(document).ready(function(){
        $('#id').val('<?php echo $vlan;?>');
        $('#add').val('<?php echo $add;?>');
		$('#add_network_card').val('<?php echo $network_card;?>');
        $('#add_vlan_id').val('<?php echo $vlan_id;?>');
        $('#get_ip_method').val('<?php echo $getRecord["get_ip_method"];?>');
        $('#add_get_ip_method').val('<?php 
            if($getRecord["get_ip_method"] == 1){
                echo "静态配置";
            }elseif($getRecord["get_ip_method"] == 2){
                echo "DHCP";
            }elseif($getRecord["get_ip_method"] == 3){
                echo "PPPOE";
            }
        ?>');
        <?php 
            if($getRecord["get_ip_method"] == 1){
                echo "$('#tr_add_ip').attr('style','display:');";
                echo "$('#tr_add_subnet_mask').attr('style','display:');";
                echo "$('#tr_add_pppoe_user').attr('style','display:none');";
                echo "$('#tr_add_pppoe_psw').attr('style','display:none');";
            }elseif($getRecord["get_ip_method"] == 2){
                echo "$('#tr_add_ip').attr('style','display:none');";
                echo "$('#tr_add_subnet_mask').attr('style','display:none');";
                echo "$('#tr_add_pppoe_user').attr('style','display:none');";
                echo "$('#tr_add_pppoe_psw').attr('style','display:none');";
            }elseif($getRecord["get_ip_method"] == 3){
                echo "$('#tr_add_ip').attr('style','display:none');";
                echo "$('#tr_add_subnet_mask').attr('style','display:none');";
                echo "$('#tr_add_pppoe_user').attr('style','display:');";
                echo "$('#tr_add_pppoe_psw').attr('style','display:');";
            }
        ?>
        $('#add_get_ip_method').attr("disabled","disabled");
        $('#add_ip').val('<?php echo $getRecord["ip"];?>');
		$('#add_subnet_mask').val('<?php echo $getRecord["mask"];?>');
        $('#add_pppoe_user').val('<?php echo $getRecord["pppoe_user"];?>');
        $('#add_pppoe_psw').val('<?php echo $getRecord["pppoe_psw"];?>');
        $("#add_vlan").validate({
			rules:{
				"add_vlan_id":{
					required:true
				}
			}
		});        
	});
</script>
    
</head>

<body>
<div class="title">添加/修改VLAN</div>
<br />
<form name="add_vlan" id="add_vlan" action="add_vlan.php" method="POST">
    <input type="hidden" name="id" id="id" value="" />
    <input type="hidden" name="add" id="add" value="" />
    <table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="td_ac_network_config">
        <tr>
            <td class="tdContentF9" width="40%">网卡：</td>
            <td class="tdContentF9" align="left"><input type="text" name="add_network_card" id="add_network_card" style="border: 0; background-color: #f9f9f9;" readonly="readonly" /></td>
        </tr>
        <tr>
            <td class="tdContent" width="40%">VLAN ID：</td>
            <td class="tdContent" align="left"><input type="text" name="add_vlan_id" id="add_vlan_id"/></td>
        </tr>     
        <tr>
            <td class="tdContentF9">地址获取方式：</td>
            <td class="tdContentF9" align="left">
            <input type="hidden" name="get_ip_method" id="get_ip_method"  />
            <input type="text" name="add_get_ip_method" id="add_get_ip_method" readonly="true" />
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
<input class="bt" type="reset" value="重置"/>&nbsp;&nbsp;
<input class="bt" type="button" onclick="location='ac_network_config.php'" value="返回"/> 
</form>
</body>
</html>