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
<title>网络配置</title>
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

.item {
	font-size: 18px;
	letter-spacing:4px;
	font-weight:bold;
	text-align: left;
	color:#0099CC;
}
-->
</style>

<?php
//删除操作
if(isset($_GET["id"])|isset($_POST["id"])){
	$dbhelper = new DAL();
	if(isset($_GET["delete"])){
	   $sql = "delete from ac_network_config where network_card like '".$_GET["delete"]."%'";
	   $delete = $dbhelper->delete($sql);
    	if($delete>0){
    		echo "<script>alert('操作成功');location='ac_network_config.php?r='+Math.random();</script>";
	}	
	}else{
    	$delete = $dbhelper->delete("delete from ac_network_config where id=".$_GET["id"]);
	if($delete>0){
		echo "<script>alert('操作成功');location='ac_network_config.php?r='+Math.random();</script>";
	}
}
}
//查询操作
$selectSql='select * from ac_network_config order by network_card';
$countSql='select count(*) from ac_network_config';
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
?>
</head>
<script type="text/javascript">
$(document).ready(function(){
    $("#stripe tr").mouseover(function(){
    $(this).addClass("over");
    });
    $("#stripe tr").mouseout(function(){
    $(this).removeClass("over");
    });
});


	function add(t,action){
	t.form.action=action;
	t.form.submit();	
	}
</script>
<body>
<div class="item">网络配置:</div>
<form name="ac_network_config" id="ac_network_config" method="post">
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table class="acinfo_table"  id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">网卡</td>
        <td class="tdHeader">类型</td>
        <td class="tdHeader">地址获取方式</td><!-- 1:静态 2:DHCP 3:PPPOE -->
        <td class="tdHeader">IP地址</td>
        <td class="tdHeader">子网掩码</td>
        <td class="tdHeader" style="width: 25%;">操作</td>
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
        <td class="tdContent"><?php echo $i;?>
        </td>
        <td class="tdContent"><?php echo $rs["network_card"];?>
        </td>
        <td class="tdContent"><?php if($rs["type"] == 0) echo "注册"; else echo "配置"?>
        </td>
        <?php
            if($rs["get_ip_method"] == 1){
                echo "<td class='tdContent'>静态配置</td>";
                echo "<td class='tdContent'>".$rs["ip"]."</td>";
                echo "<td class='tdContent'>".$rs["mask"]."</td>";
            }elseif($rs["get_ip_method"] == 2){
                echo "<td class='tdContent'>DHCP</td>";
                echo "<td class='tdContent'></td>";
                echo "<td class='tdContent'></td>";
            }else{
                echo "<td class='tdContent'>PPPOE</td>";
                echo "<td class='tdContent'></td>";
                echo "<td class='tdContent'></td>";
            }
        ?>
        <td class="tdContent">
        <?php
            if($rs["type"] == 0){
                echo "<a href='#' onclick=\"this.href='add_vlan.php?action=add&id=".$rs["id"]."'\">新增VLAN</a> &nbsp;|&nbsp;";
                echo "<a href='#' onclick=\"this.href='add_ac_network_config.php?id=".$rs["id"]."'\">修改</a> &nbsp;|&nbsp;";
                echo "<a href='#' onclick=\"if(confirm('删除该注册网卡将删除关于该网卡的所有配置，您确定删除？')) this.href='ac_network_config.php?delete=".$rs["network_card"]."&id=".$rs["id"]."'\">删除</a>";
            }else{
                if(strstr($rs["network_card"],".") > 0){
                echo "<a href='#' onclick=\"this.href='add_vlan.php?action=modify&id=".$rs["id"]."'\">修改</a> &nbsp;|&nbsp;";
            }else{
                echo "<a href='#' onclick=\"this.href='add_ac_network_config.php?id=".$rs["id"]."'\">修改</a> &nbsp;|&nbsp;";
            }
                echo "<a href='#' onclick=\"if(confirm('您确定删除？')) this.href='ac_network_config.php?id=".$rs["id"]."'\">删除</a>"; 
            }
        ?>
        </td>
      </tr>
      <?php
			}else{
		?>
        <tr>
        <td class="tdContentF9"><?php echo $i;?>
        </td>
        <td class="tdContentF9"><?php echo $rs["network_card"];?>
        </td>
        <td class="tdContentF9"><?php if($rs["type"] == 0) echo "注册"; else echo "配置"?>
        </td>
        <?php
            if($rs["get_ip_method"] == 1){
                echo "<td class='tdContentF9'>静态配置</td>";
                echo "<td class='tdContentF9'>".$rs["ip"]."</td>";
                echo "<td class='tdContentF9'>".$rs["mask"]."</td>";
            }elseif($rs["get_ip_method"] == 0){
                echo "<td class='tdContentF9'>无</td>";
                echo "<td class='tdContentF9'></td>";
                echo "<td class='tdContentF9'></td>";
            }elseif($rs["get_ip_method"] == 2){
                echo "<td class='tdContentF9'>DHCP</td>";
                echo "<td class='tdContentF9'></td>";
                echo "<td class='tdContentF9'></td>";
            }else{
                echo "<td class='tdContentF9'>PPPOE</td>";
                echo "<td class='tdContentF9'></td>";
                echo "<td class='tdContentF9'></td>";
            }
        ?>
        <td class="tdContentF9">
        <?php
            if($rs["type"] == 0){
                echo "<a href='#' onclick=\"this.href='add_vlan.php?action=add&id=".$rs["id"]."'\">新增VLAN</a> &nbsp;|&nbsp;";
                echo "<a href='#' onclick=\"this.href='add_ac_network_config.php?id=".$rs["id"]."'\">修改</a> &nbsp;|&nbsp;";
                echo "<a href='#' onclick=\"if(confirm('删除该注册网卡将删除关于该网卡的所有配置，您确定删除？')) this.href='ac_network_config.php?delete=".$rs["network_card"]."&id=".$rs["id"]."'\">删除</a>";
            }else{
                if(strstr($rs["network_card"],".") > 0){
                echo "<a href='#' onclick=\"this.href='add_vlan.php?action=modify&id=".$rs["id"]."'\">修改</a> &nbsp;|&nbsp;";
            }else{
                echo "<a href='#' onclick=\"this.href='add_ac_network_config.php?id=".$rs["id"]."'\">修改</a> &nbsp;|&nbsp;";
            }
                echo "<a href='#' onclick=\"if(confirm('您确定删除？')) this.href='ac_network_config.php?id=".$rs["id"]."'\">删除</a>"; 
            }
        ?></td>
      </tr>
        <?php	 
			}
            }
	?>
    </table>
    </td>
    </tr>
    <tr>
		<td style="text-align: left;">
                <input class="bt" type="button"	onclick="add(this,'add_ac_network_config.php')" value="添加" />
                <input class="bt" type="button"	onclick="add(this,'ac_network_config.php?apply=apply')" id="apply" value="配置应用" />
		</td>
		<td  align="right" width="80%"><?php $formId='ac_network_config';include PATH.'db/pageTemplate.php';?>				
		</td>
	</tr>
</table>
</form>
<hr />
<div class="item">静态路由:</div>
<form id="route" name="route" method="POST">
<table style="background:#a8c7ce; width: 90%; margin: 0 5%;" cellpadding="1px" cellspacing="1px">
    <tr>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">目的IP/掩码</td>
        <td class="tdHeader">下一跳IP</td>
        <td class="tdHeader">操作</td>
    </tr>
    <?php
        $dbhelper = new DAL();
		$routes = $dbhelper->getall("select * from route");        
        $i = 0;
		foreach ($routes as $route){
			$i++;
    ?>
    <tr>
        <td class="tdContent"><?php echo $i;?>
        </td>
        <td class="tdContent"><?php echo $route["source_ip"];?>
        </td>
        <td class="tdContent"><?php echo $route["destination_ip"];?>
        </td>
        <td class="tdContent">
        <a href="#" onclick="if(confirm('您确定删除？')) this.href='route.php?id=<?php echo $route["id"];?>'">删除</a>
        </td>
    </tr>
        <?php	 
			}
	?>
 </table>
 <div style="text-align: left; padding:5px 5%;">
 <input class="bt" type="button" onclick="add(this,'route.php')" value="添加" />
 <input class="bt" type="button" onclick="add(this,'route.php?action=route_apply')" id="route_apply" value="配置应用" />
 </div>
</form>
<hr />
<div class="item">NAT配置:</div>
<form id="nat" name="nat" method="POST">
<table style="background:#a8c7ce; width: 90%; margin: 0 5%;" cellpadding="1px" cellspacing="1px">
    <tr>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">源IP</td>
        <td class="tdHeader">掩码</td>
        <td class="tdHeader">出口名称</td>
        <td class="tdHeader">操作</td>
    </tr>
    <?php
        $dbhelper = new DAL();
		$nats = $dbhelper->getall("select * from nat");        
        $i = 0;
		foreach ($nats as $nat){
			$i++;
    ?>
    <tr>
		</td>
        <td class="tdContent"><?php echo $i;?>
        </td>
        <td class="tdContent"><?php echo $nat["source_ip"];?>
        </td>
        <td class="tdContent"><?php echo $nat["source_mask"];?>
        </td>
        <td class="tdContent"><?php echo $nat["output_eth"];?>
        </td>
        <td class="tdContent">
        <a href="#" onclick="if(confirm('您确定删除？')) this.href='nat.php?id=<?php echo $nat["id"];?>'">删除</a>
        </td>
    </tr>
        <?php	 
			}
	?>
 </table>
 <div style="text-align: left; padding:5px 5%;">
 <input class="bt" type="button" onclick="add(this,'nat.php')" value="添加" />
 <input class="bt" type="button" onclick="add(this,'nat.php?action=nat_apply')" value="配置应用" />
 </div>
</form>
</body>
<script type="text/javascript">
<?php
if(isset($_GET["apply"])){
    ?>
var tag = document.getElementById("apply");
var j = 5;
 function apply(t){ 
    if(j == 5){
        <?php
            exec('cd /ac/script && ./init_eth_scr',$arr,$retval);
            if($retval != 0){
                echo "alert('应用失败!');return false;";
            }
        ?>
    }        
    t.disabled = "disabled";
    t.value = "应用中...";
    if(j>0)
		setTimeout(function(){j--;apply(t);},1000);
    else{
        alert("应用成功!");
        location='ac_network_config.php?r='+Math.random();
 } 
 }
 apply(tag);
 <?php
    }
 ?>
 </script>
</html>