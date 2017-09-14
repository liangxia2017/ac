<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
define("PATH", "../../../");
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
<title>查看地图</title>
</head>
<style type="text/css">
.css {
    background: #46FA03;
    /***半径****/
/*  border-radius: 8px;
    padding: 8px;*/
    /**********/
    position: absolute;
    display: none;
    background: transparent;
}
</style>
<?php
$dbhelper = new DAL();
$map = $dbhelper->getOne("select map_path from ap_locate_edit where area_name='".$_GET["area_name"]."'");
$filePath = PATH."map/";
$file = $filePath.$map;
//图片长宽
list($width, $height) = getimagesize($file);
$sql = "select ap_mac,ap_x,ap_y from ap_info where ap_locate_area in (select area_name from ap_locate_edit where map_path='".$map."')";
$ap_info = $dbhelper->getall($sql);
if(isset($_GET["flag"])){
    $flag = 1;
}else
    $flag = 0;
?>


<script type="text/javascript">
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
    
    var ap_all = document.getElementById("ap_all");
    var sta_all = document.getElementById("sta_all"); 
    
    //图片长宽
    var width = <?php echo $width;?>;
    var height = <?php echo $height;?>;
    
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


//ap坐标(相对浏览器；减掉圆形半径8)
    <?php
    if($ap_info != null){
        for($i=0; $i<count($ap_info); $i++){
            $ap_mac = $ap_info[$i]["ap_mac"];
                $mac = "";
                for($k = 0; $k<strlen($ap_mac); $k++){
                    if($k != 0 && $k%2 == 0)
                        $mac = $mac.":".$ap_mac[$k];
                    else
                        $mac = $mac.$ap_mac[$k];
                }
            /****************
            echo "var x".$i."= ".$ap_info[$i]["ap_x"]." + coord_x + document.body.clientLeft;";
            echo "var y".$i."= coord_y - document.body.clientTop - ".$ap_info[$i]["ap_y"].";";            
            echo "ap".$i.".style.left= x".$i." - 8 + 'px';";
            echo "ap".$i.".style.top= y".$i." - 8 + 'px';";
            echo "ap".$i.".style.display= 'block';";
            echo "$('#ap".$i." img').attr('title','".$mac."');";
            echo "mac".$i.".innerHTML= '(".$ap_info[$i]["ap_x"].",".$ap_info[$i]["ap_y"].")';";
            echo "mac".$i.".style.left= x".$i."- 23 + 'px';";
            echo "mac".$i.".style.top= y".$i."- 28 + 'px';";
            **************/
            
            echo "var div".$i." = document.createElement('div');";
            echo "var img = document.createElement('img');";
            echo "var div_mac".$i." = document.createElement('div');";
            echo "var x".$i." = ".$ap_info[$i]["ap_x"]." +  coord_x + document.body.clientLeft;";
            echo "var y".$i." = coord_y - document.body.clientTop - ".$ap_info[$i]["ap_y"].";";
            echo "div".$i.".className = 'css';";            
            echo "div".$i.".style.left= x".$i." - 8 + 'px';";
            echo "div".$i.".style.top= y".$i." - 8 + 'px';";
            echo "div".$i.".style.display= 'block';";
            //echo "div".$i.".style.zIndex= '".$i."*100 + 2000';";
            echo "img.src='".PATH."images/wireless.png';";
            echo "img.width='16';";
            echo "img.height='16';";
            echo "img.title = '".$mac."';";
            echo "div_mac".$i.".innerHTML= '(".$ap_info[$i]["ap_x"].",".$ap_info[$i]["ap_y"].")';";
            echo "div_mac".$i.".style.left= x".$i."- 23 + 'px';";
            echo "div_mac".$i.".style.top= y".$i."- 28 + 'px';"; 
            echo "div_mac".$i.".style.zIndex= '".$i."*100 + 2150';";
            echo "div_mac".$i.".style.position= 'absolute';";
            echo "div".$i.".appendChild(img);";
            echo "ap_all.appendChild(div_mac".$i.");";
            echo "ap_all.appendChild(div".$i.");";
                
        }
    }    
/*    if(isset($_GET["flag_ap"])){
    define("FLAG","flag");
    $dbhelper1 = new DAL();
    $sta_info = $dbhelper1->getall("select sta_mac,sta_x,sta_y from file_data where area_name='".$_GET["area_name"]."'");
    for($j = 0;$j<count($sta_info); $j++){
        $sta = $sta_info[$j];
        $sta_mac = $sta["sta_mac"];
        $mac = "";
        for($k = 0; $k<strlen($sta_mac); $k++){
            if($k != 0 && $k%2 == 0)
                $mac = $mac.":".$sta_mac[$k];
            else
                $mac = $mac.$sta_mac[$k];
        }
        echo "var div".$j." = document.createElement('div');";
        echo "var img = document.createElement('img');";
        echo "var div_mac".$j." = document.createElement('div');";
        echo "var x".$j." = ".$sta["sta_x"]." +  coord_x + document.body.clientLeft;";
        echo "var y".$j." = coord_y - document.body.clientTop - ".$sta["sta_y"].";";
        echo "div".$j.".className = 'css';";            
        echo "div".$j.".style.left= x".$j." - 8 + 'px';";
        echo "div".$j.".style.top= y".$j." - 8 + 'px';";
        echo "div".$j.".style.display= 'block';";
        echo "div".$j.".style.zIndex= '".$j."*100 + 2100';";
        echo "img.src='".PATH."images/sta.png';";
        echo "img.width='16';";
        echo "img.height='18';";
        echo "img.title = '".$mac."';";
        echo "div_mac".$j.".innerHTML= '(".$sta["sta_x"].",".$sta["sta_y"].")';";
        echo "div_mac".$j.".style.left= x".$j."- 23 + 'px';";
        echo "div_mac".$j.".style.top= y".$j."- 28 + 'px';";      
        echo "div".$j.".appendChild(div_mac".$j.");";
        echo "div".$j.".appendChild(img);";
        echo "sta_all.appendChild(div".$j.");";
                
            }
    }
  */  
    ?>
    document.getElementById("show_map_action").src="show_map_action.php?area_name=<?php echo $_GET["area_name"];?>&coord_x="+coord_x+"&coord_y="+coord_y;
}
</script>

</head>

<body style="overflow: hidden;">
<table align="center" valign="middle" cellpadding="0px" cellspacing="0px">
    <tr>
        <td align="center" valign="middle" style="background-color: #CECECE;" width="1060px" height="800px" id="td_img">
            <img src="<?php echo $file;?>" id="img" />
        </td>
    </tr>
</table>
<!--
<div class="css" id="ap0"><img src="<?php echo PATH ?>images/wireless.png" width="16"/></div><div style="position: absolute;" id="mac0"></div>
<div class="css" id="ap1"><img src="<?php echo PATH ?>images/wireless.png" width="16"/></div><div style="position: absolute;" id="mac1"></div>
<div class="css" id="ap2"><img src="<?php echo PATH ?>images/wireless.png" width="16"/></div><div style="position: absolute;" id="mac2"></div>
-->
<div id="ap_all"></div>
<div id="sta_all"></div>
<iframe name="show_map_action" id="show_map_action" style="width: 0; height: 0; border: 0;"></iframe>
</body>
<script>
function refre(){
	window.show_map_action.location = window.show_map_action.location;
    }
setInterval("refre()",5000);
</script>
</html>