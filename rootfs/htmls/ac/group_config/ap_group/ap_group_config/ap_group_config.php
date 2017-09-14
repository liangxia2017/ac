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
<title>AP组配置</title>
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
    width: 25%;
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
//查询操作
$selectSql='select * from ap_group order by id';
$countSql='select count(*) from ap_group';
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
?>

<?php
if(isset($_GET["file"]) && !empty($_GET["file"])){
    $c = "rm -f ".$_GET["file"];
    exec($c,$a,$in);
}
?>

<?php
if(isset($_GET["id"])|isset($_POST["id"])){
    if(isset($_GET["action"])){
	   $dbhelper = new DAL();	
	   $getRecord = $dbhelper->getRow("select * from ap_group where id =".$_GET['id']);
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
	   $delete = $dbhelper->delete("delete from ap_group where id in (".$group_id.")");
    if($delete>0){
		echo "<script>alert('删除成功');location='ap_group_config.php?r='+Math.random();</script>";
	   }
    }
}
?>

<script type="text/javascript"> 
$(document).ready(function(){
    $("#edit_id").val("<?php echo $getRecord->id;?>");
    $("#edit_ap_group_name").val("<?php echo $getRecord->ap_group_name;?>");
    $("#license").val("<?php echo $getRecord->max_ap;?>");
    $("#sta_blance_sw").val("<?php echo $getRecord->sta_blance_sw;?>");
    $("#edit_ap_group").validate({
			rules:{
				"edit_ap_group_name":{
					required:true
				}
			}
		});
});	
</script>


</head>
<body>
<!--<div class="title">AP组配置</div>-->
<form name="edit_ap_group" id="edit_ap_group" action="edit_ap_group.php" method="post">
    <input type="hidden" name="edit_id" id="edit_id" value=""/>
    <table cellpadding="0px" cellspacing="0px" width="50%">
    <tr>
        <td class="tdRegist">AP组名称:</td>
        <td align="left"><input type="text" name="edit_ap_group_name" id="edit_ap_group_name"/></td>
    </tr>
    <tr>
        <td class="tdRegist">license数:</td>
        <td align="left"><input type="license" name="license" id="license"/>(剩余<span style="color: red; padding: 1px;">
        <?php            
            $dbhelper = new DAL();
            $sum = $dbhelper->getOne("select sum(max_ap) from ap_group where ap_group_name != 'unknown'");
            $license_path = "/ac/config/license.bin";
            if(file_exists($license_path)){
                exec("/ac/sbin/read_block -l /ac/config/license.bin",$arr,$inter);
                if($inter > 0 && $inter < 254){
                    if($inter*16 - $sum > 0)
                        echo $inter*16 - $sum;
                    else
                        echo 0;
                }else{
                    if(16 - $sum > 0)
                        echo 16 - $sum;
                    else
                        echo 0;
                }
            }else{
                if(16 - $sum > 0)
                        echo 16 - $sum;
                    else
                        echo 0;
            }
        ?></span> 个)
        </td>
    </tr>
    <tr>
        <td class="tdRegist">组内负载均衡</td>
          <td align="left">
            <select name="sta_blance_sw" id="sta_blance_sw" onchange="sta_blance()">
                <option value="0">关闭</option>
                <option value="1">基于用户</option>
            </select>
          </td>
    </tr>
    <tr>
        <td colspan="2"><input class="bt" type="submit" value="注册" />&nbsp;&nbsp;<input class="bt" type="reset" value="重置"/></td>
    </tr>
</table>
</form>
<hr />
<form name="ap_group" id="ap_group" method="post">
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table class="acinfo_table" width="100%" id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	onclick="select_all(this,'id[]')" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">AP组名称</td>
        <td class="tdHeader">license数</td>
        <td class="tdHeader">ap总数</td>
        <td class="tdHeader">在线ap数</td>
        <td class="tdHeader">离线ap数</td>
        <td class="tdHeader">终端数</td>
        <td class="tdHeader">操作</td>
      </tr>
      <?php
			$result = $page->getResult();
			$i=1;
			foreach ($result as $rs){
				$rs = (array)$rs;
				if($i%2 == 0){
	   ?>
      <tr>
        <td class="tdContent">
            <?php if($rs["ap_group_name"]==="unknown"){
                ?>
            <input type="checkbox" disabled="disabled" />
            <?php }else{
                ?>
            <input type="checkbox" name="id[]" id="id" value="<?php echo $rs["id"];?>" />
            <?php 
            }
                ?>
		</td>
        <td class="tdContent"><?php echo $i;?>
        </td>
        <td class="tdContent"><a href="../ap_info/ap_info.php?ap_group_name=<?php echo $rs["ap_group_name"];?>"><?php echo $rs["ap_group_name"];?></a>
        </td>
        <?php
            $dbhelper = new DAL();
            /*
            $ap_all_sql = "select ap_mac from ap_info where ap_group_name='".$rs["ap_group_name"]."'";
            $ap_all_info = $dbhelper->getall($ap_all_sql);
            $ap_num = count($ap_all_info);
            */
            $ap_num = $dbhelper->getone("select count(*) from ap_info where ap_group_name='".$rs["ap_group_name"]."'");
            $sta_num = $dbhelper->getone("select sum(sta_num) from ap_info where ap_group_name='".$rs["ap_group_name"]."'");
            /*
            foreach($ap_all_info as $ap){
                $sta_num = $sta_num + $dbhelper->getone("select count(*) from sta_list_assc where assc_ap_mac='".$ap["ap_mac"]."'");
            }*/
            $ap_up_sql = "select count(*) from ap_info where ap_group_name='".$rs["ap_group_name"]."' and status = 1";
            $ap_up_num = $dbhelper->getone($ap_up_sql); 
        ?>
        <td class="tdContent"><?php echo $rs["max_ap"];?>
        </td>
        <td class="tdContent"><?php echo $ap_num;?>
        </td>
        <td class="tdContent"><?php echo $ap_up_num;?>
        </td>
        <td class="tdContent"><?php echo $ap_num-$ap_up_num;?>
        </td>
        <td class="tdContent"><?php echo $sta_num;?>
        </td>
        <td class="tdContent">
        <?php if($rs["ap_group_name"]==="unknown"){
                ?>
        <!--<a style="color: blue;">导出</a> &nbsp;|&nbsp;--><a style="color: blue;">修改</a> &nbsp;|&nbsp;<a style="color: blue;">删除</a>
        <?php }else{
                ?>
         <!--   <a href="#" onclick="this.href='export.php?ap_group_name=<?php echo $rs["ap_group_name"];?>'">导出</a> &nbsp;|&nbsp;
         -->   <a href="#" onclick="this.href='ap_group_config.php?id=<?php echo $rs["id"];?>&action=modify'">
				修改</a> &nbsp;|&nbsp;<a href="#"
					onclick="if(confirm('您确定删除？')) this.href='ap_group_config.php?id=<?php echo $rs["id"];?>'">删除</a>
            <?php 
            }
                ?>
        </td>
      </tr>
      <?php	 
      	}else{
          ?>
        <tr>
        <td class="tdContentF9">
            <?php if($rs["ap_group_name"]==="unknown"){
                ?>
            <input type="checkbox" disabled="disabled" />
            <?php }else{
                ?>
            <input type="checkbox" name="id[]" id="id" value="<?php echo $rs["id"];?>" />
            <?php 
            }
                ?>
		</td>
        <td class="tdContentF9"><?php echo $i;?>
        </td>
        <td class="tdContentF9"><a href="../ap_info/ap_info.php?ap_group_name=<?php echo $rs["ap_group_name"];?>"><?php echo $rs["ap_group_name"];?></a>
        </td>
        <?php
            $dbhelper = new DAL();
            /*
            $ap_all_sql = "select ap_mac from ap_info where ap_group_name='".$rs["ap_group_name"]."'";
            $ap_all_info = $dbhelper->getall($ap_all_sql);
            $ap_num = count($ap_all_info);
            */
            $ap_num = $dbhelper->getone("select count(*) from ap_info where ap_group_name='".$rs["ap_group_name"]."'");
            $sta_num = $dbhelper->getone("select sum(sta_num) from ap_info where ap_group_name='".$rs["ap_group_name"]."'");
            /*
            foreach($ap_all_info as $ap){
                $sta_num = $sta_num + $dbhelper->getone("select count(*) from sta_list_assc where assc_ap_mac='".$ap["ap_mac"]."'");
            }*/
            $ap_up_sql = "select count(*) from ap_info where ap_group_name='".$rs["ap_group_name"]."' and status = 1";
            $ap_up_num = $dbhelper->getone($ap_up_sql); 
        ?>
        <td class="tdContentF9"><?php echo $rs["max_ap"];?>
        </td>
        <td class="tdContentF9"><?php echo $ap_num;?>
        </td>
        <td class="tdContentF9"><?php echo $ap_up_num;?>
        </td>
        <td class="tdContentF9"><?php echo $ap_num-$ap_up_num;?>
        </td>
        <td class="tdContentF9"><?php echo $sta_num;?>
        </td>
        <td class="tdContentF9">
        <?php if($rs["ap_group_name"]==="unknown"){
                ?>
        <!--<a style="color: blue;">导出</a> &nbsp;|&nbsp;--><a style="color: blue;">修改</a> &nbsp;|&nbsp;<a style="color: blue;">删除</a>
        <?php }else{
                ?>
         <!--       <a href="#" onclick="this.href='export.php?ap_group_name=<?php echo $rs["ap_group_name"];?>'">导出</a> &nbsp;|&nbsp;
          -->  <a href="#" onclick="this.href='ap_group_config.php?id=<?php echo $rs["id"];?>&action=modify'">
				修改</a> &nbsp;|&nbsp;<a href="#"
					onclick="if(confirm('您确定删除？')) this.href='ap_group_config.php?id=<?php echo $rs["id"];?>'">删除</a>
            <?php 
            }
                ?>
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
		<td align="left">
                <input class="bt" type="button"	onclick="delete_all(this,'id[]','ap_group_config.php')" value="删除" />
                <input class="bt" type="button"	onclick="location='export.php'" value="导出AP列表" />
		</td>
		<td  align="right" width="85%"><?php $formId='ap_group';include PATH.'db/pageTemplate.php';?>				
		</td>
	</tr>
</table>
</form>
<!--
<div style="text-align: left; padding-left: 5%;">
    <form action="import.php" enctype="multipart/form-data" method="POST">
        导入AP:<input class="bt" type="file" name="import" value="浏览" />
        <input class="bt" type="submit" value="导入" />
    </form>
</div>
-->
</body>
</html>