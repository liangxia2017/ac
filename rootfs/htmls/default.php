<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>无标题文档</title>
<style type="text/css">
<!--
.STYLE3 {
	font-size: 24px;
	color: #428eff;
}
-->
</style>
</head>

<body>
<span class="STYLE3">欢迎进入
<?php
	$param = $_GET['id'];
	if($param=='1'){
		echo "AC配置";
	}else if($param=='2'){
		echo " 分组管理";
	}else if($param=='3'){
		echo " 统计信息";
	}
?>
</span>
</body>
</html>