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
<title>AP导入及统计</title>
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

#ap_upgrade{
    border:1px solid #369;
    background:#e2ecf5;
    width: 750px;
    height: 450px;
    /*z-index:1000;*/
    position:absolute;
    display:none;
    overflow: auto;
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
    
    <?php 
        if(isset($_POST["query"])){
            if($_POST["query"] != 2){                
    ?>
    $("#query").val("<?php echo $_POST["query"];?>");
    $("#mac_or_ip").val("<?php echo $_POST["mac_or_ip"];?>");
    <?php            
                } 
            if($_POST["query"] == 2){
     ?>
    $("#query").val(2);
    $("#status").val("<?php echo $_POST["status"];?>");
    document.getElementById("mac_or_ip").style.display = "none";
    document.getElementById("status").style.display = "";
    <?php             
                 }   
        }
    ?>
    
	});
    function rel(){
        window.location.href = window.location.href;
        window.location.reload();
    }
    
    function upgrade(cid,fid,url){
        document.getElementById("cid").value = cid;
        document.getElementById("fid").value = fid;
        document.getElementById("url").value = url;
        var arrObj = document.getElementsByName(cid+"[]");
		var count = 0;
		for ( var i = 0; i < arrObj.length; i++) {
			if(arrObj[i].checked==true){
				count++;
			}
		}
		if(count==0){
			alert("你还没有选择要操作的项!");
            return;
		}
        var ap_upgrade = document.getElementById("ap_upgrade");
        var mybg = document.getElementById("mybg");
        ap_upgrade.style.display = "block";
        ap_upgrade.style.position = "absolute";
        ap_upgrade.style.left = "15%";
        ap_upgrade.style.top = "15%";
        mybg.style.display = "";        
    }
    
    function conf(){
        var obj = $("input:radio:checked").val();
    	if(obj != null){
    	   var cid = document.getElementById("cid").value;
           var fid = document.getElementById("fid").value;
           var url = document.getElementById("url").value + "&version=" + obj;
           //alert(cid);alert(fid);alert(url);
           opt_all1(cid,fid,url);
    	}else{
    		alert("请选择升级版本！");
            return;
        }        
    }
    
    
    function sub(cid,fid,url){
        var all = $("input[id='"+cid+"']");
		var count = 0;
		for ( var i = 0; i < all.length; i++) {
			if(all[i].checked==true){
				count++;
			}
		}
		if(count==0){
			alert("你还没有选择要操作的项!");
		}else{
			if(confirm("你确定要操作这些数据吗?")){
				$("#"+fid).attr("action",url);
				$("#"+fid).submit();
			}
		}
    }
 
    
    
    function clo(){
        ap_upgrade.style.display = "none";
        mybg.style.display = "none";  
    }
    
    
    	function opt_all1(cid,fid,url){
		var all = $("input[id='"+cid+"']");
		var count = 0;
		for ( var i = 0; i < all.length; i++) {
			if(all[i].checked==true){
				count++;
			}
		}
		if(count==0){
			alert("你还没有选择要操作的项!");
		}else{
				$("#"+fid).attr("action",url);
				$("#"+fid).submit();			
		}
	}
    
    
    function apply_all(){
        if(confirm("此操作将对该ap分组下所有ap下发信道等配置，一般用在下发ap恢复默认的十分钟后使用，是否继续？")){
            location='ap_info_apply_all.php?ap_group_name=<?php echo $_GET["ap_group_name"];?>';
        }
    }    
</script>
<?php
//删除操作
if(isset($_POST["id"]) && !isset($_GET["id"])){
	$dbhelper = new DAL();
	$group_id="0";
		foreach ($_POST["id"] as $v){
			$group_id=$group_id.",".$v;
		}		
	$delete = $dbhelper->delete("delete from ap_info where id in (".$group_id.")");
	if($delete>0){
		echo "<script>alert('删除成功');location='ap_info.php?ap_group_name=".$_GET["ap_group_name"]."';</script>";
	}
}
if(isset($_GET["id"])){
	$dbhelper = new DAL();
    $ap_remark = "ap_remark".$_GET["id"];
    $bg_channel = "bg_channel".$_GET["id"];
    $a_channel = "a_channel".$_GET["id"];
    
     //------msg--------
    $head = pack("n","4096");
    $sub_id = pack("C","3");
    $value = pack("C",$_POST[$bg_channel]);
    $value = $value.pack("C",$_POST[$a_channel]);
    $subid_len = pack("C",strlen($value));
    $body = $sub_id.$subid_len.$value;
    $len = pack("n",strlen($body));
    $msg = $head.$len.$body;
    $msg = unpack("H*",$msg);
    //------msg--------
    $params=array($_POST[$ap_remark],$_POST[$bg_channel],$_POST[$a_channel],$_GET["id"]);
    $sql = "update ap_info set ap_remark=?,bg_channel=?,a_channel=?,config_status=x'".$msg[1]."' where id=?";
	$update = $dbhelper->update($sql,$params);
	if($update>0){
		echo "<script>alert('配置成功!');location='ap_info.php?ap_group_name=".$_GET["ap_group_name"]."';</script>";
	}
}
//查询操作
if(isset($_POST["query"])){
    if($_POST["query"] == 0){
        $mac = $_POST["mac_or_ip"];
        $mac = preg_replace("/:|：| /","",$mac);
        $selectSql="select * from ap_info where ap_group_name='".$_GET["ap_group_name"]."' and ap_mac like '%".$mac."%'";
        $countSql="select count(*) from ap_info where ap_group_name='".$_GET["ap_group_name"]."' and ap_mac like '%".$mac."%'";
    } 
    if($_POST["query"] == 1){
        $ip = trim($_POST["mac_or_ip"]);
        $selectSql="select * from ap_info where ap_group_name='".$_GET["ap_group_name"]."' and ap_ip like '%".$ip."%'";
        $countSql="select count(*) from ap_info where ap_group_name='".$_GET["ap_group_name"]."' and ap_ip like '%".$ip."%'";
    }
    if($_POST["query"] == 2){
        $selectSql="select * from ap_info where ap_group_name='".$_GET["ap_group_name"]."' and status=".$_POST["status"];
        $countSql="select count(*) from ap_info where ap_group_name='".$_GET["ap_group_name"]."' and status=".$_POST["status"];
    }
    if($_POST["query"] == 3){
        $selectSql="select * from ap_info where ap_group_name='".$_GET["ap_group_name"]."' and ap_remark like '%".$_POST["mac_or_ip"]."%'";
        $countSql="select count(*) from ap_info where ap_group_name='".$_GET["ap_group_name"]."' and ap_remark like '%".$_POST["mac_or_ip"]."%'";
    }
    if($_POST["query"] == 4){
        $selectSql="select * from ap_info where ap_group_name='".$_GET["ap_group_name"]."' and soft_ver like '%".$_POST["mac_or_ip"]."%'";
        $countSql="select count(*) from ap_info where ap_group_name='".$_GET["ap_group_name"]."' and soft_ver like '%".$_POST["mac_or_ip"]."%'";
    }       
}else{
$selectSql="select * from ap_info where ap_group_name='".$_GET["ap_group_name"]."'";
$countSql="select count(*) from ap_info where ap_group_name='".$_GET["ap_group_name"]."'";
}
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
?>
</head>
<script>
	function add(t,action){
	t.form.action=action;
	t.form.submit();	
	}
    
    function query_met(t){
        if(t.value == 2){
            t.form.mac_or_ip.style.display = 'none';
            t.form.status.style.display = '';
        }else{
            t.form.mac_or_ip.style.display = '';
            t.form.status.style.display = 'none';
        }
    }
    function add(t,action){
    	t.form.action=action;
            t.form.submit();
        }
    
</script>
<body>
<form name="ap_info" id="ap_info" method="post">
<div id="mybg" style="background: #000; width:100%; height:100%; position:absolute; top:0; left:0; opacity: 0.3; filter:Alpha(opacity=30); display: none; " >
</div>
<input type="hidden" name="group" value="<?php echo $_GET["ap_group_name"];?>" />
<div style="float: left;">
查询方式:<select name="query" id="query" style="margin: 0 5px;" onchange="javascript:query_met(this);" >
    <option value="0">MAC地址</option>
    <option value="1">IP地址</option>
    <option value="2">状态</option>
    <option value="3">备注</option>
    <option value="4">软件版本</option>
</select>
<input type="text" id="mac_or_ip" name="mac_or_ip" style="margin-right: 5px;" />
<select name="status" id="status" style="display: none; margin-right: 5px;" >
    <option value="0">未加入</option>
    <option value="1">up</option>
    <option value="2">idle</option>
</select>
<input type="button" class="bt" onclick="add(this,'ap_info.php?ap_group_name=<?php echo $_GET["ap_group_name"];?>')" value="确定" />
</div>
<div style="text-align: right;">
<input type="button" value="全部应用" onclick="apply_all()" class="bt" />
</div>
	<table align="center" width="100%">
      <tr>
      <td colspan="2">
        <table class="acinfo_table"  id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	onclick="select_all(this,'id[]')" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">MAC地址</td>
        <td class="tdHeader">IP地址</td>
        <td class="tdHeader">备注信息</td>
        <td class="tdHeader">状态</td>
        <td class="tdHeader">加入时间</td>
        <td class="tdHeader">bg卡信道</td>
        <td class="tdHeader">a卡信道</td>
        <td class="tdHeader">详细</td>
        <td class="tdHeader">操作</td>
      </tr>
      <?php
            $apinfo = $page->getResult();
			$i=0;
			foreach ($apinfo as $rs){
				$rs = (array)$rs;
				$i++;
                if($i%2 == 0){
	   ?>
      <tr>
        <td class="tdContent"><input type="checkbox" name="id[]" id="id" value="<?php echo $rs["id"];?>" />
		</td>
        <td class="tdContent"><?php echo $i;?>
        </td>
        <td class="tdContent">
        <?php 
         $ap_mac = $rs["ap_mac"];
         $mac = "";
         for($k = 0; $k<strlen($ap_mac); $k++){
            if($k != 0 && $k%2 == 0)
                $mac = $mac.":".$ap_mac[$k];
            else
                $mac = $mac.$ap_mac[$k];
            }
         echo $mac;
        ?>
        </td>
        <td class="tdContent"><?php echo $rs["ap_ip"];?>        
        </td>
        <td class="tdContent"><input type="text" name="ap_remark<?php echo $rs["id"];?>" id="ap_remark" value="<?php echo $rs["ap_remark"];?>" />
        </td>
        <td class="tdContent">
        <?php 
        //0-从未加入，1-up，2-idle
            if($rs["status"] == 0)
                echo "未加入";
            elseif($rs["status"] == 1)
                echo "在线";
            else
                echo "离线";
        ?>
        </td>        
        <td class="tdContent"><?php echo $rs["last_join_time"];?>
        </td>
        <td class="tdContent">
            <select name="bg_channel<?php echo $rs["id"];?>">
            <?php 
            //bg 1-13
                $bg_channel = array_combine(range(1,13),range(1,13));
                foreach($bg_channel as $bg)
                if($rs["bg_channel"] == $bg)
                    echo "<option value='".$bg."' selected='selected'>".$bg."</option>";
                else
                    echo "<option value='".$bg."'>".$bg."</option>";
            ?>
            </select>
        </td>
        <td class="tdContent">
            <select name="a_channel<?php echo $rs["id"];?>">
            <?php 
            //a 36-64,149-165
                $temp = array_merge(range(36,64,4),range(149,165,4));
                $a_channel = array_combine($temp,$temp);
                foreach($a_channel as $a)
                {               	
                	if($rs["a_channel"] == $a)
                    echo "<option value='".$a."' selected='selected'>".$a."</option>";
               	 else
                    echo "<option value='".$a."'>".$a."</option>";
                }
                if($rs["a_channel"] == 1)
              		echo "<option value='1' selected='selected'>关5.1g</option>";
              	else
                  echo "<option value='1'>关5.1g</option>";
                if($rs["a_channel"] == 2)
              		echo "<option value='2' selected='selected'>开5.1g</option>";
              	else
                  echo "<option value='2'>开5.1g</option>";
            ?>
            </select>
        </td>
        <td class="tdContent">
        <a href="#" onclick="window.open('detail.php?ap_mac=<?php echo $ap_mac;?>','','height=770, width=1000, top=60, left=150');">详细</a>
        </td>
        <td class="tdContent">
        <input type="button" class="bt" onclick="add(this,'ap_info.php?id=<?php echo $rs["id"];?>&ap_group_name=<?php echo $_GET["ap_group_name"];?>')" value="应用" />
        </td>
      </tr>
      <?php
			}else{
		?>
        <tr>
        <td class="tdContentF9"><input type="checkbox" name="id[]" id="id" value="<?php echo $rs["id"];?>" />
		</td>
        <td class="tdContentF9"><?php echo $i;?>
        </td>
        <td class="tdContentF9">
        <?php 
         $ap_mac = $rs["ap_mac"];
         $mac = "";
        for($k = 0; $k<strlen($ap_mac); $k++){
            if($k != 0 && $k%2 == 0)
                $mac = $mac.":".$ap_mac[$k];
            else
                $mac = $mac.$ap_mac[$k];
        }
         echo $mac;
        ?>
        </td>
        <td class="tdContentF9"><?php echo $rs["ap_ip"];?>        
        </td>
        <td class="tdContentF9"><input type="text" name="ap_remark<?php echo $rs["id"];?>" id="ap_remark" value="<?php echo $rs["ap_remark"];?>" />
        </td>
        <td class="tdContentF9">
        <?php 
        //0-从未加入，1-up，2-idle
            if($rs["status"] == 0)
                echo "未加入";
            elseif($rs["status"] == 1)
                echo "在线";
            else
                echo "离线";
        ?>
        </td>        
        <td class="tdContentF9"><?php echo $rs["last_join_time"];?>
        </td>
        <td class="tdContentF9">
            <select name="bg_channel<?php echo $rs["id"];?>">
            <?php
            //bg 1-13
                $bg_channel = array_combine(range(1,13),range(1,13));
                foreach($bg_channel as $bg)
                if($rs["bg_channel"] == $bg)
                    echo "<option value='".$bg."' selected='selected'>".$bg."</option>";
                else
                    echo "<option value='".$bg."'>".$bg."</option>";
            ?>
            </select>
        </td>
        <td class="tdContentF9">
            <select name="a_channel<?php echo $rs["id"];?>">
            <?php
            //a 36-64,149-165
                $temp = array_merge(range(36,64,4),range(149,165,4));
                $a_channel = array_combine($temp,$temp);
                foreach($a_channel as $a)
                if($rs["a_channel"] == $a)
                    echo "<option value='".$a."' selected='selected'>".$a."</option>";
                else
                    echo "<option value='".$a."'>".$a."</option>";
                    
                if($rs["a_channel"] == 1)
              		echo "<option value='1' selected='selected'>关5.1g</option>";
              	else
                  echo "<option value='1'>关5.1g</option>";
                if($rs["a_channel"] == 2)
              		echo "<option value='2' selected='selected'>开5.1g</option>";
              	else
                  echo "<option value='2'>开5.1g</option>";
            ?>
            </select>
        </td>
        <td class="tdContentF9">
        <a href="#" onclick="window.open('detail.php?ap_mac=<?php echo $ap_mac;?>','','height=770, width=1000, top=60, left=150');">详细</a>
        </td>
        <td class="tdContentF9">
        <input type="button" class="bt" onclick="add(this,'ap_info.php?id=<?php echo $rs["id"];?>&ap_group_name=<?php echo $_GET["ap_group_name"];?>')" value="应用" />
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
		<td align="left" width="20%">
                <input class="bt" type="button"	onclick="delete_all(this,'id[]','ap_info.php?ap_group_name=<?php echo $_GET["ap_group_name"];?>')" value="删除" />
                <input class="bt" type="button"	onclick="location='add_ap_info.php?ap_group_name=<?php echo $_GET["ap_group_name"];?>'" value="添加" />
                <input class="bt" type="button"	onclick="location='../ap_group_config/ap_group_config.php'" value="返回" />
                <input class="bt" type="button"	onclick="window.location.reload()" value="刷新" /><br />
                <input class="bt" type="button"	onclick="if(confirm('确定要对所选AP执行重启操作吗？该操作需要较长时间')){ opt_all1('id','ap_info','ap_msg.php?tag=0');}else{return;}" value="重启" />
                <input class="bt" type="button"	onclick="upgrade('id','ap_info','ap_msg.php?tag=2')" value="升级" />
                <input class="bt" type="button"	onclick="if(confirm('确定要对所选AP执行恢复出厂操作吗？该操作需要较长时间')){ opt_all1('id','ap_info','ap_msg.php?tag=1');}else{return;}" value="恢复出厂" />
		</td>
		<td  align="right" width="80%"><?php $formId='ap_info';include PATH.'db/pageTemplate.php';?>				
		</td>
	</tr>
</table>
</form>

<div id="ap_upgrade">
<form id="upgrade"><br />
<input type="hidden" id="cid" />
<input type="hidden" id="fid" />
<input type="hidden" id="url" />
<table align="center" width="90%">
<tr>
<td height="350px" valign="top">
<table width="100%">
      <tr>
      <td>
        <table id="stripe" class="acinfo_table" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	disabled="disabled" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">AP版本号</td>
        <td class="tdHeader">AP版本名称</td>
        <td class="tdHeader"><input type="checkbox"	disabled="disabled" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">AP版本号</td>
        <td class="tdHeader">AP版本名称</td>
      </tr>
      <?php 
            $dbhelper = new DAL();
			$result1 = $dbhelper->getall("select * from ap_upgrade_config");
            $len = 	count($result1);	
			for($j=0;$j<$len;$j++){
                if($j%4 == 0){
	   ?>
      <tr>
        <td class="tdContent"><input type="radio" name="version" value="<?php echo ($result1[$j]["img_name"]);?>" />
		</td>
        <td class="tdContent"><?php echo $j+1;?>
        </td>
        <td class="tdContent"><?php echo $result1[$j]["img_version"];?>
        </td>
        <td class="tdContent"><?php echo $result1[$j]["img_name"];?>
        </td>
        <?php $j++;if($j>=$len) break;?>
        <td class="tdContent"><input type="radio" name="version" value="<?php echo ($result1[$j]["img_name"]);?>" />
		</td>
        <td class="tdContent"><?php echo $j+1;?>
        </td>
        <td class="tdContent"><?php echo $result1[$j]["img_version"];?>
        </td>
        <td class="tdContent"><?php echo $result1[$j]["img_name"];?>
        </td>
      </tr>
      <?php
			}else{
		?>
        <tr>
        <td class="tdContentF9"><input type="radio" name="version" value="<?php echo ($result1[$j]["img_name"]);?>" />
		</td>
        <td class="tdContentF9"><?php echo $j+1;?>
        </td>
        <td class="tdContentF9"><?php echo $result1[$j]["img_version"];?>
        </td>
        <td class="tdContentF9"><?php echo $result1[$j]["img_name"];?>
        </td>
        <?php $j++;if($j>=$len) break;?>
        <td class="tdContentF9"><input type="radio" name="version" value="<?php echo ($result1[$j]["img_name"]);?>" />
		</td>
        <td class="tdContentF9"><?php echo $j+1;?>
        </td>
        <td class="tdContentF9"><?php echo $result1[$j]["img_version"];?>
        </td>
        <td class="tdContentF9"><?php echo $result1[$j]["img_name"];?>
        </td>
      </tr>
        <?php	 
			}
            }
	?>
    </table>
    </td>
    </tr>       
</table>
</td>
</tr>
<tr>
    <td height="10%" valign="bottom">
        <input type="button" class="bt" onclick="if(confirm('确定要对所选AP执行升级操作吗？该操作需要较长时间')){conf();}else{return;}"  value="确定" />&nbsp;&nbsp;
        <input type="button" class="bt" onclick="clo()"  value="取消" />				
    </td>
</tr> 
</table>
</form>
</div>
</body>
</html>