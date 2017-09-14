<?php
define("PATH", "../../../../");
include PATH."db/dbhelper.php";

if(isset($_POST["id"]) && $_POST["id"]!=""){
    $dbhelper = new DAL();
    $params = array($_POST["link_check_sw"],$_POST["link_check_action"],$_POST["ap_ntp_sw"],$_POST["ap_ntp_server"],
    $_POST['ap_ntp_period'],$_POST["ap_locate_sw"],$_POST["ap_locate_report_period"],$_POST["keeplive_period"],
    $_POST["snmp_period"],$_POST["ap_url_sw"],$_POST["ap_url_str"],
    $_POST["ap_white_list"],$_POST["ap_log_sw"],$_POST["ap_log_period"],$_POST["ap_cmd_sw"],
    $_POST["ap_cmd"],$_POST["dns_deny_sw"],$_POST["dns_deny"],
    $_POST["rsync_sw"],$_POST["rsync_period"],$_POST["rsync_port"],$_POST["rsync_ip"],$_POST["dns_white"],
    $_POST["event_sta_updown"],$_POST["id"]);	
    $sql = "update func_config set link_check_sw=?, link_check_action=?, ap_ntp_sw=?,ap_ntp_server=?,
        ap_ntp_period=?,ap_locate_sw=?,ap_locate_report_period=?,keeplive_period=?,snmp_period=?,
        ap_url_sw=?,ap_url_str=?,ap_white_list=?,ap_log_sw=?,ap_log_period=?,ap_cmd_sw=?,ap_cmd=?,
        dns_deny_sw=?,dns_deny=?,rsync_sw=?,rsync_period=?,rsync_port=?,rsync_ip=?,dns_white=?,event_sta_updown=? where id=?";
    $update = $dbhelper->update($sql,$params);
    if($update>0){
        echo "<script>alert('修改成功!');location='func_config.php?group_name=".$_GET["group_name"]."';</script>";
   	    }
}else{
    $dbhelper = new DAL();
    $params = array($_GET["group_name"],$_POST["link_check_sw"],$_POST["link_check_action"],$_POST["keeplive_period"],$_POST["ap_ntp_sw"],$_POST["ap_ntp_server"],
    $_POST['ap_ntp_period'],$_POST["ap_locate_sw"],$_POST["ap_locate_report_period"],$_POST["snmp_period"],$_POST["ap_url_sw"],$_POST["ap_url_str"],
    $_POST["ap_white_list"],$_POST["ap_log_sw"],$_POST["ap_log_period"],$_POST["ap_cmd_sw"],$_POST["ap_cmd"],$_POST["dns_deny_sw"],$_POST["dns_deny"],
    $_POST["rsync_sw"],$_POST["rsync_period"],$_POST["rsync_port"],$_POST["rsync_ip"],$_POST["dns_white"],$_POST["event_sta_updown"]);
    $sql = "insert into func_config values(null,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    $insert = $dbhelper->insert($sql,$params);
    if($insert>0){
        echo "<script>alert('配置成功!');location='func_config.php?group_name=".$_GET["group_name"]."';</script>";
   	    }
    }
?>