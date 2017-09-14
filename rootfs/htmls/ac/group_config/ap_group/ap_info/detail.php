<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script type="text/javascript" src="<?php echo PATH ?>js/jquery.js"></script>
<script src="<?php echo PATH ?>js/jquery.validate.js" type="text/javascript"></script>
<script src="<?php echo PATH ?>js/checkbox.js" type="text/javascript"></script>
<link rel="stylesheet" href="<?php echo PATH ?>css/body.css" type="text/css" />
</head>
<style type="text/css">
<!--
.tdContent{
	background-color:#ffffff;
	padding:5px;
	text-align:center;
}
li{
    float: left;
    font-weight: bold;
    color: gray;
    font-size: 14px;
    padding: 5px 28px;
}
.onclick{    
    background: url(../../../../images/detail_onclick.jpg) no-repeat;
    color: #0099CC;
}
.unclick{    
    background: url(../../../../images/detail.jpg) no-repeat;    
    cursor:pointer;
}
-->
</style>
<script type="text/javascript">
var li = document.getElementsByTagName("li");
function list_sub_nav(flag){
    switch(flag){
        case 0:
            if(li[0].className != "onclick"){
                for(var i=0;i<li.length;i++){
                    if(i != 0)
                        li[i].className = "unclick";
                }        
                li[0].className="onclick";
                document.form.target = "show_detail";
                window.top.show_detail.location = "../ap_info_detail/basic_info.php?ap_mac=<?php echo $_GET["ap_mac"];?>";   
            }
            break;
        case 1:
            if(li[1].className != "onclick"){
                for(var i=0;i<li.length;i++){
                    if(i != 1)
                        li[i].className = "unclick";
                }        
                li[1].className="onclick";
                document.form.target = "show_detail";
                window.top.show_detail.location = "../ap_info_detail/user_list.php?ap_mac=<?php echo $_GET["ap_mac"];?>";   
            }
            break;        
        case 2:
            if(li[2].className != "onclick"){
                for(var i=0;i<li.length;i++){
                    if(i != 2)
                        li[i].className = "unclick";
                }        
                li[2].className="onclick";
                document.form.target = "show_detail";
                window.top.show_detail.location = "../ap_info_detail/ap_report.php?ap_mac=<?php echo $_GET["ap_mac"];?>";   
            }
            break;
    }
}
</script>
<body>
<form name="form" action="">
<table style="width: 100%; height: 70px; margin-top: -20px;">
<tr>
<td class="tdContent">
<ul style="list-style: none;">
	<li onclick="list_sub_nav(0)" class="onclick">基本信息</li>
	<li onclick="list_sub_nav(1)" class="unclick">用户列表</li>
  <li onclick="list_sub_nav(2)" class="unclick">网管统计</li>
</ul>
</td>
</tr>
</table>
<table style="width: 100%; height: 90%; margin-top: -20px;">
<tr>
<iframe src="../ap_info_detail/basic_info.php?ap_mac=<?php echo $_GET["ap_mac"];?>" name="show_detail" style="height: 700px; width: 100%;" frameborder="0"></iframe>
</tr>
</table>
</form>
</body>
</html>