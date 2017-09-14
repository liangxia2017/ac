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
    $("#mac").val("<?php echo $_POST["mac"];?>");
	});
    
	function add_to_black(t,tag,action){
	var arrObj = document.getElementsByName(tag);
	var count = 0;
	for ( var i = 0; i < arrObj.length; i++) {
		if(arrObj[i].checked==true){
			count++;
		}
	}
	if(count==0){
		alert("你还没有选择要操作的项!");
	}else{
		if(confirm("你确定将这些数据删除或加入黑名单吗?")){
			t.form.action=action;				
			t.form.submit();
		}
	}
}
   function add(t,action){
    	t.form.action=action;
    	t.form.submit();	
	}
</script>

<?php
if(isset($_POST["id"])){
    //var_dump($_POST["id"]);
    $dbhelper = new DAL();
	$group_id="0";
	foreach ($_POST["id"] as $v){
		$group_id=$group_id.",".$v;
	}
    if($_GET["action"] == "del"){
        $delete = $dbhelper->delete("delete from sta_list_assc where id in (".$group_id.")");
        if($delete > 0){
            echo "<script>alert('删除成功!');location='sta_list_assc.php';</script>";
        }
    }else{        
    $sta_info = $dbhelper->getall("select sta_mac from sta_list_assc where id in (".$group_id.")");
    foreach($sta_info as $sta){
        $ap_group_name = $dbhelper->getall("select ap_group_name from ap_group where ap_group_name != 'unknown'");
        foreach($ap_group_name as $group_name){
            $num = $dbhelper->getOne("select count(*) from sta_blacklist where sta_mac = '".$sta["sta_mac"]."' and ap_group_name = '".$group_name["ap_group_name"]."'");
            if($num>0){
                continue;
            }else{
                $insert = $dbhelper->insert("insert into sta_blacklist(sta_mac,ap_group_name) values('".$sta["sta_mac"]."','".$group_name["ap_group_name"]."')");
                if($insert <= 0){
                        echo "<script>alert('".$sta["sta_mac"]." 加入 ".$group_name["ap_group_name"]." 组失败');location='sta_list_assc.php';</script>";
                }
            }
        }
    }
        echo "<script>alert('加入黑名单成功!');location='sta_list_assc.php';</script>";
    }
}

if(isset($_GET["sta_mac"])){
    $dbhelper = new DAL();
    $delete = $dbhelper->delete("delete from sta_list_assc where sta_mac = '".$_GET["sta_mac"]."'");
    if($delete > 0){
        echo "<script>alert('删除成功!');location='sta_list_assc.php';</script>";
    }
}

//查询操作
if(isset($_POST["query"])){
    if($_POST["query"] == 0){
        $sta_mac = $_POST["mac"];
        $sta_mac = preg_replace("/:|：| /","",$sta_mac);
        $selectSql="select * from sta_list_assc where sta_mac like '%".$sta_mac."%'";
        $countSql="select count(*) from sta_list_assc where sta_mac like '%".$sta_mac."%'";
        $page = new Page($selectSql,$_POST["pageNow"],$countSql);
        //$result = $page->getResult();
    }
    if($_POST["query"] == 1){
        $ap_mac = $_POST["mac"];
        $ap_mac = preg_replace("/:|：| /","",$ap_mac);
        $selectSql="select * from sta_list_assc where assc_ap_mac like '%".$ap_mac."%'";
        $countSql="select count(*) from sta_list_assc where assc_ap_mac like '%".$ap_mac."%'";
        $page = new Page($selectSql,$_POST["pageNow"],$countSql);
        //$result = $page->getResult();
    }
}else{
$selectSql='select * from sta_list_assc';
$countSql='select count(*) from sta_list_assc';
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
}
?>
</head>

<body>
<!--<div class="title">关联终端信息表</div>--><br />
<form name="sta_list_assc" id="sta_list_assc" method="post">
<div style="text-align: left; padding-left: 60px;">
查询方式:<select name="query" id="query" style="margin: 0 5px;">
    <option value="0">终端MAC地址</option>
    <option value="1">所属AP MAC</option>
</select>
<input type="text" id="mac" name="mac"/>
<input type="button" class="bt" onclick="add(this,'sta_list_assc.php')" value="确定" />
</div>
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table id="stripe" class="acinfo_table" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	onclick="select_all(this,'id[]')" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">用户MAC地址</td>
        <td class="tdHeader">所属AP MAC</td>
        <td class="tdHeader">所属AP IP</td>
        <td class="tdHeader">上线时间</td>
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
        $dbhelper = new DAL();
        $num = $dbhelper->getOne("select count(*) from sta_blacklist where sta_mac='".$sta_mac."'");
        if($num>0)
            echo "<font color='red'>".$mac."</font>";
        else
            echo $mac;
        ?>
        </td>
        <td class="tdContent">
        <?php 
         $assc_ap_mac = $rs["assc_ap_mac"];
         $mac = "";
                for($k = 0; $k<strlen($assc_ap_mac); $k++){
                    if($k != 0 && $k%2 == 0)
                        $mac = $mac.":".$assc_ap_mac[$k];
                    else
                        $mac = $mac.$assc_ap_mac[$k];
                }
            echo $mac;
        ?>
        </td>
        <td class="tdContent">
        <?php
        	$assc_ap_ip = $dbhelper->getOne("select ap_ip from ap_info where ap_mac='".$assc_ap_mac."'");
        	echo $assc_ap_ip;
        ?>
        </td>
        <td class="tdContent"><?php echo $rs["assc_time"];?>
        </td>
        <td class="tdContent">
        <a href="#" onclick="if(confirm('是否删除?'))location='sta_list_assc.php?sta_mac=<?php echo $sta_mac;?>';">删除</a>|
        <a href="#" onclick="window.open('../sta_snmp_info/sta_snmp_info.php?sta_mac=<?php echo $sta_mac;?>','','height=770, width=1000, top=60, left=150');">详细</a>
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
        $dbhelper = new DAL();
        $num = $dbhelper->getOne("select count(*) from sta_blacklist where sta_mac='".$sta_mac."'");
        if($num>0)
            echo "<font color='red'>".$mac."</font>";
        else
            echo $mac;
        ?>
        </td>
        <td class="tdContentF9">
        <?php 
         $assc_ap_mac = $rs["assc_ap_mac"];
         $mac = "";
                for($k = 0; $k<strlen($assc_ap_mac); $k++){
                    if($k != 0 && $k%2 == 0)
                        $mac = $mac.":".$assc_ap_mac[$k];
                    else
                        $mac = $mac.$assc_ap_mac[$k];
                }
            echo $mac;
        ?>
        </td>
        <td class="tdContentF9">
        <?php
        	$assc_ap_ip = $dbhelper->getOne("select ap_ip from ap_info where ap_mac='".$assc_ap_mac."'");
        	echo $assc_ap_ip;
        ?>
        </td>
        <td class="tdContentF9"><?php echo $rs["assc_time"];?>
        </td>
        <td class="tdContentF9">
        <a href="#" onclick="if(confirm('是否删除?'))location='sta_list_assc.php?sta_mac=<?php echo $sta_mac;?>';">删除</a>|
        <a href="#" onclick="window.open('../sta_snmp_info/sta_snmp_info.php?sta_mac=<?php echo $sta_mac;?>','','height=770, width=1000, top=60, left=150');">详细</a>
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
        <td align="left" width="25%">
        <input class="bt" type="button" onclick="javascript:window.location.reload(true);" value="刷新" />
        <input class="bt" type="button" onclick="add_to_black(this,'id[]','sta_list_assc.php?action=del')" value="删除" />
        <input class="bt" type="button" onclick="add_to_black(this,'id[]','sta_list_assc.php')" value="加入黑名单" />
        </td>
		<td align="right" width="75%"><?php $formId='sta_list_assc';include PATH.'db/pageTemplate.php';?></td>
	</tr>
</table>
</form>
</body>
</html>