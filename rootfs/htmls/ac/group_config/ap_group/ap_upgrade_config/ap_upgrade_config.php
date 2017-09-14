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
    /*width: 50%;*/
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
$filePath = "/ac/data/ap_img/";//文件路径
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
    $getall = $dbhelper->getall("select id,img_name from ap_upgrade_config where id in (".$group_id.")");
    foreach($getall as $getone){
        $file = $filePath.$getone["img_name"];
        if(!file_exists($file)){
            $dbhelper->delete("delete from ap_upgrade_config where id=(".$getone["id"].")");
            continue;
        }else
        if (unlink($file)){
            $delete = $dbhelper->delete("delete from ap_upgrade_config where id=(".$getone["id"].")");
            if($delete <= 0){
                 echo "<script>alert('".$getone["img_name"]."删除失败！');location='ap_upgrade_config.php?r='+Math.random();</script>";
            }
        } else{
             echo "<script>alert('".$getone["img_name"]."删除失败！');location='ap_upgrade_config.php?r='+Math.random();</script>";
        }     
    }
    echo "<script>alert('删除成功！');location='ap_upgrade_config.php?r='+Math.random();</script>";	

}else   if(isset($_POST["img_version_upgrade"])){
    $fileName = $_FILES["img_path_upgrade"];
//    var_dump($fileName);
    if(empty($fileName["name"])){
    	echo "<script>alert('请选择文件！');</script>";
    }else
    if(file_exists($filePath.$fileName['name'])){
    	echo "<script>alert('存在同名文件！');</script>";
    }else
    if($fileName["size"]>20*1024*1024 | $fileName["size"]<10*1024){
        echo "<script>alert('文件过大/过小，请重新选择！');</script>";
    }else{
        $dbhelper = new DAL();
        $img_version = $dbhelper->getOne("select count(*) as count from ap_upgrade_config where img_version='".$_POST["img_version_upgrade"]."'");
//        var_dump($img_version);
        if($img_version["count"]==0){
        if(move_uploaded_file($fileName["tmp_name"],$filePath.$fileName['name']))
        {
            $params = array(trim($_POST["img_version_upgrade"]),$fileName["name"]);
            $sql = "insert into ap_upgrade_config values(null,?,?)";
	   	   $insert = $dbhelper->insert($sql,$params);
    	   if($insert>0){
    		  echo "<script>alert('上传成功！');location='ap_upgrade_config.php?r='+Math.random();</script>";
    	   }
        }else{
            echo "<script>alert('上传失败！');</script>";
        }
        }else{
            echo "<script>alert('AP版本号已存在！');</script>";
        }
        
      }

}
//查询操作
$selectSql='select * from ap_upgrade_config';
$countSql='select count(*) from ap_upgrade_config';
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
?>
</head>

<body>
<br />
<form enctype="multipart/form-data" action="ap_upgrade_config.php" method="post">
<table cellpadding="0px" cellspacing="0px" width="50%">
    <tr>
        <td class="tdRegist">AP版本号:</td>
        <td align="left"><input type="text" name="img_version_upgrade" id="img_version_upgrade"/></td>
    </tr>
    <tr>
        <td class="tdRegist">AP版本:</td>
        <td align="left"><input type="file" name="img_path_upgrade" id="img_path_upgrade"/></td>
    </tr>
    <tr>
        <td colspan="2"><input class="bt" type="submit" value="导入" />&nbsp;&nbsp;<input class="bt" type="reset" value="重置"/></td>
    </tr>
</table>
</form>
<hr />
<form name="ap_upgrade_config" id="ap_upgrade_config" method="post">
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table class="acinfo_table"  id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	onclick="select_all(this,'id[]')" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">AP版本号</td>
        <td class="tdHeader">AP版本名称</td>
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
        <td class="tdContent"><?php echo $rs["img_version"];?>
        </td>
        <td class="tdContent"><?php echo $rs["img_name"];?>
        </td>
        <td class="tdContent">
        <a href="#" onclick="if(confirm('您确定删除？')) this.href='ap_upgrade_config.php?id=<?php echo $rs["id"];?>'">删除</a>
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
        <td class="tdContentF9"><?php echo $rs["img_version"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["img_name"];?>
        </td>
        <td class="tdContentF9">
        <a href="#" onclick="if(confirm('您确定删除？')) this.href='ap_upgrade_config.php?id=<?php echo $rs["id"];?>'">删除</a>
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
                <input class="bt" type="button"	onclick="delete_all(this,'id[]','ap_upgrade_config.php')" value="删除" />
            </div>
		</td>
		<td  align="right" width="85%"><?php $formId='ap_upgrade_config';include PATH.'db/pageTemplate.php';?>				
		</td>
	</tr>
</table>

</form>
</body>
</html>