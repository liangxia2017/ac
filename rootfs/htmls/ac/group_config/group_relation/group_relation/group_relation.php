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
<title>分组关联策略</title>
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

.add{
    background-color:#F3F8F7;
    padding:5px;
	text-align:right;
    color:#73938E;
    font-weight:bold;
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
    
 var i = 2;
 var s = null; 
 function clock(t,action){
	t.form.action=action;
	t.form.submit();	
	}   
 function add(t,action){  
    for(var k=0; k<t.form.apply.length; k++)
        t.form.apply[k].disabled = "disabled";
    t.value = "应用中...";
    if(s != null)
        clearTimeout(s);
    if(i>0)
		s = setTimeout(function(){i--;add(t,action);},1000);
    else
        clock(t,action);
 }
</script>

<?php
$selectSql="select * from group_relation where ap_group_name != 'unknown'";
$countSql="select count(*) from group_relation where ap_group_name != 'unknown'";
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
?>

<?php    
if(isset($_GET["id"])){
    $dbhelper = new DAL();
    $id = $_GET["id"];
    $ap_group_name = $dbhelper->getOne("select ap_group_name from group_relation where id = ".$id);    
    //总应用置位
    $config_mask = $dbhelper->getOne("select config_mask from ap_info where ap_group_name = '".$ap_group_name."' order by id limit 1");
    $wlan_group = "wlan_group_name".$id;
    $wireless_group = "wireless_group_name".$id;
    $function_group = "function_group_name".$id;
    $params = array($_POST[$wlan_group],$_POST[$wireless_group],$_POST[$function_group],$id);
    if($config_mask == null || $config_mask == "")
        $config_mask = 0;
    $config_mask = $config_mask | 111;
    if($_POST[$wlan_group] == null || $_POST[$wlan_group] == "")
        $config_mask = $config_mask & 6;
    if($_POST[$wireless_group] == null || $_POST[$wireless_group] == "")
        $config_mask = $config_mask & 5;
    if($_POST[$function_group] == null || $_POST[$function_group] == "")
        $config_mask = $config_mask & 3;    
    $update_ap_info = $dbhelper->update("update ap_info set config_mask = ".$config_mask." where ap_group_name = '".$ap_group_name."'");
    
    $sql = "update group_relation set wlan_group_name=?,wireless_group_name=?,function_group_name=? where id=?";    
    $update = $dbhelper->update($sql,$params);
    if($update>0)
        echo "<script>alert('应用成功！');location='group_relation.php';</script>";
}
?>

</head>
<body>
<!--<div class="title">分组关联策略</div>-->
<br />
<form name="function_group" id="function_group" action="" method="post">
	<table align="center" width="90%">
      <tr>
      <td>
        <table class="acinfo_table" width="100%" id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">AP组组名</td>
        <td class="tdHeader">WLAN组组名</td>
        <td class="tdHeader">无线组组名</td>
        <td class="tdHeader">功能组组名</td>
        <td class="tdHeader">操作</td>
      </tr>
      <?php
			$result = $page->getResult();
			$i=1;
            $dbhelper = new DAL();
            $wlan_group_name = $dbhelper->getall("select wlan_group_name from wlan_group");
            $wireless_group_name = $dbhelper->getall("select wireless_group_name from wireless_group");
            $function_group_name = $dbhelper->getall("select function_group_name from function_group");
			foreach ($result as $rs){
				$rs = (array)$rs;
				if($i%2 == 0){
	   ?>
      <tr>
		</td>
        <td class="tdContent"><?php echo $i;?>
        </td>
        <td class="tdContent"><?php echo $rs["ap_group_name"];?>
        </td>
        <td class="tdContent">
        <select id="wlan_group_name<?php echo $rs["id"];?>" name="wlan_group_name<?php echo $rs["id"];?>">            
            <option value=''>--请选择--</option> 
            <?php 
                foreach($wlan_group_name as $wlan)
                    if($rs["wlan_group_name"] == $wlan["wlan_group_name"]){
                        echo "<option value='".$wlan["wlan_group_name"]."' selected='selected'>".$wlan["wlan_group_name"]."</option>";
                        }else{
                            echo "<option value='".$wlan["wlan_group_name"]."'>".$wlan["wlan_group_name"]."</option>";
                        }
            ?>
        </select>
        <input type="button" class="bt" onclick="add(this,'group_relation_apply.php?id=<?php echo $rs["id"];?>&flag=1')" value="应用" name="apply" />
        </td>
        <td class="tdContent">
        <select id="wireless_group_name<?php echo $rs["id"];?>" name="wireless_group_name<?php echo $rs["id"];?>">
            <option value=''>--请选择--</option>
            <?php 
                foreach($wireless_group_name as $wireless)
                    if($rs["wireless_group_name"] == $wireless["wireless_group_name"]){
                        echo "<option value='".$wireless["wireless_group_name"]."' selected='selected'>".$wireless["wireless_group_name"]."</option>";
                        }else{
                            echo "<option value='".$wireless["wireless_group_name"]."'>".$wireless["wireless_group_name"]."</option>";
                        }
            ?>
        </select>
        <input type="button" class="bt" onclick="add(this,'group_relation_apply.php?id=<?php echo $rs["id"];?>&flag=2')" value="应用" name="apply" />
        
        </td>
        <td class="tdContent">
        <select id="function_group_name<?php echo $rs["id"];?>" name="function_group_name<?php echo $rs["id"];?>">
            <option value=''>--请选择--</option>
            <?php 
                foreach($function_group_name as $function)
                    if($rs["function_group_name"] == $function["function_group_name"]){
                        echo "<option value='".$function["function_group_name"]."' selected='selected'>".$function["function_group_name"]."</option>";
                        }else{
                            echo "<option value='".$function["function_group_name"]."'>".$function["function_group_name"]."</option>";
                        }
            ?>
        </select>
        <input type="button" class="bt" onclick="add(this,'group_relation_apply.php?id=<?php echo $rs["id"];?>&flag=4')" value="应用" name="apply" />
        
        </td>
        <td class="tdContent">
        <input type="button" class="bt" onclick="add(this,'group_relation.php?id=<?php echo $rs["id"];?>')" value="全部应用" name="apply" />
        </td>
      </tr>
      <?php 
        }else{
      ?>
      <tr>
		</td>
        <td class="tdContentF9"><?php echo $i;?>
        </td>
        <td class="tdContentF9"><?php echo $rs["ap_group_name"];?>
        </td>
        <td class="tdContentF9">
        <select id="wlan_group_name<?php echo $rs["id"];?>" name="wlan_group_name<?php echo $rs["id"];?>">
            <option value=''>--请选择--</option>
            <?php 
                foreach($wlan_group_name as $wlan)
                    if($rs["wlan_group_name"] == $wlan["wlan_group_name"]){
                        echo "<option value='".$wlan["wlan_group_name"]."' selected='selected'>".$wlan["wlan_group_name"]."</option>";
                        }else{
                            echo "<option value='".$wlan["wlan_group_name"]."'>".$wlan["wlan_group_name"]."</option>";
                        }
            ?>
        </select>
        <input type="button" class="bt" onclick="add(this,'group_relation_apply.php?id=<?php echo $rs["id"];?>&flag=1')" value="应用" name="apply" />
        
        </td>
        <td class="tdContentF9">
        <select id="wireless_group_name<?php echo $rs["id"];?>" name="wireless_group_name<?php echo $rs["id"];?>">
            <option value=''>--请选择--</option>
            <?php 
                foreach($wireless_group_name as $wireless)
                    if($rs["wireless_group_name"] == $wireless["wireless_group_name"]){
                        echo "<option value='".$wireless["wireless_group_name"]."' selected='selected'>".$wireless["wireless_group_name"]."</option>";
                        }else{
                            echo "<option value='".$wireless["wireless_group_name"]."'>".$wireless["wireless_group_name"]."</option>";
                        }
            ?>
        </select>
        <input type="button" class="bt" onclick="add(this,'group_relation_apply.php?id=<?php echo $rs["id"];?>&flag=2')" value="应用" name="apply" />
        
        </td>
        <td class="tdContentF9">
        <select id="function_group_name<?php echo $rs["id"];?>" name="function_group_name<?php echo $rs["id"];?>">
            <option value=''>--请选择--</option>
            <?php 
                foreach($function_group_name as $function)
                    if($rs["function_group_name"] == $function["function_group_name"]){
                        echo "<option value='".$function["function_group_name"]."' selected='selected'>".$function["function_group_name"]."</option>";
                        }else{
                            echo "<option value='".$function["function_group_name"]."'>".$function["function_group_name"]."</option>";
                        }
            ?>
        </select>
        <input type="button" class="bt" onclick="add(this,'group_relation_apply.php?id=<?php echo $rs["id"];?>&flag=4')" value="应用" name="apply" />
        
        </td>
        <td class="tdContentF9">
        <input type="button" class="bt" onclick="add(this,'group_relation.php?id=<?php echo $rs["id"];?>')" value="全部应用" name="apply" />
        </td>
      </tr>
        <?php
        }
            $i++;	 
			}
            
	?>
    </table>
    </td>
    </tr>
    <tr>
      <td  align="right" width="85%"><?php $formId='group_relation';include PATH.'db/pageTemplate.php';?></td>
    </tr>
</table>
</form>
</body>
</html>