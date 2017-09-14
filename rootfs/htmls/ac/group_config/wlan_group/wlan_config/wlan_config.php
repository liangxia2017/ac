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
<title>ssid</title>
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
	$delete = $dbhelper->delete("delete from wlan_config where id in (".$group_id.")");
	if($delete>0){
		echo "<script>alert('删除成功');location='wlan_config.php?group_name=".$_GET["group_name"]."';</script>";
	}
}
//查询操作
$dbhelper = new DAL();
$ssid = $dbhelper->getall("select * from wlan_config where wlan_group_name='".$_GET["group_name"]."' order by wlan_id");
?>
</head>
<script>
	function add(t,action){
	t.form.action=action;
	t.form.submit();	
	}
</script>
<body>

<form name="wlan_config" id="wlan_config" method="post">
	<table align="center" width="100%">
      <tr>
      <td>
        <table class="acinfo_table"  id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	onclick="select_all(this,'id[]')" /></td>
        <td class="tdHeader">WLAN ID</td>
        <td class="tdHeader">WLAN组名称</td>
        <td class="tdHeader">SSID名称</td>
        <td class="tdHeader">安全策略名称</td>
        <td class="tdHeader">SSID是否隐藏</td>
        <td class="tdHeader">最大用户数</td>
        <td class="tdHeader">上行SSID流量限制</td>
        <td class="tdHeader">下行SSID流量限制</td>
        <td class="tdHeader">上行用户流量限制</td>
        <td class="tdHeader">下行用户流量限制</td>
        <td class="tdHeader">转发模式</td>
        <td class="tdHeader">操作</td>
      </tr>
      <?php
			$i=0;
			foreach ($ssid as $rs){
				$rs = (array)$rs;
				$i++;
                if($i%2 == 0){
	   ?>
      <tr>
        <td class="tdContent"><input type="checkbox" name="id[]" id="id" value="<?php echo $rs["id"];?>" />
		</td>
        <td class="tdContent"><?php echo $rs["wlan_id"];?>
        </td>
        <td class="tdContent"><?php echo $rs["wlan_group_name"];?>
        </td>
        <td class="tdContent"><?php echo $rs["ssid"];?>
        </td>
        <td class="tdContent"><?php echo $rs["security_policy"];?>
        </td>
        <td class="tdContent">
        <?php 
            if($rs["ssid_hide_sw"])
                echo "隐藏";
            else
                echo "不隐藏";
        
        ?>
        </td>
        <td class="tdContent"><?php echo $rs["max_user"];?>
        </td>
        <td class="tdContent"><?php echo $rs["ssid_up_traffic"];?>
        </td>
        <td class="tdContent"><?php echo $rs["ssid_down_traffic"];?>
        </td>
        <td class="tdContent"><?php echo $rs["user_up_traffic"];?>
        </td>
        <td class="tdContent"><?php echo $rs["user_down_traffic"];?>
        </td>
        <td class="tdContent"><?php if($rs["forward_mode"] == 0) echo "本地"; else echo "集中";?>
        </td>
        <td class="tdContent">
        <a href="#" onclick="this.href='edit_wlan_config.php?id=<?php echo $rs["id"];?>&group_name=<?php echo $_GET["group_name"];?>'">
				修改</a> &nbsp;|&nbsp;<a href="#"
					onclick="if(confirm('您确定删除？')) this.href='wlan_config.php?id=<?php echo $rs["id"];?>&group_name=<?php echo $_GET["group_name"];?>'">删除</a>
        </td>
      </tr>
      <?php
			}else{
		?>
        <tr>
        <td class="tdContentF9"><input type="checkbox" name="id[]" id="id" value="<?php echo $rs["id"];?>" />
		</td>
        <td class="tdContentF9"><?php echo $rs["wlan_id"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["wlan_group_name"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["ssid"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["security_policy"];?>
        </td>
        <td class="tdContentF9">
        <?php 
            if($rs["ssid_hide_sw"])
                echo "隐藏";
            else
                echo "不隐藏";
        
        ?>
        </td>
        <td class="tdContentF9"><?php echo $rs["max_user"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["ssid_up_traffic"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["ssid_down_traffic"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["user_up_traffic"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["user_down_traffic"];?>
        </td>
        <td class="tdContentF9"><?php if($rs["forward_mode"] == 0) echo "本地"; else echo "集中";?>
        </td>
        <td class="tdContentF9">
        <a href="#" onclick="this.href='edit_wlan_config.php?id=<?php echo $rs["id"];?>&group_name=<?php echo $_GET["group_name"];?>'">
				修改</a> &nbsp;|&nbsp;<a href="#"
					onclick="if(confirm('您确定删除？')) this.href='wlan_config.php?id=<?php echo $rs["id"];?>&group_name=<?php echo $_GET["group_name"];?>'">删除</a>
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
		<td>
			<div align="left">
                <input class="bt" type="button"	onclick="delete_all(this,'id[]','wlan_config.php?group_name=<?php echo $_GET["group_name"];?>')" value="删除" />
                <?php
                    if(count($ssid) < 8){
                ?>
                <input class="bt" type="button"	onclick="location='edit_wlan_config.php?group_name=<?php echo $_GET["group_name"];?>'" value="添加" />
                <?php
                    }else{
                        ?>
                <input class="bt" type="button"	onclick="location='edit_wlan_config.php?group_name=<?php echo $_GET["group_name"];?>'" value="添加" disabled="disabled" />
                <?php
                }
                ?>
                <input class="bt" type="button"	onclick="location='../wlan_group/wlan_group.php'" value="返回" />
                <input class="bt" type="button"	onclick="javascript:window.location.reload();" value="刷新" />
            </div>
		</td>
	</tr>
</table>

</form>
</body>
</html>