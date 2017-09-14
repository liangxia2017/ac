<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<?php
include "db/dbhelper.php";
$dbhelper = new DAL();
?>
<html>
 <head>
  <title> 留言回复 </title>
	<meta  http-equiv="pragma" content="no-cache" />
	<meta  http-equiv="Cache-Control" content="no-cache,must-revalidate" />
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
	<meta  http-equiv="expires" content="0" />
 </head>
 <?php
 if(isset($_POST["liuyan_area"]) && $_POST["liuyan_area"] != ""){
    $params = array($_POST["liuyan_area"],date("Y年m月d日 H:i"),$_POST["id"]);
    $sql = "insert into reply_liuyan(msg,msg_time,reply_id) values (?,?,?)";
    $insert = $dbhelper->insert($sql,$params);
    if($insert > 0){
        echo "<script>location='reply.php?id=".$_POST["id"]."&r='+Math.random();</script>"; 
    }
 }
 ?>
 <?php
    $liuyan = $dbhelper->getRow('select * from liuyan where id ='.$_GET["id"]);//留言
    $liuyan = (array)$liuyan;
    $replies = $dbhelper->getall("select * from reply_liuyan where reply_id =".$id." order by id");//留言对应的回复
 ?>
 
 <style type="text/css">
*{
    font-family: monospace;
    font-size: 18px;
    word-break: keep-words;
    word-wrap: break-word;
}
img{
    border: 0;
}
 body{
    background-color: #EEEEEE;
 }
 .liuyan_msg{
    background-color: #FFFFFF;
    margin-bottom: 12px;
    padding: 5px;
    padding-left: 10px;
    border-top: 1px solid #CECECE;
    border-bottom: 1px solid #CECECE;
 }
 .reply_msg{
    background-color: #FFFFFF;
    padding: 5px;
    padding-left: 10px;
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

 .time{
    position: relative;
    float: right;
    color: #6A6A6A;
    right: 3%;
    letter-spacing: -2px;
    font-size: 13px;
    bottom: -8px;
 }
 #textarea{
    background-color: #FFFFFF;
    position: relative;
    top: -10px;
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
 
 <body style="max-width:100%; margin:0 auto;">
 <div class="jianbian" style="position: fixed; width: 100%;z-index: 10000; bottom: 0; padding: 2%; color: #FFFFFF;">
 <a href="liuyanban.php" ><img src="../web_images/liuyanban_fanhui.png" style="width: 3%; position: relative; left: 3%;" /></a>
 <a href="#go_bottom"><img src="../web_images/liuyanban_liuyan.png" style="width: 6%; position: relative; left: 40%;"/></a>
 <a href="reply.php?id=<?php echo $_GET["id"];?>"><img src="../web_images/liuyanban_shuaxin.png" style="width: 5%; position: relative; left: 75%;" /></a>
 </div>
 
 <div style="position: relative; top: -10px;"> 
 <div class='liuyan_msg'> 
        <p>层主:</p>
        <p style="padding: 10px 0;"><?php echo $liuyan["msg"];?></p>
        <div class='time'><?php echo $liuyan["msg_time"];?></div>
        <br />
 </div>
 
 <?php
    for($i = 0; $i < count($replies) ;$i++){
        echo "<div class='reply_msg'  style='background-color: #FFFFFF; '>";
        echo "<p class='floor'>".($i+1)."L:</p>";
        echo "<p style='font-size: 16px; color: #3A3A3A; padding: 5px 0; '>".$replies[$i]["msg"]."</p>";
        echo "<p class='time'>".$replies[$i]["msg_time"]."</p>";
        echo "<br /></div>";
        echo "<hr style=' border: 0px solid; border-top: 1px solid #EEEEEE; margin: 0;' />";
    }
 ?>
</div>
 
<div id="textarea" style="padding: 5px 0; margin-bottom: 8%; border-top: 1px solid #EEEEEE;">
<form method="post">
 <input type="hidden" name="id" value="<?php echo $_GET["id"];?>" />
<textarea name="liuyan_area" id="liuyan_area" style="width: 95%; height: 150px; resize: none; margin: 5px;" oninput="textarea(this)" ></textarea>
<div style="text-align: center;">
还能输入<i id="number">120</i>字&nbsp;&nbsp;<input type="button" onclick="testarea(this)" class="bt" value="回 复" />
</div>
</form>
<a id="go_bottom" href="#"></a>
</div> 
<script type="text/javascript">
 function textarea(t){
    if(t.value.length > 120)
        t.value = t.value.substr(0,120);
    var number = document.getElementById("number");
    number.innerHTML = 120 - t.value.length;
 }
 function testarea(t){
    var liuyan_area = document.getElementById("liuyan_area");
    if(liuyan_area.value.trim().length > 0)
        t.form.submit();
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
</script>
 </body>
 </html>