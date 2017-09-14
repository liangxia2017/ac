<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";

    $dbhelper = new DAL();
    $id = $_GET["id"];
    $flag = $_GET["flag"];
    
    $wlan_group = "wlan_group_name".$id;
    $wireless_group = "wireless_group_name".$id;
    $function_group = "function_group_name".$id;
    
    $ap_group_name = $dbhelper->getOne("select ap_group_name from group_relation where id = ".$id);
    $config_mask = $dbhelper->getOne("select config_mask from ap_info where ap_group_name = '".$ap_group_name."' order by id limit 1");
    if($config_mask == null || $config_mask == "")
        $config_mask = 0;
        
    switch ($flag) {
    case 1:
        if($_POST[$wlan_group] == null || $_POST[$wlan_group] == "")
            $config_mask = $config_mask & 6;
        else
            $config_mask = $config_mask | $flag;
        $dbhelper->update("update group_relation set wlan_group_name='".$_POST[$wlan_group]."' where id=".$id);
        break;
    case 2:
        if($_POST[$wireless_group] == null || $_POST[$wireless_group] == "")
            $config_mask = $config_mask & 5;
        else
            $config_mask = $config_mask | $flag;
        $dbhelper->update("update group_relation set wireless_group_name='".$_POST[$wireless_group]."' where id=".$id);
        break;
    case 4:
        if($_POST[$function_group] == null || $_POST[$function_group] == "")
            $config_mask = $config_mask & 3;
        else
            $config_mask = $config_mask | $flag;
        $dbhelper->update("update group_relation set function_group_name='".$_POST[$function_group]."' where id=".$id);
        break;
    }   
    $update_ap_info = $dbhelper->update("update ap_info set config_mask = ".$config_mask." where ap_group_name = '".$ap_group_name."'");
    echo "<script>alert('应用成功！');location='group_relation.php';</script>";
        
?>