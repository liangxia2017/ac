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
<title>AP信息注册</title>
</head>

<style type="text/css">
<!--
.acinfo_table{
	background:#a8c7ce;
	width:100%;
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
//删除操作
if(isset($_GET["id"])|isset($_POST["id"])){
	$dbhelper = new DAL();
	$group_id="0";	
	if(isset($_GET["id"])){
		$group_id=$group_id.",".$_GET["id"];
	}else{
		foreach ($_POST["id"] as $v){
			$group_id=$group_id.",".$v;
		}
	}	
	$delete = $dbhelper->delete("delete from ap_version where id in (".$group_id.")");
	if($delete>0){
		echo "<script>alert('操作成功');location='ap_version.php?r='+Math.random();</script>";
	}
}else   if(isset($_POST["regist_manufacturer"])){
    $dbhelper = new DAL();
    $params = array(trim($_POST["regist_manufacturer"]),trim($_POST["regist_hardware_version"]),trim($_POST["regist_product_model"]));	
	$sql = "insert into ap_version values(null,?,?,?)";
    $insert = $dbhelper->insert($sql,$params);
	if($insert>0){
		echo "<script>alert('注册成功');location='ap_version.php?r='+Math.random();</script>";
	}
}
//查询操作
$selectSql='select * from ap_version';
$countSql='select count(*) from ap_version';
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
?>
</head>

<body>
<!--<div class="title">AP信息注册</div>-->
<br />
<form name="regist_ap_version" id="regist_ap_version" action="ap_version.php" method="post">
<table cellpadding="0px" cellspacing="0px" width="30%">
    <tr>
        <td class="tdRegist">厂商:</td>
        <td align="left"><input type="text" name="regist_manufacturer" id="regist_manufacturer"/></td>
    </tr>
    <tr>
        <td class="tdRegist">硬件版本:</td>
        <td align="left"><input type="text" name="regist_hardware_version" id="regist_hardware_version"/></td>
    </tr>
    <tr>
        <td class="tdRegist">设备型号:</td>
        <td align="left"><input type="text" name="regist_product_model" id="regist_product_model"/></td>
    </tr>
    <tr>
        <td colspan="2"><input class="bt" type="submit" value="注册" />&nbsp;&nbsp;<input class="bt" type="reset" value="重置"/></td>
    </tr>
</table>
</form>
<hr />
<form name="ap_version" id="ap_version" method="post">
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table class="acinfo_table"  id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	onclick="select_all(this,'id[]')" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">厂商</td>
        <td class="tdHeader">硬件版本</td>
        <td class="tdHeader">设备型号</td>
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
        <td class="tdContent"><div id="manufacturer"><?php echo $rs["manufacturer"];?></div>
        </td>
        <td class="tdContent"><div id="hardware_version"><?php echo $rs["hardware_version"];?></div>
        </td>
        <td class="tdContent"><div id="product_model"><?php echo $rs["product_model"];?></div>
        <td class="tdContent">
        <a href="#" onclick="if(confirm('您确定删除？')) this.href='ap_version.php?id=<?php echo $rs["id"];?>'">删除</a>
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
        <td class="tdContentF9"><div id="manufacturer"><?php echo $rs["manufacturer"];?></div>
        </td>
        <td class="tdContentF9"><div id="hardware_version"><?php echo $rs["hardware_version"];?></div>
        </td>
        <td class="tdContentF9"><div id="product_model"><?php echo $rs["product_model"];?></div>
        <td class="tdContentF9">
        <a href="#" onclick="if(confirm('您确定删除？')) this.href='ap_version.php?id=<?php echo $rs["id"];?>'">删除</a>
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
		<td width="15%">
			<div align="left">
                <input class="bt" type="button"	onclick="delete_all(this,'id[]','ap_version.php')" value="删除" />
            </div>
		</td>
		<td  align="right" width="85%"><?php $formId='ap_version';include PATH.'db/pageTemplate.php';?>				
		</td>
	</tr>
</table>

</form>
</body>
</html>