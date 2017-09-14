<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";
?>
<head>
<META   HTTP-EQUIV="pragma"   CONTENT="no-cache">         
<META   HTTP-EQUIV="Cache-Control"   CONTENT="no-cache,   must-revalidate">         
<META   HTTP-EQUIV="expires"   CONTENT="0"> 
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script type="text/javascript" src="<?php echo PATH ?>js/jquery.js"></script>
<script src="<?php echo PATH ?>js/jquery.validate.js" type="text/javascript"></script>
<link rel="stylesheet" href="<?php echo PATH ?>css/body.css" type="text/css" />
<title>AC版本信息</title>
</head>

<style type="text/css">
<!--
.acinfo_table{
/*	margin: auto; */
	background:#a8c7ce;
	width:50%;
	border: 1px solid #ddeeff;
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

<?php
    if($_FILES["lic"]["error"] == 0){
        $file = $_FILES["lic"];
        if(move_uploaded_file($file["tmp_name"],"/tmp/license.bin")){
            exec("/ac/sbin/read_block -l /tmp/license.bin",$arr1,$inter);
            if($inter <= 0 || $inter >=255){
                echo "<script>alert('导入失败!');</script>";
                exec("rm -f /tmp/license.bin");
            }else{
                exec("mv -f /tmp/license.bin /ac/config/license.bin");
                echo "<script>alert('导入成功!');</script>";
            }
        }
    }
?>

<script type="text/javascript"> 
	$(document).ready(function(){
	   <?php
        $ac_sys_path = "/ac/config/ac_sysinfo.conf";
        $license_path = "/ac/config/license.bin";
        if(file_exists($ac_sys_path)){
            $ac_sys = file($ac_sys_path);
            foreach($ac_sys as $ac){
                if(trim($ac) != ""){
                    $ac_info = explode("=",str_replace(PHP_EOL,"",$ac));
                    echo "$('#".$ac_info[0]."').text('".$ac_info[1]."');";
                }
            }
        }
        if(file_exists($license_path)){
            exec("/ac/sbin/read_block -l /ac/config/license.bin",$arr3,$inter);
            if($inter > 0 && $inter < 255)
                echo "$('#license').text('".($inter*16)."');";
            else
                echo "$('#license').text('16');";
        }else
            echo "$('#license').text('16');";
        ?>
	});
</script>
<script type="text/javascript">
	$(document).ready(function(){
	$("#version_info tr").mouseover(function(){
	$(this).addClass("over");
	});
	$("#version_info tr").mouseout(function(){
	$(this).removeClass("over");
	});
	});
</script>



<body>
<!--<div class="title">AC版本信息</div>-->
<br />
<form enctype="multipart/form-data" action="ac_version_info.php" method="post">
	<table align="center" class="acinfo_table" cellpadding="0" cellspacing="1px" id="version_info">
      
	  <tr>
        <td class="tdContentF9">设备型号：</td>
        <td class="tdContentF9"><div id="AC_MODEL"></div></td>
      </tr>
	  <tr>
        <td class="tdContent">软件版本：</td>
        <td class="tdContent"><div id="SOFT_VER"></div></td>
      </tr>
	  <tr>
        <td class="tdContentF9">所属厂商：</td>
        <td class="tdContentF9"><div id="FACTORY"></div></td>
      </tr>
	  <tr>
        <td  class="tdContent">数据库版本：</td>
        <td class="tdContent"><div id="DB_VER"></div></td>
      </tr>
      <tr>
        <td  class="tdContentF9">序列号：</td>
        <td class="tdContentF9"><div id="SN"></div></td>
      </tr>
      <tr>
        <td  class="tdContent">license数(个)：</td>
        <td class="tdContent">
            <div style="float: left; padding-left: 10%;" id="license"></div>
            <input type="file" name="lic" id="lic" />
            <input type="submit" class="bt" value="提交" />
        </td>
      </tr>
</table>

</form>
</body>
</html>