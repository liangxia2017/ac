<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";

function netId($ip1,$ip2){
    $rel = ip2long($ip1)&ip2long($ip2);
    return $rel;
}

//修改操作
if(isset($_POST["edit_id"]) && $_POST["edit_id"]!=""){
    if($_POST["start_ip"] == ""){
            echo "<script>alert('开始地址不能为空');location='ac_dhcp_server.php';</script>";
        }elseif($_POST["end_ip"] == ""){
            echo "<script>alert('结束地址不能为空');location='ac_dhcp_server.php';</script>";
        }elseif($_POST["gateway"] == ""){
            echo "<script>alert('网关地址不能为空');location='ac_dhcp_server.php';</script>";
        }elseif($_POST["mask"] == ""){
            echo "<script>alert('MASK地址不能为空');location='ac_dhcp_server.php';</script>";
        }else{
        $dbhelper = new DAL();
        $net_id1 = netId($_POST["start_ip"],$_POST["mask"]);
        $net_id2 = netId($_POST["end_ip"],$_POST["mask"]);
        $net_id3 = netId($_POST["gateway"],$_POST["mask"]);        
        $record = $dbhelper->getRow("select * from ac_network_config where net_id ='".$net_id1."'");
        if($net_id1 != $net_id2){
            echo "<script>alert('开始地址和结束地址不在同一网段');location='ac_dhcp_server.php?action=modify&id='+".$_POST["edit_id"].";</script>";
        }elseif($net_id1 != $net_id3){
            echo "<script>alert('网关地址和地址池不在同一网段');location='ac_dhcp_server.php?action=modify&id='+".$_POST["edit_id"].";</script>";
        }elseif($record == null | $record == ""){
            echo "<script>alert('无法找到设备接口地址与该池地址对应，请先在网络配置中添加接口地址');location='ac_dhcp_server.php?action=modify&id='+".$_POST["edit_id"].";</script>";
        }elseif($record->ip != $_POST["gateway"]){
            echo "<script>if(!confirm('网关没有指向网络配置中的接口地址，是否继续？'))location='ac_dhcp_server.php?action=modify&id='+".$_POST["edit_id"].";</script>";
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
        for($k = 0; $k < count($params); $k++){
        		$params[$k] = trim($params[$k]);
        	}
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
            echo "<script>alert('开始地址不能为空');location='ac_dhcp_server.php';</script>";
        }elseif($_POST["end_ip"] == ""){
            echo "<script>alert('结束地址不能为空');location='ac_dhcp_server.php';</script>";
        }elseif($_POST["gateway"] == ""){
            echo "<script>alert('网关地址不能为空');location='ac_dhcp_server.php';</script>";
        }elseif($_POST["mask"] == ""){
            echo "<script>alert('MASK地址不能为空');location='ac_dhcp_server.php';</script>";
    }else{
        $dbhelper = new DAL();
        $net_id1 = netId($_POST["start_ip"],$_POST["mask"]);
        $net_id2 = netId($_POST["end_ip"],$_POST["mask"]);
        $net_id3 = netId($_POST["gateway"],$_POST["mask"]); 
            $record = $dbhelper->getRow("select * from ac_network_config where net_id ='".$net_id1."'");
            if($net_id1 != $net_id2){
            echo "<script>alert('开始地址和结束地址不在同一网段');location='ac_dhcp_server.php';</script>";
            }elseif($net_id1 != $net_id3){
            echo "<script>alert('网关地址和地址池不在同一网段');location='ac_dhcp_server.php?action=modify&id='+".$_POST["edit_id"].";</script>";
            }elseif($record == null | $record == ""){
            echo "<script>alert('无法找到设备接口地址与该池地址对应，请先在网络配置中添加接口地址');location='ac_dhcp_server.php';</script>";
            }elseif($record->ip != $_POST["gateway"]){
            echo "<script>if(!confirm('该地址没有指向网络配置中的接口地址，是否继续？'))location='ac_dhcp_server.php';</script>";
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
        for($k = 0; $k < count($params); $k++){
        		$params[$k] = trim($params[$k]);
        	}
        $sql ="insert into ac_dhcp_server(start_ip,end_ip,mask,
            gateway,lease,dns,option43,option60,option82,net_id) values(?,?,?,?,?,?,?,?,?,?)";
        $insert = $dbhelper->insert($sql,$params);
   	    if($insert>0){
		  echo "<script>alert('添加成功');location='ac_dhcp_server.php?r='+Math.random();</script>";
    	}
    }
        }
    }

?>     