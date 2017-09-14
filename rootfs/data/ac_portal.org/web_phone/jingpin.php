<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<?php
include "db/dbhelper.php";
$dbhelper = new DAL();
?>
<html>
 <head>
  <title> 精品 </title>
	<meta  http-equiv="pragma" content="no-cache" />
	<meta  http-equiv="Cache-Control" content="no-cache,must-revalidate" />
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
	<meta  http-equiv="expires" content="0" />
    <script src="db/jquery.js" type="text/javascript"></script>
 </head>
 <style type="text/css">
 *{
    font-family: monospace;
    font-size: 14px;
    font: normal 100%;
}
img{
    border: 0;
}
.head{
    position: relative;
    top: -20px;
    text-align: center;
 }
 .head img{
    vertical-align: middle;
    margin: 2% 4% 0 4%;
 }
.div_img{
    position: relative;
    top: -18px;
    text-align: center;
}
.show_img{
    width: 100%;
}

 </style>

 <?php
    $jingpins = $dbhelper->getall("select * from jingpin limit 8");
 ?>
 <body style="max-width:100%; margin:0 auto; ">
 <?php
$filePath = "count.txt";
$file = fopen($filePath,"r");
$count = fread($file,filesize($filePath));
fclose($file);
$count ++;
$file = fopen($filePath,"w");
fwrite($file,$count);
fclose($file);
?>
 <div style="position: fixed; top: 0; z-index: 9999; color: #5C5C5C; font-size: 12px;">浏览次数:<?php echo $count;?></div>
 <input type="hidden" id="num" value="<?php echo count($jingpins);?>" />
 <?php
    for($i = 1 ;$i <= count($jingpins); $i++){
        if($i == 1){
            echo "<div id='div".$i."' style='display: ;'><div class='div_img'>";
        }else{
            echo "<div id='div".$i."' style='display: none;'><div class='div_img'>";
        }        
        echo "<img src='jingpin/".$jingpins[$i-1]["path"]."' class='show_img' usemap='#map".$i."' />";
        echo "<map name='map".$i."'>";
        echo "<area shape='rect' onclick=\"tab(".$i.",'left')\" />";
        echo "<area shape='rect' onclick=\"tab(".$i.",'right')\" />";
        echo "</map></div>";
        echo "<div style='margin: 0 2%; position: relative; top: -30px;'>";
        echo "<div style='width: 50%; padding: 0 25%;'>";
        echo "<ul style='list-style: disc;'>";
        for($j = 1 ;$j <= count($jingpins); $j++){
            if($j == $i){
                echo "<li style='color: balck; float: left; margin: 0 ".floor(50/count($jingpins))."%'></li>";
            }else{
                echo "<li style=' color: gray; float: left; margin: 0 ".floor(50/count($jingpins))."%'></li>";
            }
        }
        echo "</ul></div>";
        echo "</div>";
        echo "<div style='position: relative; top: -20px;'><hr style='color: silver; width: 100%;' /><div style='margin: 0 2%; '>";
        echo "<p style='font-weight: bold; height: 10px; font-size: 18px;'>".$jingpins[$i-1]["keys"]."</p>";
        echo "<p style='height: 70px;'>".$jingpins[$i-1]["intro"]."</p>";
        echo "<p style='height: 10px;'>店铺:".$jingpins[$i-1]["shop"]."</p>";
        echo "<p style='height: 10px;'>电话:".$jingpins[$i-1]["tel"]."</p>";
        echo "</div></div></div>";
    }
 ?>
 </body>
 <script type="text/javascript">
var number = document.getElementById("num").value;
function tab(n,r){
    if(r == 'left'){
        for(var j =1 ; j <= number ; j++){
            var id = "div"+j;
            var div = document.getElementById(id);
            div.style.display = "none";
        }
        if( n - 1 == 0){
            var show = "div"+number;
            var div_show = document.getElementById(show);
            div_show.style.display = "";
        }else{
            var show = "div"+(n-1);
            var div_show = document.getElementById(show);
            div_show.style.display = "";
        }
    }else{
        for(var j = 1 ; j <= number ; j++){
            var id = "div"+j;
            var div = document.getElementById(id);
            div.style.display = "none";
        }
        if(n == number){
            var show = "div"+1;
            var div_show = document.getElementById(show);
            div_show.style.display = "";
        }else{
            var show = "div"+(n+1);
            var div_show = document.getElementById(show);
            div_show.style.display = "";
        }
    }
}


    function getstyle(sname){
        var rules;
        if(document.styleSheets[0].cssRules){
            rules = document.styleSheets[0].cssRules;
        }else{
            rules = document.styleSheets[0].rules;
        }
        for(var j=0;j<rules.length;j++){
            if(rules[j].selectorText == sname){
                return rules[j].style; 
            }
        }
    }
    
    var screen = $(window).width();
    
    var img_width = Math.floor(screen);
    var img_height = Math.floor(screen*0.7);
    getstyle('.show_img').height = img_height + 'px';
    var areas = document.getElementsByTagName("area");
    for(var j = 0; j < areas.length; j++){
        if(j % 2 == 0){
            areas[j].coords = "0,0," + Math.floor(img_width/2) + "," + img_height;
        }else{
            areas[j].coords = Math.floor(img_width/2) + ",0," + img_width + "," + img_height;
        }
    }
   
   
   
    var tmp1=0;   
    var tmp2=0; 
    function touchSatrtFunc(evt) {
        try
        {
            evt.preventDefault(); //阻止触摸时浏览器的缩放、滚动条滚动等
            var touch = evt.touches[0]; //获取第一个触点
            var x = Number(touch.pageX); //页面触点X坐标
            //记录触点初始位置
            tmp1 = x;
        }
        catch (e) {
            alert('touchSatrtFunc：' + e.message);
        }
    }


    function touchMoveFunc(evt) {
        try
        {
            evt.preventDefault();
            var touch = evt.touches[0];
            var x = Number(touch.pageX);
            tmp2 = x;
        }
        catch (e) {
            alert('touchMoveFunc：' + e.message);
        }
    }
    
    function touchEndFunc(evt) {
        try
        {
            evt.preventDefault();
            if (tmp2 - tmp1 > 50) {
                slide("left");
            }
            if (tmp1 - tmp2 > 50) {
                slide("right");
            }
        }
        catch (e) {
            alert('touchEndFunc：' + e.message);
        }
    }       
    
     //绑定事件
    function bindEvent() {
        document.addEventListener('touchstart', touchSatrtFunc, false);
        document.addEventListener('touchmove', touchMoveFunc, false);
        document.addEventListener('touchend', touchEndFunc, false);
    }
    
    function slide(r){
        for(var k =1 ; k <= number ; k++){
            var id = "div"+k;
            var div = document.getElementById(id);
            if(div.style.display == ""){
                tab(k,r);
                break;
            }
        }
    }
    
    window.onload = bindEvent;
    
 </script>
 
</html>