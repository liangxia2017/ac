<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3c.org/TR/1999/REC-html401-19991224/loose.dtd">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<html><head>
<title>用户登录</title>
<link href="css/Default.css" type="text/css" rel="stylesheet" />
<link href="css/xtree.css" type="text/css" rel="stylesheet" />
<link href="css/User_Login.css" type="text/css" rel="stylesheet" />
<script src="js/jquery.js" type="text/javascript"></script>
<script src="js/jquery.validate.js" type="text/javascript"></script>
<link rel="stylesheet" href="css/screen.css" type="text/css" />
<link rel="stylesheet" href="css/form.css" type="text/css" />
<script type="text/javascript">
	function check(){
		$("#form1").submit(function(){
			var user_name=$("#user_name");
			var user_password=$("#user_password");
			if(user_name.val()==""){
				$("#user_error").html("<br/></br><span style='color: red;'>管理员帐号不能为空！</span>");
				user_name.focus();
				return false;
			}
			if(user_password.val()==""){
				$("#user_error").html("<br/></br><span style='color: red;'>管理员密码不能为空！</span>");
				user_password.focus();
				return false;
			}
			return true;
		});
		
	}
</script>
</head>
<body id="userlogin_body">
  <form method="post" action="login_server.php" name="form1" id="form1">

<div id="user_login">
<dl>
  <dd id="user_top">
  <ul>
    <li class="user_top_l"></li>
    <li class="user_top_c"></li>
    <li class="user_top_r"></li></ul>
  <dd id="user_main">
  <ul>
    <li class="user_main_l"></li>
    <li class="user_main_c">
    <table>
	<tr>
		<td><li class="user_main_text">用户名： </li></td>
		<td><li class="user_main_input"><input class="TxtUserNameCssClass"  id="user_name" name="user_name" /> </li></td>
	</tr>
	<tr>
		<td><li class="user_main_text">密 码： </li></td>
		<td><li class="user_main_input"><input class="TxtPasswordCssClass" id="user_password" type="password" name="user_password"  />  </li></td>
	</tr>
	<tr>
		<td colspan="2"><span id="user_error"></span></td>
	</tr>
	</table>
	<li class="user_main_r">	
	<input type="image" src="images/user_botton.gif" onclick="check()" />
	</li>
	</ul>
  <dd id="user_bottom">
  <ul>
    <li class="user_bottom_l"></li>
    <li class="user_bottom_c"></li>
    <li class="user_bottom_r"></li></ul></dd></dl></div>
    <span id="ValrUserName" style="DISPLAY: none; COLOR: red"></span>
    <span id="ValrPassword" style="DISPLAY: none; COLOR: red"></span>
    <span id="ValrValidateCode" style="DISPLAY: none; COLOR: red"></span>
<div id="ValidationSummary1" style="DISPLAY: none; COLOR: red"></div>
</form></body>
<script type="text/javascript">
	$(document).ready(function(){
		if("error"=="<?php echo $_GET['action']?>"){
			$("#user_error").html("<br/></br><span style='color: red;'>管理员帐号或密码错误！</span>");
		}
	});
	$("#user_name").focus();
</script>
</html>