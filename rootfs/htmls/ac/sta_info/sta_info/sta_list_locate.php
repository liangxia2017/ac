<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
define("PATH", "../../../");
define("FLAG","");
include PATH."db/dbhelper.php";
include PATH."db/page.php";
?>
<head>
<META   HTTP-EQUIV="pragma"   CONTENT="no-cache">         
<META   HTTP-EQUIV="Cache-Control"   CONTENT="no-cache,   must-revalidate">         
<META   HTTP-EQUIV="expires"   CONTENT="0"> 
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script type="text/javascript" src="../../../js/jquery.js"></script>
<link rel="stylesheet" href="../../../css/body.css" type="text/css" />
<title>定位终端信息表</title>
</head>

<style type="text/css">
<!--

.acinfo_table{
	background:#a8c7ce;
	width:100%;
	border: 1px solid #ddeeff;
}

.tdHeader{
	background-color:#d3eaef;
	padding:5px; 
}

.tdContent{
	background-color:#ffffff;
	padding:5px;
	text-align:center;
}

.tdContentF9{
	background-color:#f9f9f9;
	padding:5px;
	text-align:center;
}


tr.over td{
    background-color:#d5f4fe;
}

-->
</style>


<script type="text/javascript">
	$(document).ready(function(){
	$("#stripe tr").mouseover(function(){
	$(this).addClass("over");
	});
	$("#stripe tr").mouseout(function(){
	$(this).removeClass("over");
	});
	$("#query").val("<?php echo $_POST["query"];?>");
    $("#mac_or_area").val("<?php echo $_POST["mac_or_area"];?>");
	});
    
    function add(t,action){
    	t.form.action=action;
            t.form.submit();
        }
</script>


<?php
//查询操作
if(file_exists("/tmp/sta_coordinate.s3db")){
    if(isset($_POST["query"])){
        if($_POST["query"] == 0){
            $sta_mac = $_POST["mac_or_area"];
            $sta_mac = preg_replace("/:|：| /","",$sta_mac);
            $selectSql="select * from file_data where sta_mac like '%".$sta_mac."%' order by sta_mac";
            $countSql="select count(*) from file_data where sta_mac like '%".$sta_mac."%'";
            $page = new Page($selectSql,$_POST["pageNow"],$countSql);
            $result = $page->getResult();
        }
        if($_POST["query"] == 1){
            $area = $_POST["mac_or_area"];
            $area = preg_replace("/ /","",$area);
            $selectSql="select * from file_data where area_name like '%".$area."%' order by area_name";
            $countSql="select count(*) from file_data where area_name like '%".$area."%'";
            $page = new Page($selectSql,$_POST["pageNow"],$countSql);
            $result = $page->getResult();
        }
    }else{
$selectSql='select * from file_data';
$countSql='select count(*) from file_data';
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
$result = $page->getResult();
    }

}else
    $result = Array();

?>
</head>

<body>
<!--<div class="title">定位终端信息表</div>--><br />
<form name="file_tmp_data" id="file_tmp_data" method="post">
<div style="text-align: left; padding-left: 60px;">
查询方式:<select name="query" id="query" style="margin: 0 5px;">
    <option value="0">MAC地址</option>
    <option value="1">所属定位区域</option>
</select>
<input type="text" id="mac_or_area" name="mac_or_area"/>
<input type="button" class="bt" onclick="add(this,'sta_list_locate.php')" value="确定" />
</div>
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table id="stripe" class="acinfo_table" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">用户MAC地址</td>
        <td class="tdHeader">所属定位区域</td>
        <td class="tdHeader">定位图</td>
      </tr>
      <?php
			$i=0;
			foreach ($result as $rs){
				$rs = (array)$rs;
				$i++;
                if($i%2 == 0){
	   ?>
      <tr>
        <td class="tdContent"><?php echo $i;?>
        </td>
        <td class="tdContent">
        <?php 
         $sta_mac = $rs["sta_mac"];
         $mac = "";
                for($k = 0; $k<strlen($sta_mac); $k++){
                    if($k != 0 && $k%2 == 0)
                        $mac = $mac.":".$sta_mac[$k];
                    else
                        $mac = $mac.$sta_mac[$k];
                }
            echo $mac;
        ?>
        </td>
        <td class="tdContent"><?php echo $rs["area_name"];?>
        </td>
        <td class="tdContent"><a href="#" onclick="window.open('../locate/show_sta.php?sta_mac=<?php echo $sta_mac;?>&area_name=<?php echo $rs["area_name"];?>&flag=1','','height=850, width=1200, top=10, left=10');">查看地图</a>
        </td>
      </tr>
      <?php 
            }else{
       ?>
       <tr>
        <td class="tdContentF9"><?php echo $i;?>
        </td>
        <td class="tdContentF9">
        <?php 
         $sta_mac = $rs["sta_mac"];
         $mac = "";
                for($k = 0; $k<strlen($sta_mac); $k++){
                    if($k != 0 && $k%2 == 0)
                        $mac = $mac.":".$sta_mac[$k];
                    else
                        $mac = $mac.$sta_mac[$k];
                }
            echo $mac;
        ?>
        </td>
        <td class="tdContentF9"><?php echo $rs["area_name"];?>
        </td>
        <td class="tdContentF9"><a href="#" onclick="window.open('../locate/show_sta.php?sta_mac=<?php echo $sta_mac;?>&area_name=<?php echo $rs["area_name"];?>&flag=1','','height=850, width=1200, top=10, left=10');">查看地图</a>
        </td>
      </tr>
     <?php
			}
            }
	?>
    </table>
    </td>
    </tr>
    <tr>
    <td align="left" width="15%"><input class="bt" type="button"	onclick="javascript:window.location.reload(true);" value="刷新" /></td>
		
    <?php
    if($result != "" && $result != null){
    ?>    
		<td align="right" width="85%"><?php $formId='file_tmp_data';include PATH.'db/pageTemplate.php';?>				
		</td>	
    <?php
        }else{
    ?>
        <td width="85%" align="right">
        <span style="padding-right: 50px;">共有<b style="color: red; padding: 5px;">0</b>条记录  当前<strong><b style="color: red; padding-left: 5px;">1</b>/<b style="color: red; padding-right: 5px;">1</b></strong>页
        </span>
        <img align="absbottom" src="../../../images/main_54.gif"/>
        <img align="absbottom" src="../../../images/main_56.gif"/>
        <img align="absbottom" src="../../../images/main_58.gif"/>
        <img align="absbottom" src="../../../images/main_60.gif"/>
        <span style="padding: 5px;">
            转到第
            <input type="text" style="width: 20px; height: 12px; font-size: 12px; border: solid 1px #7aaebd;" value="1" size="2" />
            页
        </span>
        <a href="#">
            <img align="absbottom" style="border: 0" src="../../../images/main_62.gif" />
        </a>
        </td>
    <?php
        }
    ?>
    </tr>
</table>

</form>
</body>
</html>