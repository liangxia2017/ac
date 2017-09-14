<?php
session_start();
if($_SESSION["USER_NAME"] == null | $_SESSION["USER_NAME"] == ""){
		echo "<script>alert('请先登录!');top.location='login.php';</script>";
	}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<title>WLAN AC</title>
<link rel="stylesheet" href="css/common.css" type="text/css" />
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
</head>
<frameset rows="50,*" cols="*" frameborder="no" border="0" framespacing="0">
  <frame src="topframe.php" name="topFrame" frameborder="0" scrolling="No" noresize="noresize" id="topFrame" title="topFrame" />
  <frameset id="myFrame" cols="199,7,*" frameborder="no" border="0" framespacing="0">
    <frame src="leftframe.php" name="leftFrame" frameborder="0" scrolling="No" noresize="noresize" id="leftFrame" title="leftFrame" />
	<frame src="switchframe.php" name="midFrame" frameborder="0" scrolling="No" noresize="noresize" id="midFrame" title="midFrame" />
    <frameset rows="59,*" cols="*" frameborder="no" border="0" framespacing="0">
         <frame src="mainframe.php" name="mainFrame" frameborder="0" scrolling="No"  noresize="noresize" id="mainFrame" title="mainFrame" />
         <frame src="ac/group_config/ap_group/ap_group_config/ap_group_config.php" name="manFrame" frameborder="0" id="manFrame" title="manFrame" />
     </frameset>
  </frameset>
</frameset>
<noframes><body>
</body>
</noframes>
</html>