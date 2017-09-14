<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<link rel="stylesheet" href="css/common.css" type="text/css" />
<title>显示/隐藏左侧导航栏</title>
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript">
var status = 0;
function submit_onclick(){
	if(status==0) {
		parent.document.getElementById("myFrame").cols="0,7,*";
		$("#ImgArrow").attr("src","images/switch_right.gif");
		$("#ImgArrow").attr("alt","打开左侧导航栏");
		status=1;
	} else {
		parent.document.getElementById("myFrame").cols="199,7,*";
		$("#ImgArrow").attr("src","images/switch_left.gif");
		$("#ImgArrow").attr("alt","隐藏左侧导航栏");
		status=0;
	}
}
</script>
</head>

<body>
<div id="switchpic"><a href="javascript:submit_onclick()"><img src="images/switch_left.gif" alt="隐藏左侧导航栏" id="ImgArrow" /></a></div>
</body>
</html>

