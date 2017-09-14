<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";
include PATH."db/page.php";
function netId($ip1,$ip2){
    $rel = ip2long($ip1)&ip2long($ip2);
    return $rel;
}
?>
<head>
<META HTTP-EQUIV="pragma"   CONTENT="no-cache">         
<META   HTTP-EQUIV="Cache-Control"   CONTENT="no-cache,   must-revalidate">         
<META   HTTP-EQUIV="expires"   CONTENT="0"> 
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script src="<?php echo PATH ?>js/jquery.js" type="text/javascript"></script>
<script src="<?php echo PATH ?>js/jquery.validate.js" type="text/javascript"></script>
<script src="<?php echo PATH ?>js/checkbox.js" type="text/javascript"></script>
<link rel="stylesheet" href="<?php echo PATH ?>css/body.css" type="text/css" />
<title>DHCP服务器</title>
</head>

<style type="text/css">
<!--

.acinfo_table{
	background:#a8c7ce;
/*	width:100%;*/
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

.tdRegist{
    background-color:#F3F8F7;
    padding:5px;
	text-align:right;
    color:#73938E;
    font-weight:bold;
    width: 20%;
}

-->
</style>

<?php
//查询操作
$selectSql='select * from ac_dhcp_server order by id';
$countSql='select count(*) from ac_dhcp_server';
$page = new Page($selectSql,$_POST["pageNow"],$countSql);
?>


<?php
if(!isset($_GET["apply"])){
if(isset($_GET["id"])|isset($_POST["id"])){
    if(isset($_GET["action"])){
	   $dbhelper = new DAL();	
	   $getRecord = $dbhelper->getRow("select * from ac_dhcp_server where id =".$_GET['id']);
       $getRecord = (array)$getRecord;
    }
    //删除操作
    else{
	   $dbhelper = new DAL();
	   $group_id="0";	
	   if(isset($_GET["id"])){
	       $group_id=$group_id.",".$_GET["id"];
           }else{
		foreach ($_POST["id"] as $v){
			$group_id=$group_id.",".$v;
		  }
	   }
	   $delete = $dbhelper->delete("delete from ac_dhcp_server where id in (".$group_id.")");
    if($delete>0){
		echo "<script>alert('删除成功');location='ac_dhcp_server.php?r='+Math.random();</script>";
	   }
    }
}
}
?>

<?php
if(isset($_GET["add"]) && $_GET["add"] != ""){
if(isset($_POST["edit_id"]) && $_POST["edit_id"]!=""){    
    if($_POST["start_ip"] == ""){
            echo "<script>alert('开始地址不能为空');</script>";
        }elseif($_POST["end_ip"] == ""){
            echo "<script>alert('结束地址不能为空');</script>";
        }elseif($_POST["gateway"] == ""){
            echo "<script>alert('网关地址不能为空');</script>";
        }elseif($_POST["mask"] == ""){
            echo "<script>alert('MASK地址不能为空');</script>";
        }else{
        $dbhelper = new DAL();
        $net_id1 = netId($_POST["start_ip"],$_POST["mask"]);
        $net_id2 = netId($_POST["end_ip"],$_POST["mask"]);
        $net_id3 = netId($_POST["gateway"],$_POST["mask"]);        
        $record = $dbhelper->getRow("select * from ac_network_config where net_id ='".$net_id1."'");
        if($net_id1 != $net_id2){
            echo "<script>alert('开始地址和结束地址不在同一网段');</script>";
        }elseif($net_id1 != $net_id3){
            echo "<script>alert('网关地址和地址池不在同一网段');</script>";
        }elseif($record == null | $record == ""){
            echo "<script>alert('无法找到设备接口地址与该池地址对应，请先在网络配置中添加接口地址');</script>";
       }elseif($record->ip != $_POST["gateway"]){
            echo "<script>alert('网关没有指向网络配置中的接口地址!');location='ac_dhcp_server.php?id=".$_POST["edit_id"]."&action=modify';</script>";
        //}else{
/*            if($_POST["option"] == 0){
                $option43 = $_POST["option43"];
                $option60 = $_POST["option60"];
                $option82 = '';
            }elseif($_POST["option"] == 1){
                $option43 = '';
                $option60 = '';
                $option82 = $_POST["option82"];
            }            
            $params = array($_POST["start_ip"],$_POST["end_ip"],$_POST["mask"],
                $_POST["gateway"],$_POST["lease"],$_POST["dns"],$option43,$option60,$option82,$net_id1,$_POST["edit_id"]);
            $sql ="update ac_dhcp_server set start_ip=?,end_ip=?,mask=?,
            gateway=?,lease=?,dns=?,option43=?,option60=?,option82=?,net_id=? where id=?";
            $update = $dbhelper->update($sql,$params);
       	    if($update>0){
    		  echo "<script>alert('修改成功');location='ac_dhcp_server.php?r='+Math.random();</script>";
        	}*/
        }else{
            if($_POST["option"] == 0){
                $option43 = $_POST["option43"];
                $option60 = $_POST["option60"];
                $option82 = '';
            }elseif($_POST["option"] == 1){
                $option43 = '';
                $option60 = '';
                $option82 = $_POST["option82"];
            }            
            $params = array($_POST["start_ip"],$_POST["end_ip"],$_POST["mask"],
                $_POST["gateway"],$_POST["lease"],$_POST["dns"],$option43,$option60,$option82,$net_id1,$_POST["edit_id"]);
            $sql ="update ac_dhcp_server set start_ip=?,end_ip=?,mask=?,
            gateway=?,lease=?,dns=?,option43=?,option60=?,option82=?,net_id=? where id=?";
            $update = $dbhelper->update($sql,$params);
       	    if($update>0){
    		  echo "<script>alert('修改成功');location='ac_dhcp_server.php?r='+Math.random();</script>";
        	}
        }
        }
    }else{
        if($_POST["start_ip"] == ""){
            echo "<script>alert('开始地址不能为空');</script>";
        }elseif($_POST["end_ip"] == ""){
            echo "<script>alert('结束地址不能为空');</script>";
        }elseif($_POST["gateway"] == ""){
            echo "<script>alert('网关地址不能为空');</script>";
        }elseif($_POST["mask"] == ""){
            echo "<script>alert('MASK地址不能为空');</script>";
        }else{
            $dbhelper = new DAL();
            $net_id1 = netId($_POST["start_ip"],$_POST["mask"]);
            $net_id2 = netId($_POST["end_ip"],$_POST["mask"]);
            $net_id3 = netId($_POST["gateway"],$_POST["mask"]); 
            $record = $dbhelper->getRow("select * from ac_network_config where net_id ='".$net_id1."'");
            if($net_id1 != $net_id2){
                echo "<script>alert('开始地址和结束地址不在同一网段');</script>";
            }elseif($net_id1 != $net_id3){
                echo "<script>alert('网关地址和地址池不在同一网段');</script>";
            }elseif($record == null | $record == ""){
                echo "<script>alert('无法找到设备接口地址与该池地址对应，请先在网络配置中添加接口地址');</script>";
            }elseif($record->ip != $_POST["gateway"]){
                echo "<script>alert('网关没有指向网络配置中的接口地址!');location='ac_dhcp_server.php?id=".$_POST["edit_id"]."&action=modify';</script>";
            //}else{
/*            if($_POST["option"] == 0){
                $option43 = $_POST["option43"];
                $option60 = $_POST["option60"];
                $option82 = '';
            }elseif($_POST["option"] == 1){
                $option43 = '';
                $option60 = '';
                $option82 = $_POST["option82"];
            }
            $params = array($_POST["start_ip"],$_POST["end_ip"],$_POST["mask"],
                $_POST["gateway"],$_POST["lease"],$_POST["dns"],$option43,$option60,$option82,$net_id1);
            $sql ="insert into ac_dhcp_server(start_ip,end_ip,mask,
            gateway,lease,dns,option43,option60,option82,net_id) values(?,?,?,?,?,?,?,?,?,?)";
            $insert = $dbhelper->insert($sql,$params);
       	    if($insert>0){
    		  echo "<script>alert('添加成功');location='ac_dhcp_server.php?r='+Math.random();</script>";
        	}*/
            }else{
            if($_POST["option"] == 0){
                $option43 = $_POST["option43"];
                $option60 = $_POST["option60"];
                $option82 = '';
            }elseif($_POST["option"] == 1){
                $option43 = '';
                $option60 = '';
                $option82 = $_POST["option82"];
            }
            $params = array($_POST["start_ip"],$_POST["end_ip"],$_POST["mask"],
                $_POST["gateway"],$_POST["lease"],$_POST["dns"],$option43,$option60,$option82,$net_id1);
            $sql ="insert into ac_dhcp_server(start_ip,end_ip,mask,
            gateway,lease,dns,option43,option60,option82,net_id) values(?,?,?,?,?,?,?,?,?,?)";
            $insert = $dbhelper->insert($sql,$params);
       	    if($insert>0){
    		  echo "<script>alert('添加成功');location='ac_dhcp_server.php?r='+Math.random();</script>";
        	}
            }
        }
    }
    $getRecord = $_POST;
}
?>


<script type="text/javascript"> 
	$(document).ready(function(){
	   $("#edit_ac_dhcp_server").validate({
			rules:{
				"start_ip":{
					required:true,
					ipcheck:true
				},
				"end_ip":{
					required:true,
					ipcheck:true
				},
				"lease":{
					required:true,
					intNumber:true
				},
				"gateway":{
					required:true,
					ipcheck:true
				}
			}
		});        
	   $("#edit_id").val("<?php if($getRecord["id"] == ""){echo $getRecord["edit_id"];}else{echo $getRecord["id"];}?>");
       $("#gateway").val("<?php echo $getRecord["gateway"];?>");
       $("#start_ip").val("<?php echo $getRecord["start_ip"];?>");
       $("#end_ip").val("<?php echo $getRecord["end_ip"];?>");
       $("#mask").val("<?php echo $getRecord["mask"];?>");
       $("#lease").val("<?php echo $getRecord["lease"];?>");
       $("#dns").val("<?php echo $getRecord["dns"];?>");
       $("#option43").val("<?php echo $getRecord["option43"];?>");
       $("#option60").val("<?php echo $getRecord["option60"];?>");
       $("#option82").val("<?php echo $getRecord["option82"];?>");
       <?php
        if($getRecord["option82"] == "" | $getRecord["option82"] == null){
            echo "$('#option').val('0');";
            echo "$('#td_option82').attr('style','display: none');";
        }else{
            echo "$('#option').val('1');";
            echo "$('#option43_60').attr('style','display: none');";
            echo "$('#td_option82').attr('style','display: ');";
        }
       ?>
  $("#stripe tr").mouseover(function(){
	$(this).addClass("over");
	});
	$("#stripe tr").mouseout(function(){
	$(this).removeClass("over");
	});       
	});

    function change_option(t){
        if(t.value == 1){
            document.getElementById('option43_60').style.display = 'none';
            document.getElementById('td_option82').style.display = '';
        }else{
            document.getElementById('option43_60').style.display = '';
            document.getElementById('td_option82').style.display = 'none';
        }
    }
    
    function add(t,action){
    	t.form.action=action;
    	t.form.submit();	
    }
    
</script>
</head>
<body>
<br />
<form action="ac_dhcp_server.php?add=add" id="edit_ac_dhcp_server" name="edit_ac_dhcp_server" method="post">
<input type="hidden" name="edit_id" id="edit_id" value=""/>
<table cellpadding="0px" cellspacing="0px" width="70%">
	  <tr>   
        <td class="tdRegist">起始地址</td>
        <td align="left" ><label>
          <input type="text" name="start_ip" id="start_ip"/>
        </label></td>      
        <td class="tdRegist">结束地址</td>
        <td align="left"><label>
          <input type="text" name="end_ip" id="end_ip"/>
        </label></td>
        </tr>
	  <tr>
        <td class="tdRegist">MASK</td>
        <td align="left"><label>
          <input type="text" name="mask" id="mask"/>
        </label></td>      
        <td class="tdRegist">租期(≥300s)</td>
        <td align="left"><label>
          <input type="text" name="lease" id="lease"/>
        </label></td>
      </tr>
	  <tr>
        <td class="tdRegist">网关</td>
        <td align="left"><label>
          <input type="text" name="gateway" id="gateway"/>
        </label></td>      
        <td class="tdRegist">DNS</td>
        <td align="left"><label>
          <input type="text" name="dns" id="dns"/>
        </label></td>
      </tr> 
      <tr>
        <td class="tdRegist">
            <select style="color: #73938E; font-weight: bold; background: #F3F8F7;" id="option" name="option" onchange="change_option(this)">
                <option value="0">option43/60</option>
                <option value="1">option82</option>
            </select>
        </td>
       </tr>
       <tr id="option43_60">
        <td class="tdRegist">OPTION43</td>
        <td align="left"><label>
          <input type="text" name="option43" id="option43"/>
        </label></td>
        
        <td class="tdRegist">OPTION60</td>
        <td align="left"><label>
          <input type="text" name="option60" id="option60"/>
        </label></td>
        </tr>
       <tr id="td_option82">
        <td class="tdRegist">OPTION82</td>
            <td align="left"><label>
              <input type="text" name="option82" id="option82"/>
            </label></td>
       </tr>
    <tr>
        <td colspan="4"><input class="bt" type="button" onclick="validate(this)" value="确定" />&nbsp;&nbsp;<input class="bt" type="reset" value="重置"/></td>
    </tr>
</table>
</form>
<hr />
<form name="ac_dhcp_server" id="ac_dhcp_server" method="post">
	<table align="center" width="90%">
      <tr>
      <td colspan="2">
        <table  width="100%" class="acinfo_table" id="stripe" cellpadding="1px" cellspacing="1px">
        <tr>
        <td class="tdHeader"><input type="checkbox"	onclick="select_all(this,'id[]')" /></td>
        <td class="tdHeader">序号</td>
        <td class="tdHeader">起始地址</td>
        <td class="tdHeader">结束地址</td>
        <td class="tdHeader">掩码</td>
        <td class="tdHeader">租期</td>
        <td class="tdHeader">网关</td>
        <td class="tdHeader">DNS</td>
        <td class="tdHeader">OPTION43</td>
        <td class="tdHeader">OPTION60</td>
        <td class="tdHeader">OPTION82</td>
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
        <td class="tdContent"><?php echo $rs["start_ip"];?>
        </td>
        <td class="tdContent"><?php echo $rs["end_ip"];?>
        </td>
        <td class="tdContent"><?php echo $rs["mask"];?>
        </td>
        <td class="tdContent"><?php echo $rs["lease"];?>
        </td>
        <td class="tdContent"><?php echo $rs["gateway"];?>
        </td>
        <td class="tdContent"><?php echo $rs["dns"];?>
        </td>
        <td class="tdContent"><?php echo $rs["option43"];?>
        </td>
        <td class="tdContent"><?php echo $rs["option60"];?>
        </td>
        <td class="tdContent"><?php echo $rs["option82"];?>
        </td>
        <td class="tdContent">
        <a href="#" onclick="this.href='ac_dhcp_server.php?id=<?php echo $rs["id"];?>&action=modify'">修改</a>|
        <a href="#" onclick="if(confirm('您确定删除？')) this.href='ac_dhcp_server.php?id=<?php echo $rs["id"];?>'">删除</a>
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
        <td class="tdContentF9"><?php echo $rs["start_ip"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["end_ip"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["mask"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["lease"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["gateway"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["dns"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["option43"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["option60"];?>
        </td>
        <td class="tdContentF9"><?php echo $rs["option82"];?>
        </td>
        <td class="tdContentF9">
        <a href="#" onclick="this.href='ac_dhcp_server.php?id=<?php echo $rs["id"];?>&action=modify'">修改</a>|
        <a href="#" onclick="if(confirm('您确定删除？')) this.href='ac_dhcp_server.php?id=<?php echo $rs["id"];?>'">删除</a>
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
		<td width="15%">
			<div align="left">
                <input class="bt" type="button" id="1" onclick="delete_all(this,'id[]','ac_dhcp_server.php')" value="删除" />
                <input class="bt" type="button"	onclick="add(this,'ac_dhcp_server.php?apply=apply')" id="apply" value="服务应用" />
            </div>
		</td>
		<td  align="right" width="85%"><?php $formId='ac_dhcp_server';include PATH.'db/pageTemplate.php';?>				
		</td>
	</tr>
</table>
</form>
</body>
<script type="text/javascript">
<?php
if(isset($_GET["apply"])){
?>
var tag = document.getElementById("apply");
var j = 5;
 function apply(t){ 
    if(j == 5){
        <?php
            exec('cd /ac/script/ && ./init_dhcp_pool_scr',$arr,$retval);
            if($retval != 0){
                echo "alert('应用失败');return false;";
            }
        ?>
    }        
    t.disabled = "disabled";
    t.value = "应用中...";
    if(j>0)
		setTimeout(function(){j--;apply(t);},1000);
    else{
    		alert('应用成功!');
        location='ac_dhcp_server.php?r='+Math.random();
 } 
 }
 apply(tag);
<?php
     }
?>
  
    var option = document.getElementById("option"); 
    var option43 = document.getElementById("option43");
    var option60 = document.getElementById("option60");
    var option82 = document.getElementById("option82");
    function validate(t){
        if(option.value == 0){
        if(option43.value != ""){
            var re = /^[0-9a-fA-F]{2}(:[0-9a-fA-F]{2}){5}|(:[0-9a-fA-F]{2}){9}$/;
            if(!re.test(option43.value)){
                alert("option43是以英文':'作为分隔符的16进制值(6位或者10位)");
                return false;
                }
        }
        if(option60.value != ""){
            var re = /^[0-9a-fA-F]{2}:[0-9a-fA-F]{2}$/;
            if(!re.test(option60.value)){
                alert("option60是以英文':'作为分隔符的16进制值(2位)");
                return false;
                }
        }
        }else if(option.value == 1){
            if(option82.value != ""){
                var re = /^[0-9a-fA-F]{2}(:[0-9a-fA-F]{2})*$/;
                if(!re.test(option82.value)){
                    alert("option82是以英文':'作为分隔符的16进制值");
                    return false;
                    }
            }
        }
        t.form.submit();
    }
 
</script>
</html>