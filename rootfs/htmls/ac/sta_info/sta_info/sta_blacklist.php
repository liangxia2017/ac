<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
define("PATH", "../../../");
include PATH."db/dbhelper.php";
include PATH."db/page.php";
?>
<head>
<META   HTTP-EQUIV="pragma"   CONTENT="no-cache">         
<META   HTTP-EQUIV="Cache-Control"   CONTENT="no-cache,   must-revalidate">         
<META   HTTP-EQUIV="expires"   CONTENT="0"> 
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script type="text/javascript" src="../../../js/jquery.js"></script>
<script type="text/javascript" src="../../../js/checkbox.js"></script>
<link rel="stylesheet" href="../../../css/body.css" type="text/css" />
<title>关联终端信息表</title>
</head>

<style type="text/css">
<!--

.acinfo_table{
	background:#a8c7ce;
	width:100%;
	border: 1px solid #ddeeff;
}

.tdHeader{
	background-color:#d3eaef;
	padding:5px; 
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
    $("#query").val("<?php echo $_POST["query"];?>");
    $("#mac_or_group").val("<?php echo $_POST["mac_or_group"];?>");
	});
    function add(t,action){
    	t.form.action=action;
    	t.form.submit();	
	}
    
</script>

<?php
if(isset($_POST["id"])){
    $dbhelper = new DAL();
	$group_id="0";
	foreach ($_POST["id"] as $v){
		$group_id=$group_id.",".$v;
	}
    $delete = $dbhelper->delete("delete from sta_blacklist where id in (".$group_id.")");
	if($delete>0){
		echo "<script>alert('删除成功');location='sta_blacklist.php';</script>";
	} 
}

//查询操作
if(isset($_POST["query"])){
    if($_POST["query"] == 0){
        $sta_mac = $_POST["mac_or_group"];
        $sta_mac = preg_replace("/:|：| /","",$sta_mac);
        $selectSql="select * from sta_blacklist where sta_mac like '%".$sta_mac."%'";
        $countSql="select count(*) from sta_blacklist where sta_mac like '%".$sta_mac."%'";
        $page = new Page($selectSql,$_POST["pageNow"],$countSql);
        //$result = $page->getResult();
    }
    if($_POST["query"] == 1){
        $ap_group = $_POST["mac_or_group"];
        $ap_group = preg_replace("/ /","",$ap_group);
        $selectSql="select * from sta_blacklist where ap_group_name = '".$ap_group."'";
        $countSql="select count(*) from sta_blacklist where ap_group_name = '".$ap_group."'";
        $page = new Page($selectSql,$_POST["pageNow"],$countSql);
        //$result = $page->getResult();
    }
}else{
$selectSql='select * from sta_blacklist order by ap_group_name';
$countSql='select count(*) from sta_blacklist';
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
}
?>
</head>

<body>
<form name="sta_blacklist" id="sta_blacklist" method="post">
<div style="text-align: left; padding-left: 60px;">
查询方式:<select name="query" id="query" style="margin: 0 5px;">
    <option value="0">终端MAC地址</option>
    <option value="1">所属AP分组</option>
</select>
<input type="text" id="mac_or_group" name="mac_or_group"/>
<input type="button" class="bt" onclick="add(this,'sta_blacklist.php')" value="确定" />
</div>
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table id="stripe" class="acinfo_table" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	onclick="select_all(this,'id[]')" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">用户MAC地址</td>
        <td class="tdHeader">所属AP组</td>
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
        <td class="tdContent"><input type="checkbox" name="id[]" id="id" value="<?php echo $rs["id"];?>" />
		</td>
        <td class="tdContent"><?php echo $i;?>
        </td>
        <td class="tdContent">
        <?php 
         $sta_mac = $rs["sta_mac"];
         $mac = "";
                for($k = 0; $k<strlen($sta_mac); $k++){
                    if($k != 0 && $k%2 == 0)
                        $mac = $mac.":".$sta_mac[$k];
                    else
                        $mac = $mac.$sta_mac[$k];
                }
            echo $mac;
        ?>
        </td>
        <td class="tdContent"><?php echo $rs["ap_group_name"];?>
        </td>
      </tr>
      <?php 
            }else{
       ?>
       <tr>
        <td class="tdContent"><input type="checkbox" name="id[]" id="id" value="<?php echo $rs["id"];?>" />
		</td>
        <td class="tdContentF9"><?php echo $i;?>
        </td>
        <td class="tdContentF9">
        <?php 
         $sta_mac = $rs["sta_mac"];
         $mac = "";
                for($k = 0; $k<strlen($sta_mac); $k++){
                    if($k != 0 && $k%2 == 0)
                        $mac = $mac.":".$sta_mac[$k];
                    else
                        $mac = $mac.$sta_mac[$k];
                }
            echo $mac;
        ?>
        </td>
        <td class="tdContentF9"><?php echo $rs["ap_group_name"];?>
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
        <td align="left" width="15%">
        <input class="bt" type="button" onclick="javascript:window.location.reload(true);" value="刷新" />
        <input class="bt" type="button" onclick="delete_all(this,'id[]','sta_blacklist.php')" value="移出黑名单" />
        </td>
		<td align="right" width="85%"><?php $formId='sta_blacklist';include PATH.'db/pageTemplate.php';?></td>
	</tr>
</table>
</form>
</body>
</html>