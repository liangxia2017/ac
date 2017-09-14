<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";
include PATH."db/page.php";
?>
<head>
<META   HTTP-EQUIV="pragma"   CONTENT="no-cache">         
<META   HTTP-EQUIV="Cache-Control"   CONTENT="no-cache,   must-revalidate">         
<META   HTTP-EQUIV="expires"   CONTENT="0"> 
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script type="text/javascript" src="<?php echo PATH ?>js/jquery.js"></script>
<script src="<?php echo PATH ?>js/jquery.validate.js" type="text/javascript"></script>
<script src="<?php echo PATH ?>js/checkbox.js" type="text/javascript"></script>
<link rel="stylesheet" href="<?php echo PATH ?>css/body.css" type="text/css" />
<title>用户信息表</title>
</head>

<style type="text/css">
<!--
.acinfo_table{
	background:#a8c7ce;
	width:100%;
/*	border: 1px solid #ddeeff;*/
}

.tdHeader{
	background-color:#d3eaef;
/*	padding:5px; */
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

.t {
	font-size: 18px;
	letter-spacing:4px;
	width: 96%;
	font-weight:bold;
	text-align: center;
    padding-top: 0;
	color:#0099CC;
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
	});
</script>
<?php
//查询操作
$selectSql="select * from sta_list_assc where assc_ap_mac='".$_GET["ap_mac"]."'";
$countSql="select count(*) from sta_list_assc where assc_ap_mac='".$_GET["ap_mac"]."'";
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
?>
</head>
<script>
	function add(t,action){
	t.form.action=action;
	t.form.submit();	
	}
</script>
<body>
<div class="t">用户信息表</div>
<form name="user_list" id="user_list" method="post">
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table class="acinfo_table"  id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	onclick="select_all(this,'id[]')" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">用户mac</td>
        <td class="tdHeader">所属APmac</td>
        <td class="tdHeader">信道</td>
        <!--<td class="tdHeader">用户所属SSID</td>-->
        <td class="tdHeader">用户上线时间</td>
        <td class="tdHeader">操作</td>
      </tr>
      <?php
			$result = $page->getResult();
			$i=0;
			foreach ($result as $rs){
				$rs = (array)$rs;
				$i++;
                if($i%2 == 0){
	   ?>
      <tr>
        <td class="tdContent"><input type="checkbox" name="id[]" id="id" value="<?php echo ($rs["id"]);?>" />
		</td>
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
         $dbhelper = new DAL();
        $num = $dbhelper->getOne("select count(*) from sta_blacklist where sta_mac='".$sta_mac."' and ap_group_name in (select ap_group_name from ap_info where ap_mac='".$rs["assc_ap_mac"]."')");
        if($num>0)
            echo "<font color='red'>".$mac."</font>";
        else
         echo $mac;
        ?>
        </td>
        <td class="tdContent">
        <?php 
         $assc_ap_mac = $rs["assc_ap_mac"];
         $mac = "";
         for($k = 0; $k<strlen($assc_ap_mac); $k++){
            if($k != 0 && $k%2 == 0)
                $mac = $mac.":".$assc_ap_mac[$k];
            else
                $mac = $mac.$assc_ap_mac[$k];
            }
         echo $mac;
        ?>
        </td>
        <!--<td class="tdContent"><?php echo $rs["assc_ssid"];?>
        </td>-->
        <td class="tdContent"><?php echo $rs["radio"];?>
        <td class="tdContent"><?php echo $rs["assc_time"];?>
        </td>
        <td class="tdContent">
        <a href="#" onclick="window.open('../../../sta_info/sta_snmp_info/sta_snmp_info.php?sta_mac=<?php echo $sta_mac;?>','','height=770, width=1000, top=60, left=150');">详细</a>
        </td>
      </tr>
      <?php
			}else{
		?>
       <tr>
        <td class="tdContentF9"><input type="checkbox" name="id[]" id="id" value="<?php echo ($rs["id"]);?>" />
		</td>
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
        $dbhelper = new DAL();
        $num = $dbhelper->getOne("select count(*) from sta_blacklist where sta_mac='".$sta_mac."' and ap_group_name in (select ap_group_name from ap_info where ap_mac='".$rs["assc_ap_mac"]."')");
        if($num>0)
            echo "<font color='red'>".$mac."</font>";
        else
         echo $mac;
        ?>
        </td>
        <td class="tdContentF9">
        <?php 
         $assc_ap_mac = $rs["assc_ap_mac"];
         $mac = "";
         for($k = 0; $k<strlen($assc_ap_mac); $k++){
            if($k != 0 && $k%2 == 0)
                $mac = $mac.":".$assc_ap_mac[$k];
            else
                $mac = $mac.$assc_ap_mac[$k];
            }
         echo $mac;
        ?>
        </td>
        <!--<td class="tdContentF9"><?php echo $rs["assc_ssid"];?>
        </td>-->
        <td class="tdContentF9"><?php echo $rs["radio"];?>
        <td class="tdContentF9"><?php echo $rs["assc_time"];?>
        </td>
        <td class="tdContentF9">
        <a href="#" onclick="window.open('../../../sta_info/sta_snmp_info/sta_snmp_info.php?sta_mac=<?php echo $sta_mac;?>','','height=770, width=1000, top=60, left=150');">详细</a>
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
        <td  align="left" style="width: 15%;">
        <!--<input type="button" class="bt" onclick="" value="强制下线" />-->	
        <input class="bt" type="button"	onclick="javascript:window.location.reload(true);" value="刷新" />			
		</td>
		<td  align="right"><?php $formId='user_list';include PATH.'db/pageTemplate.php';?>				
		</td>
	</tr>
</table>

</form>
</body>
</html>