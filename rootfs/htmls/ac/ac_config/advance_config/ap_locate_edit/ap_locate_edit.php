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
<title>定位功能</title>
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
    padding: 0 5px;
	text-align:right;
    color:#73938E;
    font-weight:bold;
    width: 70%;
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
    function back(){
        window.location.reload(true);
    }
    function add(t,action){
		t.form.action=action;
		t.form.submit();	
	}
</script>
<?php
$filePath = PATH."map/";//文件路径
if(isset($_GET["service"])){//点击应用
	$dbhelper = new DAL();
	$exsit = $dbhelper->getOne("select count(*) from ac_basic_conf");
	if($exsit > 0){
		$dbhelper->update("update ac_basic_conf set location_switch = ".$_POST["mode"]);
	}else{
		$dbhelper->insert("insert into ac_basic_conf(location_switch) values(".$_POST["mode"].")");
	}
    if($_POST["mode"] == 0){//关闭
        $cmd = "pkill -9 locating";
        exec($cmd,$arr,$ret);
        if($ret > 0){
            echo "<script>alert('操作失败!');location='ap_locate_edit.php?r='+Math.random();</script>";    
        }else
      		echo "<script>alert('操作成功!');location='ap_locate_edit.php?r='+Math.random();</script>";        
    }elseif($_POST["mode"] == 1){//定位
        $cmd = "pkill -9 locating;/ac/sbin/locating -B -p 9010 -m 1";
        exec($cmd,$arr,$ret);
        if($ret > 0){
            echo "<script>alert('操作失败!');location='ap_locate_edit.php?r='+Math.random();</script>";    
    }else
      		echo "<script>alert('操作成功!');location='ap_locate_edit.php?r='+Math.random();</script>";         
    }elseif($_POST["mode"] == 2){//采集
        $cmd = "pkill -9 locating;/ac/sbin/locating -B -p 9010 -m 0";
        exec($cmd,$arr,$ret);
        if($ret > 0){
            echo "<script>alert('操作失败!');location='ap_locate_edit.php?r='+Math.random();</script>";    
        }else
      		echo "<script>alert('操作成功!');location='ap_locate_edit.php?r='+Math.random();</script>";        
    }
}else if(isset($_GET["id"])|isset($_POST["id"])){//删除操作
	$dbhelper = new DAL();
	$group_id="0";	
	if(isset($_GET["id"])){
		$group_id=$group_id.",".$_GET["id"];
	}else{
		foreach ($_POST["id"] as $v){
			$group_id=$group_id.",".$v;
		}
	}
    $getall = $dbhelper->getall("select * from ap_locate_edit where id in (".$group_id.")");
    $dbhelper->update("update ap_info set ap_locate_area = '' where ap_locate_area in (select area_name from ap_locate_edit where id in (".$group_id."))");
    foreach($getall as $getone){
        $file = $filePath.$getone["map_path"];
        $con = $dbhelper->getOne("select count(*) from ap_locate_edit where map_path='".$getone["map_path"]."'");
        if(!file_exists($file) | $con > 1){
            $dbhelper->delete("delete from ap_locate_edit where id=(".$getone["id"].")");
            continue;
        }else
        if (unlink($file)){
            $delete = $dbhelper->delete("delete from ap_locate_edit where id=(".$getone["id"].")");
            if($delete <= 0){
                 echo "<script>alert('".$getone["map_path"]."删除失败！');location='ap_locate_edit.php?r='+Math.random();</script>";
            }
        } else{
             echo "<script>alert('".$getone["map_path"]."删除失败！');location='ap_locate_edit.php?r='+Math.random();</script>";
        }     
    }
    echo "<script>alert('删除成功！');location='ap_locate_edit.php?r='+Math.random();</script>";	

}else   if(isset($_POST["area_name_upgrade"])){
    $fileName = $_FILES["map_path_upgrade"];
    //var_dump($fileName);
    if(empty($fileName["name"])){
    	echo "<script>alert('请选择文件！');location='ap_locate_edit.php?r='+Math.random();</script>";
    }else{
//    if(file_exists($filePath.$fileName['name'])){
//    	echo "<script>alert('存在同名文件！');location='ap_locate_edit.php?r='+Math.random();</script>";
//    }else{
        $dbhelper = new DAL();
        $map = $dbhelper->getOne("select count(*) as count from ap_locate_edit where area_name='".$_POST["area_name_upgrade"]."'");

        if($map["count"]==0){
        if(move_uploaded_file($fileName["tmp_name"],$filePath.$fileName['name']))
        {
            $params = array($_POST["area_name_upgrade"],$fileName["name"],$_POST["map_x"],$_POST["map_y"]);
            $sql = "insert into ap_locate_edit values(null,?,?,?,?)";
	   	   $insert = $dbhelper->insert($sql,$params);
    	   if($insert>0){
    		  echo "<script>alert('上传成功！');location='ap_locate_edit.php?r='+Math.random();</script>";
    	   }
        }else{
            echo "<script>alert('上传失败！');location='ap_locate_edit.php?r='+Math.random();</script>";
        }
        }else{
            echo "<script>alert('区域名已存在！');location='ap_locate_edit.php?r='+Math.random();</script>";
        }
        
      }

}
//查询操作
$selectSql='select * from ap_locate_edit';
$countSql='select count(*) from ap_locate_edit';
$page = new Page($selectSql,$_POST["pageNow"],$countSql);

$dbhelper = new DAL();
$location_switch = $dbhelper->getOne("select location_switch from ac_basic_conf");

?>
</head>

<body>
<br />
<form enctype="multipart/form-data" action="ap_locate_edit.php" method="post">
<table cellpadding="0px" cellspacing="0px" width="35%">
    <tr>
        <td class="tdRegist">定位区域名:</td>
        <td align="left"><input type="text" name="area_name_upgrade" id="area_name_upgrade"/></td>
    </tr>
    <tr>
        <td class="tdRegist">地图名:</td>
        <td align="left"><input type="file" name="map_path_upgrade" id="map_path_upgrade"/></td>
    </tr>
    <tr>
        <td class="tdRegist">横向距离:</td>
        <td align="left"><input type="text" name="map_x" id="map_x" maxlength="3" size="5"/>(米)</td>
    </tr>
    <tr>
        <td class="tdRegist">纵向距离:</td>
        <td align="left"><input type="text" name="map_y" id="map_y" maxlength="3" size="5"/>(米)</td>
    </tr>
    <tr>
        <td colspan="2"><input class="bt" type="submit" value="导入" />&nbsp;&nbsp;<input class="bt" type="reset" value="重置"/></td>
    </tr>
</table>
</form>
<hr />
<form name="ap_locate_edit" id="ap_locate_edit" method="post">
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table class="acinfo_table"  id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	onclick="select_all(this,'id[]')" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">定位区域名</td>
        <td class="tdHeader">地图名</td>
        <td class="tdHeader" colspan="3">AP MAC及坐标</td>
        <td class="tdHeader">查看地图</td>
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
        <td class="tdContent"><?php echo $rs["area_name"];?>
        </td>
        <td class="tdContent"><?php echo $rs["map_path"];?>
        </td>
        <?php
            $dbhelper = new DAL();
            $ap_info = $dbhelper->getall("select * from ap_info where ap_locate_area = '".$rs["area_name"]."'");
            $len = count($ap_info);
            
            if($len > 0){
            foreach($ap_info as $ap){
                $ap_mac = $ap["ap_mac"];
                $mac = "";
                for($k = 0; $k<strlen($ap_mac); $k++){
                    if($k != 0 && $k%2 == 0)
                        $mac = $mac.":".$ap_mac[$k];
                    else
                        $mac = $mac.$ap_mac[$k];
                }
                echo "<td class=\"tdContent\"><a href=\"#\" onclick=\"window.open('locate_ap.php?map_path=".$rs["map_path"]."&area_name=".$rs["area_name"]."&ap_mac=".$ap["ap_mac"]."','','height=850, width=1200, top=10, left=10');\">".$mac."(".$ap["ap_x"].",".$ap["ap_y"].")</td>";
            }
            }
            for($k = 0;$k < 3 - $len; $k++){
                echo "<td class=\"tdContent\"><a href=\"#\" onclick=\"window.open('locate_ap.php?map_path=".$rs["map_path"]."&area_name=".$rs["area_name"]."','','height=850, width=1200, top=10, left=10');\">设置坐标</td>";
            
            }
        ?>
        <td class="tdContent"><a href="#" onclick="window.open('../../../sta_info/locate/show_map.php?area_name=<?php echo $rs["area_name"];?>&flag_ap=1','','height=850, width=1200, top=10, left=10');">查看地图</a>
        </td>
        <td class="tdContent">
        <a href="#" onclick="if(confirm('您确定删除？')) this.href='ap_locate_edit.php?id=<?php echo $rs["id"];?>'">删除</a>
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
        <td class="tdContentF9"><?php echo $rs["area_name"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["map_path"];?>
        </td>
        <?php
            $dbhelper = new DAL();
            $ap_info = $dbhelper->getall("select * from ap_info where ap_locate_area = '".$rs["area_name"]."'");
            $len = count($ap_info);
            
            if($len > 0){
            foreach($ap_info as $ap){
                $ap_mac = $ap["ap_mac"];
                $mac = "";
                for($k = 0; $k<strlen($ap_mac); $k++){
                    if($k != 0 && $k%2 == 0)
                        $mac = $mac.":".$ap_mac[$k];
                    else
                        $mac = $mac.$ap_mac[$k];
                    }
                echo "<td class=\"tdContentF9\"><a href=\"#\" onclick=\"window.open('locate_ap.php?map_path=".$rs["map_path"]."&area_name=".$rs["area_name"]."&ap_mac=".$ap["ap_mac"]."','','height=850, width=1200, top=10, left=10');\">".$mac."(".$ap["ap_x"].",".$ap["ap_y"].")</td>";
            }
            }
            for($k = 0;$k < 3 - $len; $k++){
                echo "<td class=\"tdContentF9\"><a href=\"#\" onclick=\"window.open('locate_ap.php?map_path=".$rs["map_path"]."&area_name=".$rs["area_name"]."','','height=850, width=1200, top=10, left=10');\">设置坐标</td>";
            
            }
        ?>
        <td class="tdContentF9"><a href="#" onclick="window.open('../../../sta_info/locate/show_map.php?area_name=<?php echo $rs["area_name"];?>&flag_ap=1','','height=850, width=1200, top=10, left=10');">查看地图</a>
        </td>
        <td class="tdContentF9">
        <a href="#" onclick="if(confirm('您确定删除？')) this.href='ap_locate_edit.php?id=<?php echo $rs["id"];?>'">删除</a>
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
                <input class="bt" type="button"	onclick="delete_all(this,'id[]','ap_locate_edit.php')" value="删除" />
                <select name="mode" id="mode" class="bt">
                    <option value="0">关闭</option>
                    <option value="1" selected="selected">定位</option>
                    <option value="2">采集</option>
                </select>
                <input class="bt" type="button"	onclick="add(this,'ap_locate_edit.php?service=1')" value="应用" />
            </div>
		</td>
		<td  align="right" width="70%"><?php $formId='ap_locate_edit';include PATH.'db/pageTemplate.php';?>				
		</td>
	</tr>
</table>

</form>
</body>
<script type="text/javascript">
    document.getElementById("mode").options[<?php echo $location_switch;?>].selected = true;
 </script>
</html>