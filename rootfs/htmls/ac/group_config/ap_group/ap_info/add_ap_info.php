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
<title>添加AP信息</title>
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
if(isset($_POST["ap_mac"]) && $_POST["ap_mac"]!=""){
        $dbhelper = new DAL();
        
        $real_ap_num = $dbhelper->getOne("select count(*) from ap_info where ap_group_name='".$_GET["ap_group_name"]."'");
        $max_ap_num = $dbhelper->getOne("select max_ap from ap_group where ap_group_name='".$_GET["ap_group_name"]."'");
        if($real_ap_num == $max_ap_num){
            echo "<script>alert('添加失败!该组license数已经上限!');location='add_ap_info.php?ap_group_name=".$_GET["ap_group_name"]."';</script>";
        }else{
        
        $count = $dbhelper->getOne("select count(*) from ap_info where ap_mac = '".$_POST["ap_mac"]."'");
        if($count > 0){
            echo "<script>if(!confirm('该地址已存在，是否覆盖？'))location='add_ap_info.php?ap_group_name=".$_GET["ap_group_name"]."';</script>";
            $head = pack("n","4096");
            $sub_id = pack("C","3");
            $value = pack("C",$_POST[$bg_channel]);
            $value = $value.pack("C",$_POST[$a_channel]);
            $subid_len = pack("C",strlen($value));
            $body = $sub_id.$subid_len.$value;
            $len = pack("n",strlen($body));
            $msg = $head.$len.$body;
            $msg = unpack("H*",$msg);
            $params = array($_GET["ap_group_name"],$_POST["ap_remark"],$_POST["bg_channel"],$_POST["a_channel"],$_POST["ap_mac"]);
            $sql = "update ap_info set ap_group_name=?,ap_remark=?,bg_channel=?,a_channel=?,config_status=x'".$msg[1]."',config_mask=7 where ap_mac=?";	
            $update = $dbhelper->update($sql,$params);
            if($update > 0){
                echo "<script>alert('覆盖成功');location='ap_info.php?ap_group_name=".$_GET["ap_group_name"]."';</script>";
            }
        }else{
            $head = pack("n","4096");
            $sub_id = pack("C","3");
            $value = pack("C",$_POST[$bg_channel]);
            $value = $value.pack("C",$_POST[$a_channel]);
            $subid_len = pack("C",strlen($value));
            $body = $sub_id.$subid_len.$value;
            $len = pack("n",strlen($body));
            $msg = $head.$len.$body;
            $msg = unpack("H*",$msg);   
        $params = array($_GET["ap_group_name"],$_POST["ap_mac"],$_POST["ap_remark"],$_POST["bg_channel"],$_POST["a_channel"]);
            $sql = "insert into ap_info(ap_group_name,ap_mac,ap_remark,bg_channel,a_channel,config_status) values(?,?,?,?,?,x'".$msg[1]."')";	
        $insert = $dbhelper->insert($sql,$params);
        if($insert > 0){
            echo "<script>alert('添加成功');location='ap_info.php?ap_group_name=".$_GET["ap_group_name"]."';</script>";
        }
        }
    }
    }
?>

<script type="text/javascript">
$(document).ready(function(){
	$("#add_ap_info tr").mouseover(function(){
	$(this).addClass("over");
	});
	$("#add_ap_info tr").mouseout(function(){
	$(this).removeClass("over");
	});
       
	});

	function add(t,action){
	t.form.action=action;
	t.form.submit();	
	}
    
    function mac_change(){
        var temp = document.getElementById("mac").value;
        var ap_mac = temp.replace(/:|：| /g,"").toLowerCase();
        //var mac = "x'" + ap_mac + "'";
        var val = document.getElementById("ap_mac");
        //val.value = mac;
        val.value = ap_mac;
    }
    
</script>
    
</head>

<body>
<div class="title">添加AP信息</div>
<br />
<form name="add_ap_info" id="add_ap_info" method="post">
    <input type="hidden" name="id" id="id" value="" />
    <table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="add_ap_info">
        <tr>
            <td class="tdContent" width="40%">AP组名称</td>
            <td class="tdContent" align="left">
            <input class="input" name="ap_group_name" id="ap_group_name" value="<?php echo $_GET["ap_group_name"];?>" disabled="disabled"/>
            </td>
        </tr>
        <tr>
            <td class="tdContentF9">AP MAC地址</td>
            <td class="tdContentF9" align="left">
            <input type="text" id="mac" onchange="mac_change()" maxlength="17" /> 
            <input type="hidden" id="ap_mac" name="ap_mac" />           
            </td>
        </tr>
        <tr>
            <td class="tdContent">备注信息</td>
            <td class="tdContent" align="left"><input type="text" name="ap_remark" id="ap_remark"/></td>
        </tr>
        <tr>
            <td class="tdContentF9">bg卡信道</td>
            <td class="tdContentF9" align="left">
            <select name="bg_channel">
            <?php
            //bg 1-13
                $bg_channel = array_combine(range(1,13),range(1,13));
                foreach($bg_channel as $bg)
                    echo "<option value='".$bg."'>".$bg."</option>";
            ?>
            </select>
            </td>
        </tr>
        <tr>
            <td class="tdContent" width="40%">a卡信道</td>
            <td class="tdContent" align="left">
            <select name="a_channel">
            <?php
            //a 36-64,149-165
                $temp = array_merge(range(36,64,4),range(149,165,4));
                $a_channel = array_combine($temp,$temp);
                foreach($a_channel as $a)
                    echo "<option value='".$a."'>".$a."</option>";
            ?>
            </select>
            </td>
        </tr>
     </table>
<br />
<input class="bt" type="submit" value="确定" />
<input class="bt" type="button"	onclick="location='ap_info.php?ap_group_name=<?php echo $_GET["ap_group_name"]?>'" value="返回" /> 
</form>
</body>
</html>