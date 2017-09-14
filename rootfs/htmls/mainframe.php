<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>管理导航区域</title>
<link rel="stylesheet" href="css/common.css" type="text/css" />
</head>
<script type="text/javascript">
var preClassName = "man_nav_2";
function list_sub_nav(Id,sortname){
   if(preClassName != ""){
      getObject(preClassName).className="bg_image";
   }
   if(getObject(Id).className == "bg_image"){
      getObject(Id).className="bg_image_onclick";
      preClassName = Id;
	  showInnerText(Id);
	  window.top.frames['leftFrame'].outlookbar.getbytitle(sortname);
	  window.top.frames['leftFrame'].outlookbar.getdefaultnav(sortname);
	  /*
	  点击主菜单切换
	  */
	  /**
	  var url = "manframe.php";
	  if(sortname==1){
		  url="manframe.php";
	  }else if(sortname==2){
		  url="ac/group_config/group_config/ap_group/ap_group.php";
	  }else if(sortname==3){
		  url="ac/glob_config/basic_config/radius/list_radius.php";
	  }else if(sortname==4){
		  url="ac/access_config/access_config/net_info/net_info.php";
	  }else if(sortname==5){
		  url="ac/online_count/online_ap/ap_list/ap_list.php";
	  }
	  //alert(window.parent.document.getElementById("manFrame").src);
	  window.parent.document.getElementById("manFrame").src=url;
	  */
	  window.parent.document.getElementById("manFrame").src="default.php?id="+sortname;
   }
}

function showInnerText(Id){
    var switchId = parseInt(Id.substring(8));
	var showText = "对不起没有信息！";
	switch(switchId){
	    case 1:
		   showText =  "欢迎使用WLAN集中控制系统!";
		   break;
	    case 2:
		   showText =  "WLAN/AP分组管理系统";
		   break;
	    case 3:
		   showText =  "终端统计和终端定位";
		   break;		   		   
	}
	getObject('show_text').innerHTML = showText;
}
 //获取对象属性兼容方法
 function getObject(objectId) {
    if(document.getElementById && document.getElementById(objectId)) {
	// W3C DOM
	return document.getElementById(objectId);
    } else if (document.all && document.all(objectId)) {
	// MSIE 4 DOM
	return document.all(objectId);
    } else if (document.layers && document.layers[objectId]) {
	// NN 4 DOM.. note: this won't find nested layers
	return document.layers[objectId];
    } else {
	return false;
    }
}
</script>
<body>
<div id="nav">
<ul>
	<li id="man_nav_1" onclick="list_sub_nav(id,1)" class="bg_image">AC配置</li>
	<li id="man_nav_2" onclick="list_sub_nav(id,2)"	class="bg_image_onclick">分组管理</li>
	<li id="man_nav_3" onclick="list_sub_nav(id,3)" class="bg_image">统计信息</li>
</ul>
</div>
		<div id="sub_info">
			&nbsp;&nbsp;
			<img src="images/hi.gif" />
			&nbsp;
			<span id="show_text">欢迎使用后台管理系统!</span>
		</div>
</body>
</html>
