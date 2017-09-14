<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";
include PATH."db/page.php";
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
/*	width:100%;*/
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

.tdRegist{
    background-color:#F3F8F7;
    padding:5px;
	text-align:right;
    color:#73938E;
    font-weight:bold;
}

-->
</style>

<script type="text/javascript">
	$(document).ready(function(){
	$("#stripe tr").mouseover(function(){
	$(this).addClass("over");
	});
	$("#stripe tr").mouseout(function(){
	$(this).removeClass("over");
	});
	});
</script>

<?php

if(isset($_GET["id"])|isset($_POST["id"])){
    if(isset($_GET["action"])){
	   $dbhelper = new DAL();	
	   $getRecord = $dbhelper->getRow("select * from wlan_security_policy where id =".$_GET['id']);
    }
    //删除操作
    else{
	   $dbhelper = new DAL();
	   $group_id="0";	
	   if(isset($_GET["id"])){
	       $group_id=$group_id.",".$_GET["id"];
           }else{
		foreach ($_POST["id"] as $v){
			$group_id=$group_id.",".$v;
		  }
	   }
	   $delete = $dbhelper->delete("delete from wlan_security_policy where id in (".$group_id.")");
    if($delete>0){
		echo "<script>alert('删除成功');location='wlan_security_policy.php?r='+Math.random();</script>";
	   }
    }
}
?>

<script type="text/javascript"> 
$(document).ready(function(){
    $("#edit_id").val("<?php echo $getRecord->id;?>");
    $("#edit_security_policy_name").val("<?php echo $getRecord->security_policy_name;?>");
    $("#edit_wlan_group").validate({
			rules:{
				"edit_security_policy_name":{
					required:true
				}
			}
		});
});	
</script>

<?php
//查询操作
$selectSql='select * from wlan_security_policy';
$countSql='select count(*) from wlan_security_policy';
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
?>
</head>

<body>
<!--<div class="title">安全策略组配置</div>-->
<form name="edit_wlan_security_policy" id="edit_wlan_security_policy" action="edit_wlan_security_policy.php" method="post">
<input type="hidden" name="edit_id" id="edit_id" value=""/>
<table cellpadding="0px" cellspacing="0px" width="50%">
    <tr>
        <td class="tdRegist">安全策略名称:</td>
        <td align="left"><input type="text" name="edit_security_policy_name" id="edit_security_policy_name"/></td>
    </tr>
    <tr>
        <td colspan="2"><input class="bt" type="submit" value="注册" />&nbsp;&nbsp;<input class="bt" type="reset" value="重置"/></td>
    </tr>
</table>
</form>
<hr />
<form name="wlan_security_policy" id="wlan_security_policy" method="post">
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table class="acinfo_table" width="100%" id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	onclick="select_all(this,'id[]')" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">安全策略名称</td>
        <td class="tdHeader">鉴权模式</td>
        <td class="tdHeader">操作</td>
      </tr>
      <?php
			$result = $page->getResult();
			$i=0;
			foreach ($result as $rs){
				$rs = (array)$rs;
				$i++;
                if($i%2 == 0){
	   ?>
      <tr>
        <td class="tdContent"><input type="checkbox" name="id[]" id="id" value="<?php echo ($rs["id"]);?>" />
		</td>
        <td class="tdContent"><?php echo $i;?>
        </td>
        <td class="tdContent"><a href="security_policy.php?id=<?php echo ($rs["id"]);?>"><?php echo $rs["security_policy_name"];?></a>
        </td>        
        <td class="tdContent">
            <div id="auth_mode">
            <?php 
            if($rs["auth_mode"] == "1"){
                echo "PSK2/PSK";
            }
            if($rs["auth_mode"] == "2"){
                echo "PEAP";
            }            
        ?>            
            </div>
        </td>
        <td class="tdContent">
        <a href="#" onclick="this.href='wlan_security_policy.php?id=<?php echo $rs["id"];?>&action=modify'">
				修改</a> &nbsp;|&nbsp;<a href="#"
					onclick="if(confirm('您确定删除？')) this.href='wlan_security_policy.php?id=<?php echo $rs["id"];?>'">删除</a>
        </td>
      </tr>
      <?php
			}else{
		?>
        <tr>
        <td class="tdContentF9"><input type="checkbox" name="id[]" id="id" value="<?php echo ($rs["id"]);?>" />
		</td>
        <td class="tdContentF9"><?php echo $i;?>
        </td>
        <td class="tdContentF9"><a href="security_policy.php?id=<?php echo ($rs["id"]);?>"><?php echo $rs["security_policy_name"];?></a>
        </td>        
        <td class="tdContentF9">
            <div id="auth_mode">
            <?php 
            if($rs["auth_mode"] == "1"){
                echo "PSK2/PSK";
            }
            if($rs["auth_mode"] == "2"){
                echo "PEAP";
            }
        ?>            
            </div>
        </td>
        <td class="tdContentF9">
        <a href="#" onclick="this.href='wlan_security_policy.php?id=<?php echo $rs["id"];?>&action=modify'">
				修改</a> &nbsp;|&nbsp;<a href="#"
					onclick="if(confirm('您确定删除？')) this.href='wlan_security_policy.php?id=<?php echo $rs["id"];?>'">删除</a>
        </td>
      </tr>
        <?php	 
			}
            }
	?>
    </table>
    </td>
    </tr>
    <tr>
		<td align="left">
                <input class="bt" type="button"	onclick="delete_all(this,'id[]','wlan_security_policy.php')" value="删除" />
		</td>
		<td  align="right" width="85%"><?php $formId='wlan_security_policy';include PATH.'db/pageTemplate.php';?>				
		</td>
	</tr>
</table>

</form>
</body>
</html>