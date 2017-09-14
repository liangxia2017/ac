<?php
include "/ac/htmls/db/dbhelper.php";
$dbhelper = new DAL();
$mac = $_GET["sta_mac"];
$area_name = $_GET["area_name"];
if($area_name == "" | $area_name == null){
    echo "<script>location='locating.php';</script>";
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
	<meta  http-equiv="pragma" content="no-cache" />
	<meta  http-equiv="Cache-Control" content="no-cache,must-revalidate" />
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
	<meta  http-equiv="expires" content="0" />
<title>查看地图</title>
</head>
<style type="text/css">
.css {
    background: #46FA03;
    position: absolute;
    display: none;
    background: transparent;
}
</style>
<?php
$map = $dbhelper->getOne("select map_path from ap_locate_edit where area_name='".$area_name."'");
if($map == "" | $map == null){
    echo "<script>location='locating.php';</script>";
}
//$filePath = "../web_images/";
$filePath = "/map/";
$file = $filePath.$map;
//图片长宽
list($width, $height) = getimagesize("/ac/htmls/map/".$map);
?>

<script type="text/javascript">
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

window.onload=function(){    
	var img = document.getElementById("img");
    var td_img = document.getElementById("td_img");
    
    var set = document.getElementById("set");    
    var mClose = document.getElementById("close");
    
    var ap0 = document.getElementById("ap0");
    var mac0 = document.getElementById("mac0");
    var ap1 = document.getElementById("ap1");
    var mac1 = document.getElementById("mac1");
    var ap2 = document.getElementById("ap2");
    var mac2 = document.getElementById("mac2");
        
    //终端
    var sta = document.getElementById("sta");
    var sta_mac = document.getElementById("sta_mac");
    
    var sta_all = document.getElementById("sta_all"); 
    
    //图片长宽
    var width = <?php echo $width;?>;
    var height = <?php echo $height;?>;
    
    if( height/width >= 3/4){
        img.height = "768";
    }else{
        img.width = "1024";
    }
  
//单元格大小
    cell_x = parseInt(td_img.width);
    cell_y = parseInt(td_img.height);
//中间坐标位置
    var coord_x = getLeft(0,td_img) + cell_x/2;
    var coord_y = getTop(0,td_img) + cell_y/2;
}
</script>

</head>
<body style="max-width:100%; ">
<form name="sta">
<input type="hidden" name="sta_x" id="sta_x" value="" />
<input type="hidden" name="sta_y" id="sta_y" value="" />
<input type="hidden" name="sta_mac" id="sta_mac" />
<table align="center" valign="middle" cellpadding="0px" cellspacing="0px">
    <tr>
        <td align="center" valign="middle" style="background-color: #CECECE;" width="1020px" height="800px" id="td_img">
            <img src="<?php echo $file;?>" id="img" />
        </td>
    </tr> 
</table>
<div id="sta_all"></div>
</form>
<iframe name="action" src="action.php?sta_mac=<?php echo $mac;?>" style="width: 0; height: 0; border: 0;"></iframe>
</body>
<script type="text/javascript">
  function refresh_sta(){
	window.action.location = "action.php?sta_mac=<?php echo $mac;?>";
    var td_img = document.getElementById("td_img");
//单元格大小
    cell_x = parseInt(td_img.width);
    cell_y = parseInt(td_img.height);
//中间坐标位置
    var coord_x = getLeft(0,td_img) + cell_x/2;
    var coord_y = getTop(0,td_img) + cell_y/2;    
    var sta_x = parseInt(document.getElementById("sta_x").value);
    var sta_y = parseInt(document.getElementById("sta_y").value);
    var sta_mac = document.getElementById("sta_mac").value;
    var div = document.createElement('div');
    var img = document.createElement('img');
    var x = sta_x + coord_x + document.body.clientLeft;
    var y = coord_y - document.body.clientTop - sta_y;
    div.className = 'css';            
    div.style.left= x - 40 + 'px';
    div.style.top= y - 40 + 'px';
    div.style.display= 'block';
    div.style.zIndex= '2200';
    img.src='../web_images/sta1.png';
    img.width='80';
    img.height='80';
    img.title = sta_mac;
    
    div.appendChild(img);
    sta_all.appendChild(div);
    while(sta_all.children.length > 1){
        sta_all.removeChild(sta_all.children[0]);
    }
  }
  setInterval("refresh_sta()",2000);
  
  function blink(){
    var is = document.getElementsByTagName("img");
    if(is.length >1){
    var img_blink = is[is.length-1];
    is[is.length-2].style.color == "";
    is[is.length-2].border == 0;
    img_blink.style.color = "red";        
    if(img_blink.border == 0){
        img_blink.border = '1';
    }else{
        img_blink.border = 0;
    }
    }
    }
  
</script>
</html>