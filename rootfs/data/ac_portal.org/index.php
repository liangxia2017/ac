<?php
session_start();
if(isset($_GET["7de5a7d40407d41473b51a7f23af5f15"])){
	Header("Location:http://10.1.1.2/web_phone/login.php?freewifi=free");
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
 <head>
  <title> 首页 </title>
	<meta  http-equiv="pragma" content="no-cache" />
	<meta  http-equiv="Cache-Control" content="no-cache,must-revalidate" />
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
	<meta  http-equiv="expires" content="0" />
    <script src="web_phone/db/jquery.js" type="text/javascript"></script>
 </head>
<style type="text/css">
*{
    font-family: monospace;
    font-weight: bold;
    font: normal 100%;
}
body{
/*    background-image: url('../web_images/index_back.jpg');
    background-size: 100% auto;
    background-repeat: no-repeat;*/
    background: #F9F9F9;
}
.div1{
	width: 30%;
	margin: 2% 0;
    position: relative;
    float: left;
    text-align: center;
    font-size: 16px;
    font-weight: normal;
    color: #3E3E3E;
}
.div1:nth-child(odd){
	margin-left: 15%;
}
.div1:nth-child(even){
	margin-left: 10%;
}
a{
    color: black;
}
img{
	width: 100%;
    float: left;
    border: 0;
    padding-bottom: 5px;
}
#login,#vip_regist{
    background-color:transparent;
    background-image: url('../web_images/denglu_back.png');
    background-position: center;
    background-size: 100% 100%;
    background-repeat: no-repeat;
}
.login_table{
    width: 90%;
    /*padding-top: 10%;*/
    padding: 2% 0;
}
.li_div{
    width: 5%;
    padding: 1%;
    float: left;
}
input[type="text"],[type="password"]{
    border: 0;
    border-radius: 2px;
    -webkit-border-radius: 2px;
    -moz-border-radius: 2px;
}
.bt{
    border: 0;
    padding: 2% 4%;
    background: #CE761D;
    color: #FFFFFF;
    font-size: 14px;
    border-radius: 5px;
    -webkit-border-radius: 5px;
    -moz-border-radius: 5px;
}
td{
    padding: 3px 0;
}
</style>

<script type="text/javascript">
window.onload=function(){
    var img = document.getElementById("log_img");
    var login = document.getElementById("login");
    var mClose = document.getElementById("mClose");
    img.onclick = function(){
        login.style.display = "block";
        login.style.position = "absolute";
        login.style.top = "15%";

    }
    mClose.onclick = function(){
        login.style.display = "none";
    }


    /********注册*********/
    var vip_regist = document.getElementById("vip_regist");
    var regist = document.getElementById("regist");
    var vip_mClose = document.getElementById("vip_mClose");
    regist.onclick = function(){
        vip_regist.style.display = "block";
        vip_regist.style.position = "absolute";
        vip_regist.style.top = "15%";

    }
    vip_mClose.onclick = function(){
        vip_regist.style.display = "none";
    }
    /*********************/


    var action = window.location.search.substr(1);
    var login_div = document.getElementById("login_div");
    var user_center = document.getElementById("user_center");
    <?php
    if(isset($_SESSION["user"])){
        if($_SESSION["user"] == "youke"){
   ?>
        login_div.style.display = "none";
        user_center.style.display = "";
        document.getElementById("porsen_center").innerHTML = "游客";
   <?php
        }else{
    ?>
        login_div.style.display = "none";
        user_center.style.display = "";
        document.getElementById("porsen_center").innerHTML = "个人中心";
    <?php
        }
    }else{
    ?>
        login_div.style.display = "";
        user_center.style.display = "none";
    <?php
    }
    ?>
}


</script>

 <body style="max-width:100%; margin:0 auto; margin-top: 5%; padding: 5% 0">
<!-- <div><img src="../web_images/head.png" style="width: 30%; padding: 3% 50% 2% 5%;" /></div>-->
  <div class="div1" style="margin-left: 15%;" id="login_div"><img src="../web_images/denglu.png" id="log_img" /><br />登录</div>
  <div class="div1" style="margin-left: 15%;" id="user_center"><a href="#"><img src="../web_images/user_center.png" /></a><br /><span id="porsen_center">个人中心</span></div>
  <div class="div1" style="margin-left: 10%;"><a href="web_phone/area.php"><img src="../web_images/zhoubian.png" /></a><br />周边</div>

  <div class="div1" style="margin-left: 15%;"><a href="web_phone/undo.php"><img src="../web_images/yaoyue.png" /></a><br />邀约</div>
  <div class="div1" style="margin-left: 10%;"><a href="web_phone/undo.php"><img src="../web_images/shinei.png" /></a><br />室内</div>

  <div class="div1" style="margin-left: 15%;"><a href="web_phone/jingpin.php"><img src="../web_images/jingpin.png" /></a><br />精品</div>
  <div class="div1" style="margin-left: 10%;"><a href="web_phone/liuyanban.php"><img src="../web_images/digu.png" /></a><br />嘀咕</div>
<div style="float: left; width: 100%; text-align: right; font-size: 12px; color: #9F9F9F; margin: 20px 0;">无线接入研发部&nbsp;&nbsp;</div>

<?php
    //var_dump($_SESSION);
?>

<div id="login" style="display: none; z-index: 1000; padding: 0;">
<div>
    <ul style="list-style: none;">
        <li style="margin: 5% 0 5% 8%; float: left; color: white;" id="sms" onclick="select_methd(this.id)">短信登录</li>        
        <li style="margin: 5% 0 5% 8%; float: left;" id="vip" onclick="select_methd(this.id)">会员登录</li>
        <li style="margin: 5% 0 5% 8%; float: left;" id="game" onclick="select_methd(this.id)">游戏登录</li>
        <li style="margin: 5% 0 5% 8%; float: left;" id="weixin" onclick="select_methd(this.id)">微信登录</li>
    </ul>
</div>
<form name="form_vali" method="post" action="sms/sms_validate.php" >
<table align="center" valign="middle" cellpadding="0px" cellspacing="0px" class="login_table">

    <!-- 短信认证 -->
    <tr id="sms_usr">
        <td align="right" width="30%">手机号:</td>
        <td align="left">
            <input type="text" id="sms_username" name="sms_username" />
            <input type="button" id="get_ver" value="获取验证码" onclick="ajaxFunction()" />
        </td>
    </tr>
    <tr id="sms_psw">
        <td align="right">验证码:</td>
        <td align="left"><input type="text" id="sms_password" name="sms_password" /></td>
    </tr>


    <tr id="vip_usr" style="display: none;">
        <td align="right" width="30%">会员号:</td>
        <td align="left"><input type="text" id="vip_username" name="vip_username" /></td>
    </tr>
    <tr id="vip_psw" style="display: none;">
        <td align="right">密&nbsp;&nbsp;码:</td>
        <td align="left"><input type="password" id="vip_password" name="vip_password" /></td>
    </tr>
   

	<!-- 关注微信公众号 -->
    <tr id="weixin_usr" style="display: none;">
        <td colspan="2" align="center" ><img src="weixin/gr_iot.jpg" style="width: 30%; float: none;" /></td>
    </tr>
    <tr id="weixin_psw" style="display: none;">
        <td colspan="2" align="center"  >关注微信公众号：GR_IOT<br />
        													1、点击页面“确定”，您将获得<span style="color: white;">5分钟</span>上网时限<br/>
        													2、请于5分钟内<span style="color: white;">关注公众号</span>并获取公众号中<span style="color: white;">freeWIFI</span>激活链接，点击链接获得全天时限
        </td>
    </tr>

    <!-- 游戏获取免费时长 -->
    <tr id="game_usr" style="display: none; text-align: left; padding: 1% 0;">
        <td colspan="2" >亲，游戏分数将决定您今天的上网时长或国人大堂咖啡厅优惠券额度哦！</td>
    </tr>
    <tr id="game_psw" style="display: none; text-align: left; padding: 1% 0;">
        <td colspan="2" ></td>
    </tr>


    <tr>
      <td colspan="2" align="center" style="padding: 2% 0;" >
        <input type="button" class="bt" id="confir" name="sms" value="确定" onclick="ver_vali(this.name)" />&nbsp;&nbsp;
        <input type="button" class="bt" id="mClose" value="取消" />&nbsp;&nbsp;
        <input type="button" class="bt" id="regist" style="display: none;" value="注册" />
      </td>
    </tr>
</table>
<!--
<div>
<ul style="list-style: none; padding-left: 10%;">温馨提示:
<li style="padding: 2px;">关注微信公众号获取卡号和密码登录</li>
<li style="padding: 2px;">输入会员卡上的卡号和密码登录</li>
</ul>
</div>
-->
<div style="padding: 1% 0 1% 20%"><a href="#">使用QQ登录</a></div><!--
<div style="padding: 1% 0 1% 20%"><a href="#">使用微信登录</a></div>-->
<div style="padding: 1% 0 1% 20%"><a href="#">使用新浪微博登录</a></div>
</form>
</div>

<div id="vip_regist" style="display: none; z-index: 10000; padding: 0;">
<form name="form_vip">
<table align="center" valign="middle" cellpadding="0px" cellspacing="0px" class="login_table" style="padding: 5% 0;">
    <th style="text-align:center;" colspan="2"><div style="font-size:22px; font-family: bold; letter-spacing: 10px;">会员注册</div></th>
    <tr>
        <td align="right" width="30%" style="padding-top: 5%;">会员号:</td>
        <td align="left" style="padding-top: 5%;"><input type="text" id="regist_username" name="regist_username" /></td>
    </tr>
    <tr>
        <td align="right" style="padding-top: 5%;">密&nbsp;&nbsp;码:</td>
        <td align="left" style="padding-top: 5%;"><input type="password" id="regist_password" name="regist_password" /></td>
    </tr>

    <tr>
      <td colspan="2" align="center" style="padding: 3% 0;" >
        <input type="button" class="bt" value="确定" onclick="vip_reg()" />&nbsp;&nbsp;
        <input type="button" class="bt" id="vip_mClose" value="取消" />
      </td>
    </tr>
</table>
</form>
</div>

 </body>
 <script type="text/javascript">
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
   var wid = screen*0.6;
   if(screen < 500){
   if(screen-360 > 0){
        wid = screen-360;
   }else{
        wid = 10;
   }
   }
   var log = document.getElementById("login");
   var reg = document.getElementById("vip_regist");
   log.style.margin = '0 ' + Math.floor(wid/2) + 'px';
   log.style.width = (screen - wid) + 'px';
   reg.style.margin = '0 ' + Math.floor(wid/2) + 'px';
   reg.style.width = (screen - wid) + 'px';
   reg.style.height = (screen - wid)*0.8 + 'px';
</script>

<script type="text/javascript">
var li = document.getElementsByTagName("li");
var usr;
var psw;
var action;
var confir = document.getElementById("confir");
var regist = document.getElementById("regist");
function select_methd(t){
    confir.name = t;
    if(t == "vip"){
        regist.style.display = "";
    }else{
        regist.style.display = "none";
    }
    if(t == "weixin"){
    	action = "index.php";
    }else{
    	action = t + "/" + t + "_validate.php";
    }
    for(var i=0; i<li.length; i++){
        var tag_usr = li[i].id + "_usr";
        var tag_psw = li[i].id + "_psw";
        usr = document.getElementById(tag_usr);
        psw = document.getElementById(tag_psw);
        if(li[i].id != t){
            li[i].style.color = "black";
            usr.style.display = "none";
            psw.style.display = "none";
        }else{
            li[i].style.color = "white";
            usr.style.display = "";
            psw.style.display = "";
            this.form_vali.action = action;
        }
    }
}
</script>


<script type="text/javascript">

var get_ver = document.getElementById("get_ver");//获取验证码按钮
var s;//setInterval
var n = 59;//计时数

//获取短信验证码
function ajaxFunction(){

    //按钮变灰
    get_ver.disabled = "disabled";

    var tel_reg = /^1[3|4|5|8][0-9]\d{8}$/;
    var sms_username = document.getElementById("sms_username");
    var tel = sms_username.value;
    //var time = new Date().getTime();
    if(tel.length != 11){
        alert("请输入有效的手机号码!");
        document.form_vali.sms_username.focus();
        get_ver.disabled = "";
    }else if(!tel_reg.test(tel)){
        alert("请输入有效的手机号码!");
        document.form_vali.sms_username.focus();
        get_ver.disabled = "";
    }else{
        var xmlHttp = creatHttp();
        xmlHttp.onreadystatechange=function(){
            if(xmlHttp.readyState==4){
                if(xmlHttp.responseText == 0){//短信发送成功
                    alert("验证码已发送!");
                    s = setInterval("clock()",1000);
                }else{//发送失败
                    alert("验证码获取失败!");
                    get_ver.disabled = "";
                }
            }
        }
        //xmlHttp.open("GET","sms/sms.php?tel="+tel+"&time="+time,true);
        xmlHttp.open("GET","sms/sms.php?tel="+tel,true);
        xmlHttp.send(null);
     }
 }


//按钮的计时器
function clock(){
    if(n <= 0){
    	clearInterval(s);
    	n = 59;
    	get_ver.value = "获取验证码";
        get_ver.disabled = "";
    }else{
    	get_ver.value = "再次获取("+n+"秒)";
    	n--;
    }
}


//创建http
function creatHttp(){
    var xh;
    try{
        // Firefox, Opera 8.0+, Safari
        xh=new XMLHttpRequest();
    }catch (e){
        // IE
        try{
          xh=new ActiveXObject("Msxml2.XMLHTTP");
        }catch (e){
            try{
                xh=new ActiveXObject("Microsoft.XMLHTTP");
            }catch (e){
                alert("浏览器不支持AJAX!");
                return false;
            }
        }
    }
    return xh;
}

</script>

<script type="text/javascript">
//提交登录验证
function ver_vali(t){
    if(t == "sms"){
        if(document.form_vali.sms_password.value.trim().length > 0){
            var xmlHttp_ver = creatHttp();
            var url = document.form_vali.action+"?code="+document.form_vali.sms_password.value;
            //alert(url);
            xmlHttp_ver.onreadystatechange=function(){
                if(xmlHttp_ver.readyState==4){
                    //alert(xmlHttp_ver.responseText);
                    /**
                    if(xmlHttp_ver.responseText != 0){//验证成功
                        alert(xmlHttp_ver.responseText);
                    }else{//验证失败
                        alert("错误,请重新输入!");

                    }
                    **/

                    if(xmlHttp_ver.responseText == "fail"){//验证失败
                        alert("错误,请重新输入!");
                        document.form_vali.sms_password.value = "";
                        document.form_vali.sms_password.focus();
                    }else{//验证成功
                        top.location = xmlHttp_ver.responseText;
                    }
                }
            }
            xmlHttp_ver.open("GET",url,true);
            xmlHttp_ver.send(null);
        }else{
            alert("请输入验证码!");
        }
    }else if(t == "game"){
        var xmlHttp_game = creatHttp();
        xmlHttp_game.onreadystatechange=function(){
            if(xmlHttp_game.readyState == 4){
                if(xmlHttp_game.responseText == "fail"){
                    alert("对不起,游客每天只能提交一次游戏登录!");
                }else{
                    top.location = xmlHttp_game.responseText;
                }
            }
        }
        xmlHttp_game.open("GET","web_phone/login.php?action=game_twice",true);
        xmlHttp_game.send(null);
    }else if(t == "vip"){
        /*****************会员登录*****************/
        var vip_username = document.form_vali.vip_username.value.trim();
        var vip_password = document.form_vali.vip_password.value.trim();
        if(vip_password.length > 0 && vip_username.length > 0){
            var xmlHttp_vip = creatHttp();
            var url = document.form_vali.action+"?vip_username="+vip_username+"&vip_password="+vip_password;
            xmlHttp_vip.onreadystatechange=function(){
                if(xmlHttp_vip.readyState==4){                    
                    if(xmlHttp_vip.responseText == 0){//验证失败
                        alert("密码错误,请重新输入!");
                        document.form_vali.vip_password.value = "";
                        document.form_vali.vip_password.focus();
                    }else{//验证成功
                        top.location = xmlHttp_vip.responseText;
                    }
                    //alert(xmlHttp_vip.responseText);
                }
            }
            xmlHttp_vip.open("GET",url,true);
            xmlHttp_vip.send(null);
        }else{
            alert("请输入会员号和密码!");
        }
    }else{
    	 var xmlHttp_weixin = creatHttp();
        xmlHttp_weixin.onreadystatechange=function(){
            if(xmlHttp_weixin.readyState == 4){
                if(xmlHttp_weixin.responseText == "exist"){
                    alert("您已获得上网权限!");
                }else{
                    alert("您已获得上网权限,请于5分钟内关注公众号激活永久上网权限!");
                     log.style.display = "none";
                }
            }
        }
        xmlHttp_weixin.open("GET","web_phone/login.php?action=weixin",true);
        xmlHttp_weixin.send(null);
    }
}

/*********注册***********/
function vip_reg(){
    var regist_username = document.form_vip.regist_username.value.trim();
    var regist_password = document.form_vip.regist_password.value.trim();
    if(regist_username.length > 0 && regist_password.length > 0){
        var xmlHttp_vip_reg = creatHttp();
        var url = "vip/vip_regist.php?regist_username="+regist_username+"&regist_password="+regist_password;
        xmlHttp_vip_reg.onreadystatechange=function(){
            if(xmlHttp_vip_reg.readyState==4){                    
                if(xmlHttp_vip_reg.responseText == 1){//验证失败
                    alert("该会员号已经存在!");
                    document.form_vip.regist_username.value = "";
                    document.form_vip.regist_password.value = "";
                    document.form_vip.regist_username.focus();
                }else{//验证成功
                    alert("注册成功!");
                    document.form_vip.regist_username.value = "";
                    document.form_vip.regist_password.value = "";
                    document.getElementById("vip_regist").style.display = "none";
                }
            }
        }
        xmlHttp_vip_reg.open("GET",url,true);
        xmlHttp_vip_reg.send(null);
    }else{
        alert("会员号和密码不能为空!");
    }
}
/*************************/
</script>

</html>