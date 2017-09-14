<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<?php
include "db/dbhelper.php";
$dbhelper = new DAL();
define("LIMIT",60);//展示数目
define("NUM",120);//楼层基数
?>
<html>
 <head>
  <title> 留言 </title>
	<meta  http-equiv="pragma" content="no-cache" />
	<meta  http-equiv="Cache-Control" content="no-cache,must-revalidate" />
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
	<meta  http-equiv="expires" content="0" />
    <script src="../web_phone/db/jquery.js" type="text/javascript"></script>
 </head>
 
 <?php
 if(isset($_POST["liuyan_area"]) && $_POST["liuyan_area"] != "" && $_POST["liuyan_area"] != null){
    $params = array($_POST["liuyan_area"],date("Y年m月d日 H:i"));
    $sql = "insert into liuyan(msg,msg_time) values (?,?)";
    $insert = $dbhelper->insert($sql,$params);
    if($insert > 0){
        echo "<script>location='liuyanban.php?r='+Math.random();</script>"; 
    }
 }
 ?>
 
 <?php
    $result = $dbhelper->getall('select * from liuyan where id != 0 order by msg_time desc,id desc limit '.LIMIT);
 ?>
 
 <style type="text/css">
*{
    font-family: monospace;
    font-size: 18px;
/*    word-break: keep-words;*/
    word-wrap: break-word;
}
a{
    color: black;
    text-decoration: none;
}
img{
    border: 0;
}
 body{
    background-color: #EEEEEE;
 }
 .msg{
    z-index: 100;
    position: relative;
    background-color: #FFFFFF;
/*    margin: 1px 0 12px;*/
    margin-bottom: 12px;
    padding: 11px 7px 0 9px;
    border-top: 1px solid #CECECE;
    border-bottom: 1px solid #CECECE;
 }
p{
    margin: 0;
 }
 .floor{
    position: relative;
    color: #6A6A6A;
    letter-spacing: 0;
    font-size: 13px;
 }
  .reply{
    color: #6A6A6A;
    letter-spacing: 0;
    padding: 5px 0 0;
    font-size: 20px;
    
 }
 .time{
    position: relative;
    float: right;
    color: #6A6A6A;
    right: 3%;
    letter-spacing: -2px;
    font-size: 13px;
 }
 #textarea{
    border-radius: 5px;
    -webkit-border-radius: 5px;
    -moz-border-radius: 5px;
}
 .float_a{
    width: 40px;
    position: fixed;
    right: 3%;
    bottom: 8%;
    z-index: 100;
    text-align: center;
 }
 .float_r{
    position: fixed;
    right: 3%;
    top: 15%;
    z-index: 100;
    text-align: center;
 }
.reply_table{
    width: 25%;
    margin-top: 2%; 
}
.bt{
    border: 0;
    background: #FC7B0C;
    color: #FFFFFF;
    font-size: 14px;
    border-radius: 5px;
    -webkit-border-radius: 5px;
    -moz-border-radius: 5px;
}
.jianbian{
    background: #ebeff3 -webkit-gradient(linear, 0 0, 0 100%, from(#CECECE), to(#292929));/*谷歌*/ 
	FILTER: progid:DXImageTransform.Microsoft.Gradient(gradientType=0,startColorStr=#CECECE,endColorStr=#292929); /*IE 6 7 8*/ 
	background: -ms-linear-gradient(top, #CECECE,  #292929);/* IE 10 */
	background:-moz-linear-gradient(top,#CECECE,#292929);/*火狐*/ 
	background: -webkit-gradient(linear, 0% 0%, 0% 100%, from(#CECECE), to(#292929));/* Safari 4-5, Chrome 1-9*/
	background: -webkit-linear-gradient(top, #CECECE, #292929);/*Safari5.1 Chrome 10+*/
	background: -o-linear-gradient(top, #CECECE, #292929);/*Opera 11.10+*/
}
 </style>
 
 <script type="text/javascript">
window.onload=function(){
    var img = document.getElementById("reply");
    var reply = document.getElementById("textarea");
    var mClose = document.getElementById("mClose");
    var mSubmit = document.getElementById("mSubmit");
    var liuyan_area = document.getElementById("liuyan_area");
    var sub = document.getElementById("sub");
    img.onclick = function(){
        reply.style.display = "block";
        reply.style.position = "fixed";
        reply.style.top = "30%";
            
    }    
    mClose.onclick = function(){
        reply.style.display = "none";
    }
    mSubmit.onclick = function(){
        if(liuyan_area.value.trim().length > 0){
            sub.submit();
        }else{
            reply.style.display = "none";
        }
    }
}
</script>
 
 <body style="max-width:100%; margin:0 auto;">
 <a href="#" id="go_head" ></a>
 <div class="jianbian" style="position: fixed; width: 100%;z-index: 10000; bottom: 0; padding: 2%; color: #FFFFFF;">
 <a href="../index.php" ><img src="../web_images/liuyanban_fanhui.png" style="width: 3%; position: relative; left: 3%;" /></a>
 <img src="../web_images/liuyanban_liuyan.png" style="width: 6%; position: relative; left: 40%;" id="reply" />
 <a href="liuyanban.php"><img src="../web_images/liuyanban_shuaxin.png" style="width: 5%; position: relative; left: 75%;" /></a>
 </div>
 <div style="position: relative;top: -15px; margin-bottom: 20px;">
 <?php
    for($i = 0; $i < 6 ;$i++){
        $id = $result[$i]["id"];
        $replies = $dbhelper->getOne("select count(*) from reply_liuyan where reply_id =".$id);
        $f = $id % NUM;
        if($f == 0){
            $f = NUM;
        }
        echo "<div class='msg'>";
        echo "<p class='floor'>".$f."楼:</p><br />";
        echo "<p style='padding-bottom: 7px; position: relative; top: -5px;'>".$result[$i]["msg"]."</p>";
        echo "<p class='time'>".$result[$i]["msg_time"]."</p><br />";
        echo "<hr style=' border: 0px solid; border-top: 1px solid #EEEEEE; margin: 0;' />";
        echo "<div style='vertical-align: middle; text-align: right; position: relative; padding-right: 10%'><a href='reply.php?id=".$id."'>";
        echo "<p class='reply'><img src='../web_images/liuyanban_huifu.png' style='width: 10%;' /> ".$replies."</p>";
        echo "</a></div>";
        echo "</div>";
    }

    /****** ads *******/
    $ads = $dbhelper->getRow("select * from liuyan where id = 0");
    $ads = (array)$ads;
    $replies = $dbhelper->getOne("select count(*) from reply_liuyan where reply_id = 0");
    echo "<div class='msg' style='background-color: #FC7B0C; '>";
    echo "<div style='padding: 20px 5px; height: 160px; display: table; text-align: center; width: 60%; padding: 0 20%;'>";
    echo "<p style='display: table-cell; vertical-align: middle; color: #FFFFFF;'>".$ads["msg"]."</p>";
    echo "</div>";
    echo "<hr style=' border: 0px solid; border-top: 1px solid #ee9c55; margin: 0;' />";
    echo "<div style='vertical-align: middle; text-align: right; position: relative; padding-right: 10%'><a href='reply.php?id=0'>";
    echo "<p class='reply' style='color: #FFFFFF;'><img src='../web_images/liuyanban_huifu_ads.png' style='width: 10%;' /> ".$replies."</p>";
    echo "</a></div>";
    echo "</div>";
    /****** ads *******/

    for($i = 6; $i < count($result) ;$i++){
        $id = $result[$i]["id"];
        $replies = $dbhelper->getOne("select count(*) from reply_liuyan where reply_id =".$id);
        $f = $id % NUM;
        if($f == 0){
            $f = NUM;
        }
        echo "<div class='msg'>";
        echo "<p class='floor'>".$f."楼:</p><br />";
        echo "<p style='padding-bottom: 7px; position: relative; top: -5px;'>".$result[$i]["msg"]."</p>";
        echo "<p class='time'>".$result[$i]["msg_time"]."</p><br />";
        echo "<hr style=' border: 0px solid; border-top: 1px solid #EEEEEE; margin: 0;' />";
        echo "<div style='vertical-align: middle; text-align: right; position: relative; padding-right: 10%'><a href='reply.php?id=".$id."'>";
        echo "<p class='reply'><img src='../web_images/liuyanban_huifu.png' style='width: 10%;' /> ".$replies."</p>";
        echo "</a></div>";
        echo "</div>";
    }
 ?>
 </div>
 <div id="textarea" style="display: none; z-index: 1000; padding-bottom: 10px; background-color: #CECECE;">
<form  method="post" id="sub" >
<textarea name="liuyan_area" id="liuyan_area" style="height: 150px; resize: none; margin: 10px; border: 0; padding: 0;" oninput="area(this)" ></textarea>
<div style="text-align: center; color: #FFFFFF; width: 95%; font-size: 14px;">
还能输入<i id="number">120</i>字&nbsp;<input type="button" class="bt" id="mSubmit" value="发送" />&nbsp;<input type="button" class="bt" id="mClose" value="取消" />
</div>
</form>
</div>
 <a id="go_bottom"></a>
 </body>
 <script type="text/javascript">
 function area(t){
    if(t.value.length > 120)
        t.value = t.value.substr(0,120);
    var number = document.getElementById("number");
    number.innerHTML = 120 - t.value.length;
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
    var textarea = document.getElementById('textarea');
    var liuyan_area = document.getElementById("liuyan_area");
    var screen = $(window).width();
    //alert(screen);
   var wid = screen*0.5;
   if(screen <= 360){
   if(window.screen.width-360 > 10){
        wid = window.screen.width-360;
   }else{
        wid = 10;
   }
   }else if(screen <= 480){
    if(window.screen.width-400 > 0){
        wid = window.screen.width-400;
   }else{
        wid = 80;
   }
   }
   textarea.style.margin = 5 + 'px ' + Math.floor(wid/2) + 'px';   
   textarea.style.width = (screen - wid) + 'px';
   liuyan_area.style.width = (screen - wid - 20) + 'px';
 </script>
 </html>