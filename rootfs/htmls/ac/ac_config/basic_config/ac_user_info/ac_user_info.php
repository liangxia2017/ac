<?php
    session_start();
?>
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
<title>账号管理</title>
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

if(isset($_POST["user_id"])){
    $dbhelper = new DAL();
    $old_password = crypt($_POST["user_old_pwd"],$_POST["user_old_pwd"]);
    $select = $dbhelper->getOne("select count(*) from ac_user_info where id=".$_POST["user_id"]." and password='".$old_password."'");
    if($select > 0){
        $new_password = crypt($_POST["user_new_pwd"],$_POST["user_new_pwd"]);
        $update = $dbhelper->update("update ac_user_info set password='".$new_password."' where id=".$_POST["user_id"]);
        if($update > 0){
            echo "<script>alert('密码修改成功,请重新登录!');top.location='../../../../logout.php?r='+Math.random();</script>";
        }
    }else{
        echo "<script>alert('原始密码错误!');location='ac_user_info.php?r='+Math.random();</script>";
    }
}
if(!isset($_SESSION["USER_ID"])){
    echo "<script>alert('请先登录!');top.location='".PATH."login.php';</script>";
}
$dbhelper = new DAL();
$getall = $dbhelper->getall("select * from ac_user_info where id = ".$_SESSION["USER_ID"]);
$ac_user_info = $getall[0];
?>

<script type="text/javascript"> 
	$(document).ready(function(){
        $("#ac_user_info tr").mouseover(function(){
	$(this).addClass("over");
	});
    	$("#ac_user_info tr").mouseout(function(){
	$(this).removeClass("over");
	});
	});
    
    function modify_pwd(t){
        if(t.form.user_new_pwd.value != t.form.user_new_psw_confirm.value){
            alert("两次密码不一致!");
        }else{
            t.form.submit();
        }
    }
    
</script>


</head>

<body>
<!--<div class="title">修改密码</div>-->
<br />
<form action="ac_user_info.php" method="post" >
    <input type="hidden" name="user_id" id="user_id" value="<?php echo $_SESSION["USER_ID"];?>" />
    <input type="hidden" name="user_pwd" id="user_pwd" value="<?php echo $ac_user_info['password'];?>" />
	<table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="ac_user_info">
      <tr>
        <td class="tdContent" width="40%">用户名</td>
        <td class="tdContent"  align="left"><?php echo $ac_user_info['user_name'];?></div></td>
      </tr>
	  <tr>
        <td class="tdContentF9">原始密码</td>
        <td class="tdContentF9" align="left"><input type="password" id="user_old_pwd" name="user_old_pwd" />
        </td>
      </tr>
	  <tr>
        <td class="tdContent">新密码</td>
        <td class="tdContent" align="left"><input type="password" id="user_new_pwd" name="user_new_pwd" />
        </td>
      </tr>
      <tr>
        <td class="tdContent">确认新密码</td>
        <td class="tdContent" align="left"><input type="password" id="user_new_psw_confirm" name="user_new_psw_confirm" />
        </td>
      </tr>
</table>
<br />
<input type="button" class="bt" value="确认" onclick="modify_pwd(this)" />


</form>
</body>
</html>