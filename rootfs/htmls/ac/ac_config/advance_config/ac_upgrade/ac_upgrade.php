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
<title>AC升级</title>
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
    width: 20%;
}

.div{
    color:#73938E;
    font-weight:bold;
    padding-bottom: 10px;
    text-align: left;
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

</head>
<body>
<div class="div">系统升级：</div>
<form enctype="multipart/form-data" action="ac_upgrade.php?upgrade=upgrade" method="post">
<table cellpadding="0px" cellspacing="0px" width="50%">
    <tr>
        <td class="tdRegist">AC版本:</td>
        <td align="left"><input type="file" name="img_path_upgrade" id="img_path_upgrade"/></td>
    </tr>
    <tr>
        <td colspan="2" style="left: 0; text-align: left; padding-left: 10%">
        <input class="bt" type="submit" value="升级" />
        <label style="color: red;" id="upgrading"></label>
        </td>
    </tr>
</table>
</form>
<hr />
<div class="div">系统还原：</div>
<form name="ap_upgrade_config" id="ap_upgrade_config">
	<table align="center" width="90%">
      <tr>
          <td colspan="2">
            <table class="acinfo_table"  id="stripe" cellpadding="1px" cellspacing="1px">
                <tr>
                    <td class="tdHeader">序号</td>
                    <td class="tdHeader">AC版本号</td>
                    <td class="tdHeader">还原点日期</td>
                    <td class="tdHeader">操作</td>
                </tr>
            <?php
                exec("cd /opt/micro_ac && ls -d runtime_*",$arr,$interval);
                if($interval == 0){
                    for($i = 0; $i < count($arr); $i++){
                        $runtime = explode("_",$arr[$i]);
                        $ac_version = $runtime[2];
                        $time = $runtime[1];
            ?>
                <tr>
                    <td class="tdContent"><?php echo $i+1;?>
                    </td>
                    <td class="tdContent"><?php echo $ac_version;?>
                    </td>
                    <td class="tdContent"><?php echo $time;?>
                    </td>
                    <td class="tdContent">
                    <a href="#" onclick="this.href='ac_undo.php?undo=<?php echo $arr[$i];?>&action=restore'">还原</a>|
                    <a href="#" onclick="this.href='ac_undo.php?undo=<?php echo $arr[$i];?>&action=delete'">删除</a>
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
    <div style="text-align: left;"><input type="button" class="bt" onclick="location='ac_undo.php?action=backup'" value="建立还原点" /></div>
    </td>
    </tr>
</table>
</form>
</body>
<script type="text/javascript">
    var num = 90;    
function upgrading(){
    num--;
    if(num == 0){
        top.location = '../../../../logout.php';
    }
    var upg = document.getElementById("upgrading");
    var msg = "正在升级...(剩余"+num+"秒)";
    upg.innerHTML = msg;
}

var s=null;
function checking(){
    var upg = document.getElementById("upgrading");
    if(upg.innerHTML == ""){
        upg.innerHTML = "版本校验中";
    }else if(upg.innerHTML == "版本校验中"){
        upg.innerHTML = "版本校验中.";
    }else if(upg.innerHTML == "版本校验中."){
        upg.innerHTML = "版本校验中..";
    }else if(upg.innerHTML == "版本校验中.."){
        upg.innerHTML = "版本校验中...";
    }else if(upg.innerHTML == "版本校验中..."){
        upg.innerHTML = "版本校验中.";
    }
}
<?php
if(isset($_GET["upgrade"])){    
    echo "checking();var s = setInterval('checking()',1000);";
}
?>
</script>
<?php
$filePath = "/tmp/";//文件路径
if(isset($_FILES["img_path_upgrade"])){
    $fileName = $_FILES["img_path_upgrade"];
//    var_dump($fileName);
    if(empty($fileName["name"])){
    	echo "<script>clearInterval(s);alert('请选择文件！');</script>";
    }else{
        if(move_uploaded_file($fileName["tmp_name"],$filePath.'tmp.img'))
        {
    		exec("/ac/sbin/read_block -c /tmp/tmp.img ",$ac_arr,$ac_interval);
            if($ac_interval == 255){
                echo "<script>clearInterval(s);alert('升级失败！版本不合法！');location='ac_upgrade.php';</script>";
            }else{
                exec("echo 'IMG_NAME=tmp.img' > /opt/micro_ac/tmp/system_upg.conf",$up_arr,$up_int);
                echo "<script>clearInterval(s);setInterval('upgrading()',1000);</script>";
            }
        }else{
            echo "<script>clearInterval(s);alert('升级失败！');location='ac_upgrade.php';</script>";
        }              
      }
}
?>
</html>