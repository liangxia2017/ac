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
<script src="<?php echo PATH ?>js/checkbox.js" type="text/javascript"></script>
<link rel="stylesheet" href="<?php echo PATH ?>css/body.css" type="text/css" />
<title>定位AP</title>
</head>

<style type="text/css">
#set{
    border:1px solid #369;
    background:#e2ecf5;
    width: 240px;
    height: 160px;
    z-index:1000;
    position:absolute;
    display:none;
}
#set h4 {
    height:20px;
    background:#369;
    color:#fff;
}

#close {
    margin-left:120px;
    font-weight:500;
    cursor:pointer;
}

.set_table tr td{
    padding:10px 0 0 15px;
}

</style>

<?php

if(isset($_POST["ap_mac"]) && $_POST["ap_mac"] != ""){
    $coor = $_POST["coor_hidden"];
    // 坐标
    list($x,$y) = explode(";",$coor);
    //区域名
    $area_name = $_POST["area_name"];
    //MAC
   // $mac = "x'".trim(str_ireplace(":","",$_POST["ap_mac"]))."'";
    $mac = trim(str_ireplace(":","",$_POST["ap_mac"]))."";
   // echo $x."<br />".$y."<br />".$area_name."<br />".$mac."<br />";
    
    $dbhelper = new DAL();
    //判断设置的AP是否存在
    $count = $dbhelper->getRow("select * from ap_info where ap_mac = '".$mac."'");
    //var_dump($count);
    if($count == null | $count == ""){
        echo "<script>alert('所设置的MAC ".$_POST["ap_mac"]." 不存在！');</script>";
    }else if(isset($_POST["mac_old"]) && $_POST["mac_old"] != ""){
        if(strcasecmp($_POST["mac_old"],$_POST["ap_mac"]) == 0){
            //修改坐标
            //$params = array($x, $y, $mac);
            //$sql = "update ap_info set ap_x=?,ap_y=? where ap_mac=?";
            //$update = $dbhelper->update($sql,$params);
            $sql = "update ap_info set ap_x=".$x.",ap_y=".$y." where ap_mac = '".$mac."'";
            $update = $dbhelper->update($sql);
            if($update>0)
                echo "<script>alert('设置成功');window.opener.back();window.opener=null;window.open('','_self');window.close();</script>";
            else
                echo "<script>alert('设置失败');</script>";        
        }else{
            //修改mac和坐标
        
            //$mac_old = "x'".trim(str_ireplace(":","",$_POST["mac_old"]))."'";
            $mac_old = trim(str_ireplace(":","",$_POST["mac_old"]))."";
            $update = $dbhelper->update("update ap_info set ap_locate_area=null,ap_x=null,ap_y=null where ap_mac = '".$mac_old."'");
            if($update>0){
                //$params = array($area_name, $x, $y, $mac);
                //$sql = "update ap_info set ap_locate_area='?',ap_x=?,ap_y=? where ap_mac = ?";
                $sql = "update ap_info set ap_locate_area='".$area_name."',ap_x=".$x.",ap_y=".$y." where ap_mac = '".$mac."'";
           
                $update = $dbhelper->update($sql);
                //$update = $dbhelper->update($sql,$params);
                if($update>0)
                    echo "<script>alert('设置成功');window.opener.back();window.opener=null;window.open('','_self');window.close();</script>";
                else
                    echo "<script>alert('设置失败');</script>";
            }
            else
                echo "<script>alert('设置失败');</script>";
            }
        }else{
            $params = array($area_name, $x, $y, $mac);
            $sql = "update ap_info set ap_locate_area=?,ap_x=?,ap_y=? where ap_mac = ?";
            //$sql = "update ap_info set ap_locate_area='".$area_name."',ap_x=".$x.",ap_y=".$y." where ap_mac = '".$mac."'";
           //$update = $dbhelper->update($sql);
            $update = $dbhelper->update($sql,$params);
            if($update>0)
                echo "<script>alert('设置成功');window.opener.back();window.opener=null;window.open('','_self');window.close();</script>";
            else
                echo "<script>alert('设置失败');</script>";
        }
      
}

$filePath = PATH."map/";
$file = $filePath.$_GET["map_path"];
list($width, $height) = getimagesize($file);
//$ap_mac = $_GET["ap_mac"];
?>


<script type="text/javascript">
window.onload=function(){
    
    $("#area_name").val("<?php echo $_GET["area_name"]?>");
    
	var img = document.getElementById("img");
	var div = document.getElementById("div");
    var td_img = document.getElementById("td_img");
    
    var set = document.getElementById("set");
    var ap_mac = document.getElementById("ap_mac");
    var coor_hidden = document.getElementById("coor_hidden");
    var coor_label = document.getElementById("coor_label");
    var mac_old = document.getElementById("mac_old");
    
    var mClose = document.getElementById("close");
    
    //图片长宽
    var height = <?php echo $height;?>;
    var width = <?php echo $width;?>;
    
    if( height/width >= 3/4){
        img.height = "768";
    }else{
        img.width = "1024";
    }
    
//获取标签左边距    
    function getLeft(left,obj){
        for(;obj.offsetParent != null;){
            left = left + obj.offsetLeft;
            obj = obj.offsetParent;
        }
        return left;
    }
    
//获取标签上边距    
    function getTop(top,obj){
        for(;obj.offsetParent != null;){
            top = top + obj.offsetTop;
            obj = obj.offsetParent;
        }
        return top;
    }
    
//单元格大小
    cell_x = parseInt(td_img.width);
    cell_y = parseInt(td_img.height);
//中间坐标位置
    var coord_x = getLeft(0,td_img) + cell_x/2;
    var coord_y = getTop(0,td_img) + cell_y/2;

	img.onmousemove = function(ev){
        ev = ev || window.event;
        //鼠标相对图片位置（参考点：图片左上角）
        //var x_corner = ev.clientX-getLeft(0,img)-document.body.clientLeft;
        //var y_corner = ev.clientY-getTop(0,img)-document.body.clientTop;
        //鼠标相对图片位置（参考点：图片中心）
        var x_center = ev.clientX - coord_x - document.body.clientLeft;
        var y_Center = coord_y - ev.clientY - document.body.clientLeft;
        
        div.style.display = "";
        div.style.left = ev.clientX+15+"px";
        div.style.top = ev.clientY-5+"px";
        div.innerHTML = x_center+";"+y_Center;
    }
   
    img.onmouseout = function(){
        div.style.display = "none";
    }
    
    img.onclick = function(){
        coor_hidden.value = div.innerHTML;
        coor_label.value = div.innerHTML;
        <?php 
            if(isset($_GET["ap_mac"])){
                $ap_mac = $_GET["ap_mac"];
                $mac = "";
                for($k = 0; $k<strlen($ap_mac); $k++){
                    if($k != 0 && $k%2 == 0)
                        $mac = $mac.":".$ap_mac[$k];
                    else
                        $mac = $mac.$ap_mac[$k];
                }
                echo "ap_mac.value = '".$mac."';";
                echo "mac_old.value = '".$mac."';";
                }
        ?>
        set.style.display = "block";
        set.style.position = "absolute";
        set.style.left = "40%";
        set.style.top = "40%";
        mybg.style.display = "";     
    }
    
    mClose.onclick = function(){
        set.style.display = "none";
        mybg.style.display = "none";
    }

}
</script>


<?php

?>
</head>

<body style="overflow: hidden;">
<div id="mybg" style="background: #000; width:100%; height:100%; position:absolute; top:0; left:0; opacity: 0.3; filter:Alpha(opacity=30); display: none; " >
</div>
<div id="div" style="position: absolute; display: none"></div>
<table align="center" valign="middle" cellpadding="0px" cellspacing="0px">
    <tr>
        <td align="center" valign="middle" style="background-color: #CECECE;" width="1060px" height="800px" id="td_img">
            <img src="<?php echo $file;?>" id="img" />
        </td>
    </tr>
</table>

<div id="set">
<h4>设置坐标<span id="close">关闭</span></h4>
<form  method="post">
<table align="center" valign="middle" cellpadding="0px" cellspacing="0px" class="set_table">
    <input type="hidden" id="mac_old" name="mac_old" />
    <input type="hidden" id="area_name" name="area_name" />
    <input type="hidden" id="coor_hidden" name="coor_hidden" />
    <tr>
        <td align="right" width="20%">MAC</td>
        <td align="left"><input type="text" id="ap_mac" name="ap_mac" /></td>
    </tr>
    <tr>
        <td align="right">坐标</td>
        <td align="left"><input type="text" id="coor_label" name="coor_label" disabled="disabled" /></td>
    </tr>
    <tr>
        <td colspan="2" align="center" ><input type="submit" value="确定" class="bt" /></td>
    </tr>
</table>
</form>
</div>

</body>
</html>